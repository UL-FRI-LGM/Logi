/**
 * Project Logi source code
 * Copyright (C) 2019 Primoz Lavric
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "logi/shader_reflection/shader_reflector.hpp"

namespace logi {

void ShaderReflector::reflectShader(const uint32_t* code, size_t wordSize, const std::string& entryPoint) {
  spirv_cross::Compiler compiler(code, wordSize);
  reflectShaderInternal(compiler, entryPoint);
}

void ShaderReflector::reflectShader(const uint32_t* code, size_t wordSize,
                                    const std::vector<std::string>& entryPoints) {
  spirv_cross::Compiler compiler(code, wordSize);

  for (const auto& entryPoint : entryPoints) {
    reflectShaderInternal(compiler, entryPoint);
  }
}

void ShaderReflector::reflectShaderInternal(spirv_cross::Compiler& compiler, const std::string& entryPoint) {
  spv::ExecutionModel executionModel = getEntryPointExecutionModel(compiler, entryPoint);
  vk::ShaderStageFlagBits stage = executionModelToStage(executionModel);

  // Tell the compiler which entry point will be used.
  compiler.set_entry_point(entryPoint, executionModel);

  // Check if this stage is already present.
  if (stages & stage) {
    throw ReflectionError("Stage '" + vk::to_string(stage) + "' is already present.");
  }

  // TODO: Validate stage combinations
  stages |= stage;

  // Fetch shader resources.
  spirv_cross::ShaderResources shaderResources = compiler.get_shader_resources();

  // If it's a vertex shader reflect vertex input state.
  if (stage == vk::ShaderStageFlagBits::eVertex) {
    reflectVertexAttributes(compiler, shaderResources);
  }

  // Look at: http://vulkan-spec-chunked.ahcox.com/ch13s01.html

  // Add uniform buffers meta.
  for (auto& resource : shaderResources.uniform_buffers) {
    reflectDescriptorBinding(compiler, stage, resource.id, vk::DescriptorType::eUniformBuffer);
  }

  // Add storage buffers meta.
  for (auto& resource : shaderResources.storage_buffers) {
    reflectDescriptorBinding(compiler, stage, resource.id, vk::DescriptorType::eStorageBuffer);
  }

  // Add storage images meta.
  for (auto& resource : shaderResources.storage_images) {
    if (compiler.get_type_from_variable(resource.id).image.dim == spv::DimBuffer) {
      // If it's a buffer it is a storage texel buffer not storage image.
      reflectDescriptorBinding(compiler, stage, resource.id, vk::DescriptorType::eStorageTexelBuffer);
    } else {
      reflectDescriptorBinding(compiler, stage, resource.id, vk::DescriptorType::eStorageImage);
    }
  }

  // Add sampled images (separate) meta.
  for (auto& resource : shaderResources.separate_images) {
    reflectDescriptorBinding(compiler, stage, resource.id, vk::DescriptorType::eSampledImage);
  }

  // Add sampled images (combined with sampler) meta.
  for (auto& resource : shaderResources.sampled_images) {
    // If it's a buffer it is a uniform texel buffer not combined image sampler.
    if (compiler.get_type_from_variable(resource.id).image.dim == spv::DimBuffer) {
      reflectDescriptorBinding(compiler, stage, resource.id, vk::DescriptorType::eUniformTexelBuffer);
    } else {
      reflectDescriptorBinding(compiler, stage, resource.id, vk::DescriptorType::eCombinedImageSampler);
    }
  }

  // Add sampler meta (used in conjunction with a sampled image (separate).
  for (auto& resource : shaderResources.separate_samplers) {
    reflectDescriptorBinding(compiler, stage, resource.id, vk::DescriptorType::eSampler);
  }

  // Add sub-pass attachments
  for (auto& resource : shaderResources.subpass_inputs) {
    reflectDescriptorBinding(compiler, stage, resource.id, vk::DescriptorType::eInputAttachment);
  }

  // Reflect push constants.
  reflectPushConstants(compiler, shaderResources, stage);
}

void ShaderReflector::reflectPushConstants(const spirv_cross::Compiler& compiler,
                                           const spirv_cross::ShaderResources& resources,
                                           vk::ShaderStageFlagBits stage) {
  auto& pipelineLayout = layout.get<PipelineLayoutCreateInfo>();

  for (auto& resource : resources.push_constant_buffers) {
    for (const spirv_cross::BufferRange& constRange : compiler.get_active_buffer_ranges(resource.id)) {
      // Check if same push constant was already registered for different stage.
      auto it = std::find_if(pipelineLayout.pushConstantRanges.begin(), pipelineLayout.pushConstantRanges.end(),
                             [&constRange](const PushConstantRange& entry) {
                               return entry.offset == constRange.offset && entry.size == constRange.range;
                             });

      if (it != pipelineLayout.pushConstantRanges.end()) {
        it->stageFlags |= stage;
      }

      pipelineLayout.pushConstantRanges.emplace_back(stage, constRange.offset, constRange.range);
    }
  }
}

void ShaderReflector::reflectDescriptorBinding(const spirv_cross::Compiler& compiler, vk::ShaderStageFlagBits stage,
                                               uint32_t resourceId, vk::DescriptorType type) {
  const auto& resource_type = compiler.get_type_from_variable(resourceId);

  // Parse data.
  uint32_t set = compiler.get_decoration(resourceId, spv::DecorationDescriptorSet);
  uint32_t binding = compiler.get_decoration(resourceId, spv::DecorationBinding);
  uint32_t count = std::accumulate(resource_type.array.begin(), resource_type.array.end(), 1, std::multiplies<>());

  // Add descriptor sets if set is out of range.
  if (descriptorSets.size() <= set) {
    descriptorSets.resize(set + 1u);
  }

  auto& setLayoutCI = descriptorSets[set].get<DescriptorSetLayoutCreateInfo>();

  // Look for existing binding.
  for (auto& bindingLayout : setLayoutCI.bindings) {
    if (bindingLayout.binding == binding) {
      if (bindingLayout.descriptorType != type || bindingLayout.descriptorCount != count) {
        throw ReflectionError("Shader binding miss match.");
      }

      bindingLayout.stageFlags |= stage;
      return;
    }
  }

  // If existing binding was not found create a new one.
  DescriptorSetLayoutBinding& bindingLayout = setLayoutCI.bindings.emplace_back();
  bindingLayout.binding = binding;
  bindingLayout.descriptorType = type;
  bindingLayout.descriptorCount = count;
  bindingLayout.stageFlags = stage;
}

void ShaderReflector::reflectVertexAttributes(const spirv_cross::Compiler& compiler,
                                              const spirv_cross::ShaderResources& shaderResources) {
  auto& vertex_input_state = vertexInputStateCI.get<PipelineVertexInputStateCreateInfo>();

  for (auto& input : shaderResources.stage_inputs) {
    const uint32_t binding = compiler.get_decoration(input.id, spv::DecorationBinding);
    const uint32_t location = compiler.get_decoration(input.id, spv::DecorationLocation);
    const spirv_cross::SPIRType& resourceType = compiler.get_type_from_variable(input.id);
    uint32_t offset = 0;
    const vk::Format format = SPIRTypeToVertexBufferFormat(resourceType);
    const uint32_t stride = (resourceType.width * resourceType.vecsize * resourceType.columns) / 8;

    vertex_input_state.vertexBindingDescription.emplace_back(binding, stride, vk::VertexInputRate::eVertex);
    vertex_input_state.vertexAttributeDescriptions.emplace_back(location, binding, format, offset);
  }
}

spv::ExecutionModel ShaderReflector::getEntryPointExecutionModel(const spirv_cross::Compiler& compiler,
                                                                 const std::string& entryPointName) {
  std::vector<spirv_cross::EntryPoint> entryPoints = compiler.get_entry_points_and_stages();

  for (const auto& entry : entryPoints) {
    if (entry.name == entryPointName) {
      return entry.execution_model;
    }
  }

  throw logi::ReflectionError("Failed to find entry point '" + entryPointName + "'.");
}

vk::ShaderStageFlagBits ShaderReflector::executionModelToStage(spv::ExecutionModel execModel) {
  switch (execModel) {
    case spv::ExecutionModelVertex:
      return vk::ShaderStageFlagBits::eVertex;
    case spv::ExecutionModelTessellationControl:
      return vk::ShaderStageFlagBits::eTessellationControl;
    case spv::ExecutionModelTessellationEvaluation:
      return vk::ShaderStageFlagBits::eTessellationEvaluation;
    case spv::ExecutionModelGeometry:
      return vk::ShaderStageFlagBits::eGeometry;
    case spv::ExecutionModelFragment:
      return vk::ShaderStageFlagBits::eFragment;
    case spv::ExecutionModelGLCompute:
      return vk::ShaderStageFlagBits::eCompute;
    case spv::ExecutionModelTaskNV:
      return vk::ShaderStageFlagBits::eTaskNV;
    case spv::ExecutionModelMeshNV:
      return vk::ShaderStageFlagBits::eMeshNV;
    case spv::ExecutionModelRayGenerationNV:
      return vk::ShaderStageFlagBits::eRaygenNV;
    case spv::ExecutionModelIntersectionNV:
      return vk::ShaderStageFlagBits::eIntersectionNV;
    case spv::ExecutionModelAnyHitNV:
      return vk::ShaderStageFlagBits::eAnyHitNV;
    case spv::ExecutionModelClosestHitNV:
      return vk::ShaderStageFlagBits::eClosestHitNV;
    case spv::ExecutionModelMissNV:
      return vk::ShaderStageFlagBits::eMissNV;
    case spv::ExecutionModelCallableNV:
      return vk::ShaderStageFlagBits::eCallableNV;
    default:
      throw ReflectionError("Failed to determine shader stage.");
  }
}

vk::Format ShaderReflector::SPIRTypeToVertexBufferFormat(const spirv_cross::SPIRType& format_info) {
  switch (format_info.width) {
    case 8:
      switch (format_info.basetype) {
        case spirv_cross::SPIRType::BaseType::Int:
          switch (format_info.vecsize) {
            case 1:
              return vk::Format::eR8Sint;
            case 2:
              return vk::Format::eR8G8Sint;
            case 3:
              return vk::Format::eR8G8B8Sint;
            case 4:
              return vk::Format::eR8G8B8A8Sint;
            default:
              throw ReflectionError("Failed to determine VkFormat.");
          }
        case spirv_cross::SPIRType::BaseType::UInt:
          switch (format_info.vecsize) {
            case 1:
              return vk::Format::eR8Uint;
            case 2:
              return vk::Format::eR8G8Uint;
            case 3:
              return vk::Format::eR8G8B8Uint;
            case 4:
              return vk::Format::eR8G8B8A8Uint;
            default:
              throw ReflectionError("Failed to determine VkFormat.");
          }
        default:
          throw ReflectionError("Failed to determine VkFormat.");
      }
    case 16:
      switch (format_info.basetype) {
        case spirv_cross::SPIRType::BaseType::Int:
          switch (format_info.vecsize) {
            case 1:
              return vk::Format::eR16Sint;
            case 2:
              return vk::Format::eR16G16Sint;
            case 3:
              return vk::Format::eR16G16B16Sint;
            case 4:
              return vk::Format::eR16G16B16A16Sint;
            default:
              throw ReflectionError("Failed to determine VkFormat.");
          }
        case spirv_cross::SPIRType::BaseType::UInt:
          switch (format_info.vecsize) {
            case 1:
              return vk::Format::eR16Uint;
            case 2:
              return vk::Format::eR16G16Uint;
            case 3:
              return vk::Format::eR16G16B16Uint;
            case 4:
              return vk::Format::eR16G16B16A16Uint;
            default:
              throw ReflectionError("Failed to determine VkFormat.");
          }
        case spirv_cross::SPIRType::BaseType::Float:
          switch (format_info.vecsize) {
            case 1:
              return vk::Format::eR16Sfloat;
            case 2:
              return vk::Format::eR16G16Sfloat;
            case 3:
              return vk::Format::eR16G16B16Sfloat;
            case 4:
              return vk::Format::eR16G16B16A16Sfloat;
            default:
              throw ReflectionError("Failed to determine VkFormat.");
          }
        default:
          throw ReflectionError("Failed to determine VkFormat.");
      }
    case 32:
      switch (format_info.basetype) {
        case spirv_cross::SPIRType::BaseType::Int:
          switch (format_info.vecsize) {
            case 1:
              return vk::Format::eR32Sint;
            case 2:
              return vk::Format::eR32G32Sint;
            case 3:
              return vk::Format::eR32G32B32Sint;
            case 4:
              return vk::Format::eR32G32B32A32Sint;
            default:
              throw ReflectionError("Failed to determine VkFormat.");
          }
        case spirv_cross::SPIRType::BaseType::UInt:
          switch (format_info.vecsize) {
            case 1:
              return vk::Format::eR32Uint;
            case 2:
              return vk::Format::eR32G32Uint;
            case 3:
              return vk::Format::eR32G32B32Uint;
            case 4:
              return vk::Format::eR32G32B32A32Uint;
            default:
              throw ReflectionError("Failed to determine VkFormat.");
          }
        case spirv_cross::SPIRType::BaseType::Float:
          switch (format_info.vecsize) {
            case 1:
              return vk::Format::eR32Sfloat;
            case 2:
              return vk::Format::eR32G32Sfloat;
            case 3:
              return vk::Format::eR32G32B32Sfloat;
            case 4:
              return vk::Format::eR32G32B32A32Sfloat;
            default:
              throw ReflectionError("Failed to determine VkFormat.");
          }
        default:
          throw ReflectionError("Failed to determine VkFormat.");
      }
    case 64:
      switch (format_info.basetype) {
        case spirv_cross::SPIRType::BaseType::Int:
          switch (format_info.vecsize) {
            case 1:
              return vk::Format::eR64Sint;
            case 2:
              return vk::Format::eR64G64Sint;
            case 3:
              return vk::Format::eR64G64B64Sint;
            case 4:
              return vk::Format::eR64G64B64A64Sint;
            default:
              throw ReflectionError("Failed to determine VkFormat.");
          }
        case spirv_cross::SPIRType::BaseType::UInt:
          switch (format_info.vecsize) {
            case 1:
              return vk::Format::eR64Uint;
            case 2:
              return vk::Format::eR64G64Uint;
            case 3:
              return vk::Format::eR64G64B64Uint;
            case 4:
              return vk::Format::eR64G64B64A64Uint;
            default:
              throw ReflectionError("Failed to determine VkFormat.");
          }
        case spirv_cross::SPIRType::BaseType::Double:
          switch (format_info.vecsize) {
            case 1:
              return vk::Format::eR64Sfloat;
            case 2:
              return vk::Format::eR64G64Sfloat;
            case 3:
              return vk::Format::eR64G64B64Sfloat;
            case 4:
              return vk::Format::eR64G64B64A64Sfloat;
            default:
              throw ReflectionError("Failed to determine VkFormat.");
          }
        default:
          throw ReflectionError("Failed to determine VkFormat.");
      }

    default:
      throw ReflectionError("Failed to determine VkFormat.");
  }
}

}