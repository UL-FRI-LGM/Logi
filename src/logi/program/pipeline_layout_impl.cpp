/**
 * Project Logi source code
 * Copyright (C) 2019 Lana Besevic
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

#include "logi/program/pipeline_layout_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

PipelineLayoutImpl::PipelineLayoutImpl(LogicalDeviceImpl& logicalDevice, const vk::PipelineLayoutCreateInfo& createInfo,
                                       const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkPipelineLayout_ =
    vkDevice.createPipelineLayout(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

VulkanInstanceImpl& PipelineLayoutImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& PipelineLayoutImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& PipelineLayoutImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& PipelineLayoutImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void PipelineLayoutImpl::destroy() const {
  logicalDevice_.destroyPipelineLayout(id());
}

PipelineLayoutImpl::operator const vk::PipelineLayout&() const {
  return vkPipelineLayout_;
}

void PipelineLayoutImpl::free() {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkPipelineLayout_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

}