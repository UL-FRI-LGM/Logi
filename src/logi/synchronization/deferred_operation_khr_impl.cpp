/**
 * Project Logi source code
 * Copyright (C) 2020 Aljaz Bogataj
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

#include "logi/synchronization/deferred_operation_khr_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

DeferredOperationKHRImpl::DeferredOperationKHRImpl(LogicalDeviceImpl& logicalDevice,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) 
    : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDeferredOperationKHR_ = vkDevice.createDeferredOperationKHR(allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

vk::Result DeferredOperationKHRImpl::join() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.deferredOperationJoinKHR(vkDeferredOperationKHR_, getDispatcher());
}

uint32_t DeferredOperationKHRImpl::getMaxConcurrency() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getDeferredOperationMaxConcurrencyKHR(vkDeferredOperationKHR_, getDispatcher());
}

vk::Result DeferredOperationKHRImpl::getResult() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getDeferredOperationResultKHR(vkDeferredOperationKHR_, getDispatcher());
}

VulkanInstanceImpl& DeferredOperationKHRImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& DeferredOperationKHRImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& DeferredOperationKHRImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& DeferredOperationKHRImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void DeferredOperationKHRImpl::destroy() const {
  logicalDevice_.destroyEvent(id());
}

DeferredOperationKHRImpl::operator const vk::DeferredOperationKHR&() const {
  return vkDeferredOperationKHR_;
}

void DeferredOperationKHRImpl::free() {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkDeferredOperationKHR_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

} // namespace logi
