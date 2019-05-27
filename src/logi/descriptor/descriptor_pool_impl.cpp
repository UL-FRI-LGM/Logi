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

#include "logi/descriptor/descriptor_pool_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

DescriptorPoolImpl::DescriptorPoolImpl(LogicalDeviceImpl& logicalDevice, const vk::DescriptorPoolCreateInfo& createInfo,
                                       const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  vk::Device vkDevice = logicalDevice_;
  vkDescriptorPool_ =
    vkDevice.createDescriptorPool(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

// region Vulkan Declarations

vk::ResultValueType<void>::type DescriptorPoolImpl::reset(const vk::DescriptorPoolResetFlags& flags) const {
  vk::Device vkDevice = logicalDevice_;
  vkDevice.resetDescriptorPool(vkDescriptorPool_, flags, getDispatcher());
}

// endregion

// region Logi Declarations

VulkanInstanceImpl& DescriptorPoolImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& DescriptorPoolImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& DescriptorPoolImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& DescriptorPoolImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void DescriptorPoolImpl::destroy() const {
  // TODO
}

DescriptorPoolImpl::operator vk::DescriptorPool() const {
  return vkDescriptorPool_;
}

void DescriptorPoolImpl::free() {
  vk::Device vkDevice = logicalDevice_;
  vkDevice.destroy(vkDescriptorPool_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

// endregion

} // namespace logi
