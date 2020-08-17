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

SemaphoreImpl::SemaphoreImpl(LogicalDeviceImpl& logicalDevice, const vk::SemaphoreCreateInfo& createInfo,
                             const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkSemaphore_ = vkDevice.createSemaphore(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

uint64_t SemaphoreImpl::getCounterValue() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getSemaphoreCounterValue(vkSemaphore_, getDispatcher());
}

VulkanInstanceImpl& SemaphoreImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& SemaphoreImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& SemaphoreImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& SemaphoreImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void SemaphoreImpl::destroy() const {
  logicalDevice_.destroySemaphore(id());
}

SemaphoreImpl::operator const vk::Semaphore&() const {
  return vkSemaphore_;
}

void SemaphoreImpl::free() {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkSemaphore_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

} // namespace logi
