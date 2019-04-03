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

#include "logi/synchronization/semaphore_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

SemaphoreImpl::SemaphoreImpl(LogicalDeviceImpl& logical_device, const vk::SemaphoreCreateInfo& create_info,
                             const std::optional<vk::AllocationCallbacks>& allocator)
  : logical_device_(logical_device), allocator_(allocator) {
  vk::Device vk_device = logical_device_;
  vk_semaphore_ = vk_device.createSemaphore(create_info, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

VulkanInstanceImpl& SemaphoreImpl::getInstance() const {
  return logical_device_.getInstance();
}

PhysicalDeviceImpl& SemaphoreImpl::getPhysicalDevice() const {
  return logical_device_.getPhysicalDevice();
}

LogicalDeviceImpl& SemaphoreImpl::getLogicalDevice() const {
  return logical_device_;
}

const vk::DispatchLoaderDynamic& SemaphoreImpl::getDispatcher() const {
  return logical_device_.getDispatcher();
}

void SemaphoreImpl::destroy() const {
  // TODO
}

SemaphoreImpl::operator const vk::Semaphore() const {
  return vk_semaphore_;
}

void SemaphoreImpl::free() {
  vk::Device vk_device = logical_device_;
  vk_device.destroy(vk_semaphore_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

} // namespace logi
