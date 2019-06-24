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

FenceImpl::FenceImpl(LogicalDeviceImpl& logicalDevice, const vk::DeviceEventInfoEXT& eventInfo,
                     const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  vk::Device vkDevice = logicalDevice_;
  vkFence_ = vkDevice.registerEventEXT(eventInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

FenceImpl::FenceImpl(LogicalDeviceImpl& logicalDevice, const vk::DisplayKHR& display,
                     const vk::DisplayEventInfoEXT& eventInfo, const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  vk::Device vkDevice = logicalDevice_;
  vkFence_ =
    vkDevice.registerDisplayEventEXT(display, eventInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

vk::Result FenceImpl::getStatus() const {
  vk::Device vkDevice = logicalDevice_;
  return vkDevice.getFenceStatus(vkFence_, getDispatcher());
}

vk::Result FenceImpl::wait(const std::vector<vk::Fence>& fences, vk::Bool32 waitAll, uint64_t timeout) const {
  vk::Device vkDevice = logicalDevice_;
  return vkDevice.waitForFences(fences, waitAll, timeout, getDispatcher());
}
vk::Result FenceImpl::wait(uint64_t timeout) const {
  vk::Device vkDevice = logicalDevice_;
  return vkDevice.waitForFences({vkFence_}, true, timeout, getDispatcher());
}

vk::ResultValueType<void>::type FenceImpl::reset(const std::vector<vk::Fence>& fences) const {
  vk::Device vkDevice = logicalDevice_;
  return vkDevice.resetFences(fences, getDispatcher());
}

vk::ResultValueType<void>::type FenceImpl::reset() const {
  vk::Device vkDevice = logicalDevice_;
  return vkDevice.resetFences({vkFence_}, getDispatcher());
}

vk::ResultValueType<void>::type FenceImpl::importFdKHR(const vk::FenceImportFlags& flags,
                                                       vk::ExternalFenceHandleTypeFlagBits handleType, int fd,
                                                       const ConstVkNextProxy<vk::ImportFenceFdInfoKHR>& next) const {
  vk::Device vkDevice = logicalDevice_;
  vk::ImportFenceFdInfoKHR importInfo(vkFence_, flags, handleType, fd);
  importInfo.pNext = next;

  return vkDevice.importFenceFdKHR(importInfo, getDispatcher());
}

#ifdef VK_USE_PLATFORM_WIN32_KHR

vk::ResultValueType<void>::type
  FenceImpl::importWin32HandleKHR(const vk::FenceImportFlags& flags, vk::ExternalFenceHandleTypeFlagBits handleType,
                                  HANDLE handle, LPCWSTR name,
                                  const ConstVkNextProxy<vk::ImportFenceWin32HandleInfoKHR>& next) const {
  vk::Device vkDevice = logicalDevice_;
  vk::ImportFenceFdInfoKHR importInfo(vkFence_, flags, handleType, handle, name);
  importInfo.pNext = next;

  return vkDevice.importFenceWin32HandleKHR(importInfo, getDispatcher());
}

#endif

vk::ResultValueType<int>::type FenceImpl::getFdKHR(vk::ExternalFenceHandleTypeFlagBits handleType,
                                                   const ConstVkNextProxy<vk::FenceGetFdInfoKHR>& next) const {
  vk::Device vkDevice = logicalDevice_;
  vk::FenceGetFdInfoKHR getFdInfo(vkFence_, handleType);
  getFdInfo.pNext = next;

  return vkDevice.getFenceFdKHR(getFdInfo, getDispatcher());
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
  logicalDevice_.destroyFence(id());
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
