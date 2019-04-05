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

FenceImpl::FenceImpl(LogicalDeviceImpl& logical_device, const vk::FenceCreateInfo& create_info,
                     const std::optional<vk::AllocationCallbacks>& allocator)
  : logical_device_(logical_device), allocator_(allocator) {
  vk::Device vk_device = logical_device_;
  vk_fence_ = vk_device.createFence(create_info, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

bool FenceImpl::getStatus() const {
  vk::Device vk_device;
  return vk_device.getFenceStatus(vk_fence_, getDispatcher()) == vk::Result::eSuccess;
}

VulkanInstanceImpl& FenceImpl::getInstance() const {
  return logical_device_.getInstance();
}

PhysicalDeviceImpl& FenceImpl::getPhysicalDevice() const {
  return logical_device_.getPhysicalDevice();
}

LogicalDeviceImpl& FenceImpl::getLogicalDevice() const {
  return logical_device_;
}

const vk::DispatchLoaderDynamic& FenceImpl::getDispatcher() const {
  return logical_device_.getDispatcher();
}

void FenceImpl::destroy() const {
  // TODO
}

FenceImpl::operator vk::Fence() const {
  return vk_fence_;
}

void FenceImpl::free() {
  vk::Device vk_device = logical_device_;
  vk_device.destroy(vk_fence_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

} // namespace logi
