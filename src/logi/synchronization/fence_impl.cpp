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

#include "logi/synchronization/fence_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

FenceImpl::FenceImpl(LogicalDeviceImpl& logicalDevice, const vk::FenceCreateInfo& createInfo,
                     const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  vk::Device vkDevice = logicalDevice_;
  vkFence_ = vkDevice.createFence(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

bool FenceImpl::getStatus() const {
  vk::Device vkDevice;
  return vkDevice.getFenceStatus(vkFence_, getDispatcher()) == vk::Result::eSuccess;
}

VulkanInstanceImpl& FenceImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& FenceImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& FenceImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& FenceImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void FenceImpl::destroy() const {
  // TODO
}

FenceImpl::operator vk::Fence() const {
  return vkFence_;
}

void FenceImpl::free() {
  vk::Device vkDevice = logicalDevice_;
  vkDevice.destroy(vkFence_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

} // namespace logi
