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

EntryPointInfo::EntryPointInfo(std::string name, vk::ShaderStageFlagBits stage) : name(std::move(name)), stage(stage) {}

SpecializationConstantInfo::SpecializationConstantInfo(uint32_t id, uint32_t size) : id(id), size(size) {}

DescriptorBindingInfo::DescriptorBindingInfo(std::string name, uint32_t binding, vk::DescriptorType descriptorType,
                                             uint32_t descriptorCount)
  : name(std::move(name)), binding(binding), descriptorType(descriptorType), descriptorCount(descriptorCount) {}

VertexAttributeInfo::VertexAttributeInfo(std::string name, uint32_t location, uint32_t binding, uint32_t elementSize,
                                         vk::Format format)
  : name(std::move(name)), location(location), binding(binding), elementSize(elementSize), format(format) {}

PushConstantInfo::PushConstantInfo(std::string name, uint32_t offset, uint32_t size)
  : name(std::move(name)), offset(offset), size(size) {}

ShaderModuleImpl::ShaderModuleImpl(LogicalDeviceImpl& logicalDevice, const vk::ShaderModuleCreateInfo& create_info,
                                   const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator), vkShaderModule_(nullptr),
    compiler_(create_info.pCode, create_info.codeSize) {
  vk::Device vk_device = logicalDevice;
  vkShaderModule_ =
    vk_device.createShaderModule(create_info, allocator.has_value() ? nullptr : &allocator.value(), getDispatcher());
}

std::vector<EntryPointInfo> ShaderModuleImpl::reflectEntryPoints() const {
  std::vector<spirv_cross::EntryPoint> spirvEntryPoints = compiler_.get_entry_points_and_stages();

  std::vector<EntryPointInfo> entryPoints;
  entryPoints.reserve(spirvEntryPoints.size());
  for (const auto& entryPoint : spirvEntryPoints) {
    entryPoints.emplace_back(entryPoint.name, ShaderModuleImpl::executionModelToStage(entryPoint.execution_model));
  }

  return entryPoints;
}

std::vector<std::vector<DescriptorBindingInfo>> ShaderModuleImpl::reflectDescriptorSets() const {
  // Look at: http://vulkan-spec-chunked.ahcox.com/ch13s01.html
  std::vector<std::vector<DescriptorBindingInfo>> descriptorSets;

  auto reflectDescriptorBinding = [&](uint32_t resourceId, vk::DescriptorType type) {
    const auto& resource_type = compiler_.get_type_from_variable(resourceId);

    // Parse data.
    const std::string& name = compiler_.get_name(resourceId);
    uint32_t set = compiler_.get_decoration(resourceId, spv::DecorationDescriptorSet);
    uint32_t binding = compiler_.get_decoration(resourceId, spv::DecorationBinding);
    uint32_t count = std::accumulate(resource_type.array.begin(), resource_type.array.end(), 1, std::multiplies<>());

    // Add descriptor sets if set is out of range.
    if (descriptorSets.size() <= set) {
      descriptorSets.resize(set + 1u);
    }

    std::vector<DescriptorBindingInfo>& descriptorSet = descriptorSets[set];

    // Sorted binding insert.
    auto it = std::upper_bound(
      descriptorSet.begin(), descriptorSet.end(), binding,
      [](const uint32_t binding, const DescriptorBindingInfo& bindingInfo) { return binding < bindingInfo.binding; });

    descriptorSet.emplace(it, name, binding, type, count);
  };

  spirv_cross::ShaderResources shaderResources = compiler_.get_shader_resources();

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
    if (compiler_.get_type_from_variable(resource.id).image.dim == spv::DimBuffer) {
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
    if (compiler_.get_type_from_variable(resource.id).image.dim == spv::DimBuffer) {
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

  return descriptorSets;
}

std::vector<PushConstantInfo> ShaderModuleImpl::reflectPushConstants() const {
  spirv_cross::ShaderResources shaderResources = compiler_.get_shader_resources();
  std::vector<PushConstantInfo> pushConstants;
  pushConstants.reserve(shaderResources.push_constant_buffers.size());

  for (auto& resource : shaderResources.push_constant_buffers) {
    for (const spirv_cross::BufferRange& constRange : compiler_.get_active_buffer_ranges(resource.id)) {
      const std::string& name = compiler_.get_name(resource.id);

      // Sorted insert.
      auto it = std::upper_bound(
        pushConstants.begin(), pushConstants.end(), constRange.offset,
        [](const uint32_t offset, const PushConstantInfo& constantInfo) { return offset < constantInfo.offset; });

      pushConstants.emplace(it, name, constRange.offset, constRange.range);
    }
  }

  return pushConstants;
}

std::vector<VertexAttributeInfo> ShaderModuleImpl::reflectVertexAttributes() const {
  spirv_cross::ShaderResources shaderResources = compiler_.get_shader_resources();

  std::vector<VertexAttributeInfo> vertexAttributes;
  vertexAttributes.reserve(shaderResources.stage_inputs.size());

  for (auto& input : shaderResources.stage_inputs) {
    const std::string& name = compiler_.get_name(input.id);
    const uint32_t binding = compiler_.get_decoration(input.id, spv::DecorationBinding);
    const uint32_t location = compiler_.get_decoration(input.id, spv::DecorationLocation);
    const spirv_cross::SPIRType& resourceType = compiler_.get_type_from_variable(input.id);
    const vk::Format format = SPIRTypeToVertexBufferFormat(resourceType);
    const uint32_t elementSize = (resourceType.width * resourceType.vecsize * resourceType.columns) / 8;

    // Sorted binding description insert.
    auto bindingIt =
      std::upper_bound(vertexAttributes.begin(), vertexAttributes.end(), std::make_pair(location, binding),
                       [](const std::pair<uint32_t, uint32_t> position, const VertexAttributeInfo& attributeInfo) {
                         if (position.first == attributeInfo.location) {
                           return position.second < attributeInfo.binding;
                         }

                         return position.first < attributeInfo.location;
                       });

    vertexAttributes.emplace(bindingIt, name, location, binding, elementSize, format);
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

ShaderModuleImpl::operator vk::ShaderModule() const {
  return vkShaderModule_;
}

void ShaderModuleImpl::free() {
  vk::Device vkDevice = logicalDevice_;
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
