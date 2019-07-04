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

#include "logi/synchronization/fence.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/synchronization/fence_impl.hpp"

namespace logi {

vk::Result Fence::getStatus() const {
  return object_->getStatus();
}

vk::Result Fence::wait(const std::vector<vk::Fence>& fences, vk::Bool32 waitAll, uint64_t timeout) const {
  return object_->wait(fences, waitAll, timeout);
}
vk::Result Fence::wait(uint64_t timeout) const {
  return object_->wait(timeout);
}

vk::ResultValueType<void>::type Fence::reset(const std::vector<vk::Fence>& fences) const {
  return object_->reset(fences);
}

vk::ResultValueType<void>::type Fence::reset() const {
  return object_->reset();
}

vk::ResultValueType<void>::type Fence::importFdKHR(const vk::FenceImportFlags& flags,
                                                   vk::ExternalFenceHandleTypeFlagBits handleType, int fd,
                                                   const ConstVkNextProxy<vk::ImportFenceFdInfoKHR>& next) const {
  return object_->importFdKHR(flags, handleType, fd, next);
}

#ifdef VK_USE_PLATFORM_WIN32_KHR

vk::ResultValueType<void>::type
  Fence::importWin32HandleKHR(const vk::FenceImportFlags& flags, vk::ExternalFenceHandleTypeFlagBits handleType,
                              HANDLE handle, LPCWSTR name,
                              const ConstVkNextProxy<vk::ImportFenceWin32HandleInfoKHR>& next = {}) const {
  return object_->importFdKHR(flags, handleType, handle, name, next);
}

#endif

vk::ResultValueType<int>::type Fence::getFdKHR(vk::ExternalFenceHandleTypeFlagBits handleType,
                                               const ConstVkNextProxy<vk::FenceGetFdInfoKHR>& next) const {
  return object_->getFdKHR(handleType, next);
}

vk::Result Fence::wait(const std::vector<Fence>& fences, vk::Bool32 waitAll, uint64_t timeout) {
  std::vector<vk::Fence> vkFences(fences.begin(), fences.end());
  return fences[0].wait(vkFences, waitAll, timeout);
}

vk::ResultValueType<void>::type Fence::reset(const std::vector<Fence>& fences) {
  return fences[0].reset(std::vector<vk::Fence>(fences.begin(), fences.end()));
}

VulkanInstance Fence::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice Fence::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice Fence::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& Fence::getDispatcher() const {
  return object_->getDispatcher();
}

void Fence::destroy() const {
  object_->destroy();
}

Fence::operator const vk::Fence&() const {
  return object_->operator const vk::Fence&();
}

} // namespace logi
