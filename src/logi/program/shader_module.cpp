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

const std::vector<std::vector<DescriptorBindingReflectionInfo>>&
  ShaderModule::getDescriptorSetReflectionInfo(const std::string& entryPointName) const {
  return object_->getDescriptorSetReflectionInfo(entryPointName);
}

const std::vector<PushConstantReflectionInfo>&
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
  object_->destroy();
}

ShaderModule::operator const vk::ShaderModule&() const {
  static vk::ShaderModule nullHandle(nullptr);
  return (object_) ? object_->operator const vk::ShaderModule&() : nullHandle;
}

ShaderStage::ShaderStage(ShaderModule shader, std::string entryPoint)
  : shader(std::move(shader)), entryPoint(std::move(entryPoint)) {}

/*
const std::vector<std::vector<DescriptorBindingReflectionInfo>>&
reflectDescriptorSets(const std::vector<ShaderStage>& stages) {
if (stages.size())
}*/

// const std::vector<PushConstantReflectionInfo>& reflectPushConstants(const std::vector<ShaderStage>& stages) {}

}