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

#include "logi/program/shader_module.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/program/shader_module_impl.hpp"

namespace logi {

std::vector<std::string> ShaderModule::getEntryPointNames() const {
  return object_->getEntryPointNames();
}

const EntryPointReflectionInfo& ShaderModule::getEntryPointReflectionInfo(const std::string& entryPointName) const {
  return object_->getEntryPointReflectionInfo(entryPointName);
}

const std::vector<DescriptorSetReflectionInfo>&
  ShaderModule::getDescriptorSetReflectionInfo(const std::string& entryPointName) const {
  return object_->getDescriptorSetReflectionInfo(entryPointName);
}

const std::optional<PushConstantReflectionInfo>&
  ShaderModule::getPushConstantReflectionInfo(const std::string& entryPointName) const {
  return object_->getPushConstantReflectionInfo(entryPointName);
}
const std::vector<VertexAttributeReflectionInfo>&
  ShaderModule::getVertexAttributeReflectionInfo(const std::string& entryPointName) const {
  return object_->getVertexAttributeReflectionInfo(entryPointName);
}

VulkanInstance ShaderModule::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice ShaderModule::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice ShaderModule::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& ShaderModule::getDispatcher() const {
  return object_->getDispatcher();
}

void ShaderModule::destroy() const {
  if (object_) {
    object_->destroy();
  }
}

ShaderModule::operator const vk::ShaderModule&() const {
  static vk::ShaderModule nullHandle(nullptr);
  return (object_) ? object_->operator const vk::ShaderModule&() : nullHandle;
}

ShaderStage::ShaderStage(ShaderModule shader, std::string entryPoint)
  : shader(std::move(shader)), entryPoint(std::move(entryPoint)) {}

void mergeDescriptorSetReflectionInfo(DescriptorSetReflectionInfo& merged, const DescriptorSetReflectionInfo& other) {
  for (const auto& bindingInfo : other.bindings) {
    auto it = std::find_if(merged.bindings.begin(), merged.bindings.end(),
                           [&bindingInfo](const DescriptorBindingReflectionInfo& current) {
                             return current.binding >= bindingInfo.binding;
                           });

    if (it == merged.bindings.end() || it->binding != bindingInfo.binding) {
      merged.bindings.emplace(it, bindingInfo);
    } else {
      it->stages |= bindingInfo.stages;
    }
  }
}

std::vector<DescriptorSetReflectionInfo> reflectDescriptorSets(const std::vector<ShaderStage>& stages) {
  // No stages were given. Return empty vector.
  if (stages.empty()) {
    return {};
  }

  // Use descriptors set of the first stage as a base.
  std::vector<DescriptorSetReflectionInfo> mergedDescriptorSets =
    stages[0].shader.getDescriptorSetReflectionInfo(stages[0].entryPoint);

  for (auto it = stages.begin() + 1; it != stages.end(); it++) {
    const std::vector<DescriptorSetReflectionInfo>& descriptorSets =
      it->shader.getDescriptorSetReflectionInfo(it->entryPoint);

    // Merge common descriptor sets.
    for (size_t i = 0; i < std::min(mergedDescriptorSets.size(), descriptorSets.size()); i++) {
      mergeDescriptorSetReflectionInfo(mergedDescriptorSets[i], descriptorSets[i]);
    }

    // Add extra descriptions sets.
    if (mergedDescriptorSets.size() < descriptorSets.size()) {
      mergedDescriptorSets.reserve(descriptorSets.size());
      for (size_t i = mergedDescriptorSets.size(); i < descriptorSets.size(); i++) {
        mergedDescriptorSets.emplace_back(descriptorSets[i]);
      }
    }
  }

  return mergedDescriptorSets;
}

std::vector<PushConstantReflectionInfo> reflectPushConstants(const std::vector<ShaderStage>& stages) {
  // No stages were given. Return empty vector.
  if (stages.empty()) {
    return {};
  }

  std::vector<PushConstantReflectionInfo> pushConstants;

  for (const auto& stage : stages) {
    std::optional<PushConstantReflectionInfo> reflectionInfo =
      stage.shader.getPushConstantReflectionInfo(stage.entryPoint);

    if (reflectionInfo) {
      auto it = std::find_if(
        pushConstants.begin(), pushConstants.end(), [reflectionInfo](const PushConstantReflectionInfo& constant) {
          return constant.offset > reflectionInfo->offset ||
                 (constant.offset == reflectionInfo->offset && constant.size >= reflectionInfo->size);
        });

      // Merge stages if the ranges match. Otherwise insert a new range.
      if (it == pushConstants.end() || it->offset != reflectionInfo->offset || it->size != reflectionInfo->size) {
        pushConstants.emplace(it, *reflectionInfo);
      } else {
        it->stages |= reflectionInfo->stages;
      }
    }
  }

  return pushConstants;
}

} // namespace logi