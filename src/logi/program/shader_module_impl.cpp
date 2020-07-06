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

#include "logi/program/shader_module_impl.hpp"
#include <numeric>
#include <spirv_cross.hpp>
#include <utility>
#include "logi/device/logical_device_impl.hpp"

namespace logi {

SpecializationConstantReflectionInfo::SpecializationConstantReflectionInfo(uint32_t id, uint32_t size)
  : id(id), size(size) {}

DescriptorBindingReflectionInfo::DescriptorBindingReflectionInfo(std::string name, const vk::ShaderStageFlags& stages,
                                                                 uint32_t binding, vk::DescriptorType descriptorType,
                                                                 uint32_t descriptorCount)
  : name(std::move(name)), stages(stages), binding(binding), descriptorType(descriptorType),
    descriptorCount(descriptorCount) {}

DescriptorSetReflectionInfo::DescriptorSetReflectionInfo(uint32_t set,
                                                         std::vector<DescriptorBindingReflectionInfo> bindings)
  : set(set), bindings(std::move(bindings)) {}

VertexAttributeReflectionInfo::VertexAttributeReflectionInfo(std::string name, uint32_t location, uint32_t elementSize,
                                                             vk::Format format)
  : name(std::move(name)), location(location), elementSize(elementSize), format(format) {}

PushConstantReflectionInfo::PushConstantReflectionInfo(const vk::ShaderStageFlags& stages, uint32_t offset,
                                                       uint32_t size)
  : stages(stages), offset(offset), size(size) {}

EntryPointReflectionInfo::EntryPointReflectionInfo(std::string name, vk::ShaderStageFlagBits stage,
                                                   std::vector<DescriptorSetReflectionInfo> descriptorSets,
                                                   std::optional<PushConstantReflectionInfo> pushConstants,
                                                   std::vector<VertexAttributeReflectionInfo> vertexAttributes)
  : name(std::move(name)), stage(stage), descriptorSets(std::move(descriptorSets)),
    pushConstants(std::move(pushConstants)), vertexAttributes(std::move(vertexAttributes)) {}

ShaderModuleImpl::ShaderModuleImpl(LogicalDeviceImpl& logicalDevice, const vk::ShaderModuleCreateInfo& create_info,
                                   const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator), vkShaderModule_(nullptr) {
  vk::Device vk_device = logicalDevice;
  vkShaderModule_ =
    vk_device.createShaderModule(create_info, allocator_ ? &allocator.value() : nullptr, getDispatcher());

  reflect(create_info.pCode, create_info.codeSize);
}

std::vector<std::string> ShaderModuleImpl::getEntryPointNames() const {
  std::vector<std::string> entryPoints;
  entryPoints.reserve(reflectionData_.size());

  for (auto const& entry : reflectionData_) {
    entryPoints.emplace_back(entry.first);
  }

  return entryPoints;
}

const EntryPointReflectionInfo& ShaderModuleImpl::getEntryPointReflectionInfo(const std::string& entryPointName) const {
  return reflectionData_.at(entryPointName);
}

const std::vector<DescriptorSetReflectionInfo>&
  ShaderModuleImpl::getDescriptorSetReflectionInfo(const std::string& entryPointName) const {
  return reflectionData_.at(entryPointName).descriptorSets;
}

const std::optional<PushConstantReflectionInfo>&
  ShaderModuleImpl::getPushConstantReflectionInfo(const std::string& entryPointName) const {
  return reflectionData_.at(entryPointName).pushConstants;
}

const std::vector<VertexAttributeReflectionInfo>&
  ShaderModuleImpl::getVertexAttributeReflectionInfo(const std::string& entryPointName) const {
  return reflectionData_.at(entryPointName).vertexAttributes;
}

void ShaderModuleImpl::reflect(const uint32_t* pCode, size_t codeSize) {
  spirv_cross::Compiler compiler(pCode, codeSize / 4u);

  auto spirvEntryPoints = compiler.get_entry_points_and_stages();
  for (const auto& entry : spirvEntryPoints) {
    vk::ShaderStageFlagBits stage = ShaderModuleImpl::executionModelToStage(entry.execution_model);
    compiler.set_entry_point(entry.name, entry.execution_model);

    reflectionData_.emplace(entry.name,
                            EntryPointReflectionInfo(entry.name, stage, reflectDescriptorSets(compiler, stage),
                                                     reflectPushConstants(compiler, stage),
                                                     reflectVertexAttributes(compiler, stage)));
  }
}

std::vector<DescriptorSetReflectionInfo> ShaderModuleImpl::reflectDescriptorSets(const spirv_cross::Compiler& compiler,
                                                                                 vk::ShaderStageFlagBits stage) {
  // Look at: http://vulkan-spec-chunked.ahcox.com/ch13s01.html
  std::vector<DescriptorSetReflectionInfo> descriptorSets;

  auto reflectDescriptorBinding = [&](uint32_t resourceId, vk::DescriptorType type) {
    const auto& resource_type = compiler.get_type_from_variable(resourceId);

    // Parse data.
    const std::string& name = compiler.get_name(resourceId);
    uint32_t set = compiler.get_decoration(resourceId, spv::DecorationDescriptorSet);
    uint32_t binding = compiler.get_decoration(resourceId, spv::DecorationBinding);
    uint32_t count = std::accumulate(resource_type.array.begin(), resource_type.array.end(), 1, std::multiplies<>());

    // Add descriptor sets if set is out of range.
    if (descriptorSets.size() <= set) {
      descriptorSets.reserve(set + 1u);
      for (uint32_t i = descriptorSets.size(); i < set + 1u; i++) {
        descriptorSets.emplace_back(i);
      }
    }

    DescriptorSetReflectionInfo& descriptorSet = descriptorSets[set];

    // Sorted binding insert.
    auto it = std::upper_bound(descriptorSet.bindings.begin(), descriptorSet.bindings.end(), binding,
                               [](const uint32_t binding, const DescriptorBindingReflectionInfo& bindingInfo) {
                                 return binding < bindingInfo.binding;
                               });

    descriptorSet.bindings.emplace(it, name, stage, binding, type, count);
  };

  spirv_cross::ShaderResources shaderResources =
    compiler.get_shader_resources(compiler.get_active_interface_variables());

  // Add uniform buffers meta.
  for (auto& resource : shaderResources.uniform_buffers) {
    reflectDescriptorBinding(resource.id, vk::DescriptorType::eUniformBuffer);
  }

  // Add storage buffers meta.
  for (auto& resource : shaderResources.storage_buffers) {
    reflectDescriptorBinding(resource.id, vk::DescriptorType::eStorageBuffer);
  }

  // Add storage images meta.
  for (auto& resource : shaderResources.storage_images) {
    if (compiler.get_type_from_variable(resource.id).image.dim == spv::DimBuffer) {
      // If it's a buffer it is a storage texel buffer not storage image.
      reflectDescriptorBinding(resource.id, vk::DescriptorType::eStorageTexelBuffer);
    } else {
      reflectDescriptorBinding(resource.id, vk::DescriptorType::eStorageImage);
    }
  }

  // Add sampled images (separate) meta.
  for (auto& resource : shaderResources.separate_images) {
    reflectDescriptorBinding(resource.id, vk::DescriptorType::eSampledImage);
  }

  // Add sampled images (combined with sampler) meta.
  for (auto& resource : shaderResources.sampled_images) {
    // If it's a buffer it is a uniform texel buffer not combined image sampler.
    if (compiler.get_type_from_variable(resource.id).image.dim == spv::DimBuffer) {
      reflectDescriptorBinding(resource.id, vk::DescriptorType::eUniformTexelBuffer);
    } else {
      reflectDescriptorBinding(resource.id, vk::DescriptorType::eCombinedImageSampler);
    }
  }

  // Add sampler meta (used in conjunction with a sampled image (separate).
  for (auto& resource : shaderResources.separate_samplers) {
    reflectDescriptorBinding(resource.id, vk::DescriptorType::eSampler);
  }

  // Add sub-pass attachments
  for (auto& resource : shaderResources.subpass_inputs) {
    reflectDescriptorBinding(resource.id, vk::DescriptorType::eInputAttachment);
  }

  for (auto& resource : shaderResources.acceleration_structures) {
    reflectDescriptorBinding(resource.id, vk::DescriptorType::eAccelerationStructureNV);
  }

  return descriptorSets;
}

std::optional<PushConstantReflectionInfo> ShaderModuleImpl::reflectPushConstants(const spirv_cross::Compiler& compiler,
                                                                                 vk::ShaderStageFlagBits stage) {
  spirv_cross::ShaderResources shaderResources =
    compiler.get_shader_resources(compiler.get_active_interface_variables());

  if (!shaderResources.push_constant_buffers.empty()) {
    auto pushConstantBuffer = shaderResources.push_constant_buffers[0];
    const std::string& name = compiler.get_name(pushConstantBuffer.id);

    // Find the beginning and the end of the buffer.
    // size_t bufferStart = std::numeric_limits<size_t>::max();
    size_t bufferStart = 0u;
    size_t bufferEnd = 0u;

    for (const spirv_cross::BufferRange& constMemberRange : compiler.get_active_buffer_ranges(pushConstantBuffer.id)) {
      bufferStart = std::min<size_t>(bufferStart, constMemberRange.offset);
      bufferEnd = std::max<size_t>(bufferEnd, constMemberRange.offset + constMemberRange.range);
    }

    return std::make_optional<PushConstantReflectionInfo>(stage, bufferStart, bufferEnd - bufferStart);
  }

  return {};
}

std::vector<VertexAttributeReflectionInfo>
  ShaderModuleImpl::reflectVertexAttributes(const spirv_cross::Compiler& compiler, vk::ShaderStageFlagBits stage) {
  if (stage != vk::ShaderStageFlagBits::eVertex) {
    return {};
  }

  spirv_cross::ShaderResources shaderResources =
    compiler.get_shader_resources(compiler.get_active_interface_variables());

  std::vector<VertexAttributeReflectionInfo> vertexAttributes;
  vertexAttributes.reserve(shaderResources.stage_inputs.size());

  for (auto& input : shaderResources.stage_inputs) {
    const std::string& name = compiler.get_name(input.id);
    const uint32_t binding = compiler.get_decoration(input.id, spv::DecorationBinding);
    const uint32_t location = compiler.get_decoration(input.id, spv::DecorationLocation);
    const spirv_cross::SPIRType& resourceType = compiler.get_type_from_variable(input.id);
    const vk::Format format = SPIRTypeToVertexBufferFormat(resourceType);
    const uint32_t elementSize = (resourceType.width * resourceType.vecsize * resourceType.columns) / 8;

    // Sorted binding description insert.
    auto bindingIt = std::upper_bound(
      vertexAttributes.begin(), vertexAttributes.end(), std::make_pair(location, binding),
      [](const std::pair<uint32_t, uint32_t> position, const VertexAttributeReflectionInfo& attributeInfo) {
        return position.first < attributeInfo.location;
      });

    vertexAttributes.emplace(bindingIt, name, location, elementSize, format);
  }

  return vertexAttributes;
}

VulkanInstanceImpl& ShaderModuleImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& ShaderModuleImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& ShaderModuleImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& ShaderModuleImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void ShaderModuleImpl::destroy() const {
  logicalDevice_.destroyShaderModule(id());
}

ShaderModuleImpl::operator const vk::ShaderModule&() const {
  return vkShaderModule_;
}

void ShaderModuleImpl::free() {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkShaderModule_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

vk::ShaderStageFlagBits ShaderModuleImpl::executionModelToStage(spv::ExecutionModel execModel) {
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

vk::Format ShaderModuleImpl::SPIRTypeToVertexBufferFormat(const spirv_cross::SPIRType& formatInfo) {
  switch (formatInfo.width) {
    case 8:
      switch (formatInfo.basetype) {
        case spirv_cross::SPIRType::BaseType::Int:
          switch (formatInfo.vecsize) {
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
          switch (formatInfo.vecsize) {
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
      switch (formatInfo.basetype) {
        case spirv_cross::SPIRType::BaseType::Int:
          switch (formatInfo.vecsize) {
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
          switch (formatInfo.vecsize) {
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
          switch (formatInfo.vecsize) {
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
      switch (formatInfo.basetype) {
        case spirv_cross::SPIRType::BaseType::Int:
          switch (formatInfo.vecsize) {
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
          switch (formatInfo.vecsize) {
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
          switch (formatInfo.vecsize) {
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
      switch (formatInfo.basetype) {
        case spirv_cross::SPIRType::BaseType::Int:
          switch (formatInfo.vecsize) {
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
          switch (formatInfo.vecsize) {
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
          switch (formatInfo.vecsize) {
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

} // namespace logi
