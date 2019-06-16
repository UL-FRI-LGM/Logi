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

#include "logi/swapchain/swapchain_khr_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

SwapchainKHRImpl::SwapchainKHRImpl(LogicalDeviceImpl& logicalDevice, const vk::SwapchainCreateInfoKHR& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  vk::Device vkDevice = logicalDevice_;
  vkSwapchainKHR_ =
    vkDevice.createSwapchainKHR(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

// region Vulkan Definitions

#ifdef VK_USE_PLATFORM_WIN32_KHR
vk::ResultValueType<void>::type SwapchainKHRImpl::acquireFullScreenExclusiveModeEXT() const {
  vk::Device vkDevice = logicalDevice_;
  return vkDevice.acquireFullScreenExclusiveModeEXT(vkSwapchainKHR_, getDispatcher());
}

vk::ResultValueType<void>::type SwapchainKHRImpl::releaseFullScreenExclusiveModeEXT() const {
  vk::Device vkDevice = logicalDevice_;
  return vkDevice.releaseFullScreenExclusiveModeEXT(vkSwapchainKHR_, getDispatcher());
}
#endif

vk::ResultValue<uint32_t> SwapchainKHRImpl::acquireNextImageKHR(uint64_t timeout, vk::Semaphore semaphore,
                                                                vk::Fence fence) const {
  vk::Device vkDevice = logicalDevice_;
  return vkDevice.acquireNextImageKHR(vkSwapchainKHR_, timeout, semaphore, fence, getDispatcher());
}

vk::ResultValueType<uint64_t>::type SwapchainKHRImpl::getCounterEXT(vk::SurfaceCounterFlagBitsEXT counter) const {
  vk::Device vkDevice = logicalDevice_;
  return vkDevice.getSwapchainCounterEXT(vkSwapchainKHR_, counter, getDispatcher());
}

vk::Result SwapchainKHRImpl::getStatusKHR() const {
  vk::Device vkDevice = logicalDevice_;
  return vkDevice.getSwapchainStatusKHR(vkSwapchainKHR_, getDispatcher());
}

void SwapchainKHRImpl::setLocalDimmingAMD(vk::Bool32 localDimmingEnable) const {
  vk::Device vkDevice = logicalDevice_;
  vkDevice.setLocalDimmingAMD(vkSwapchainKHR_, localDimmingEnable, getDispatcher());
}

// endregion

// region Logi Definitions

VulkanInstanceImpl& SwapchainKHRImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& SwapchainKHRImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& SwapchainKHRImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& SwapchainKHRImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void SwapchainKHRImpl::destroy() const {
  logicalDevice_.destroySwapchainKHR(id());
}

SwapchainKHRImpl::operator vk::SwapchainKHR() const {
  return vkSwapchainKHR_;
}

void SwapchainKHRImpl::free() {
  vk::Device vkDevice = logicalDevice_;
  vkDevice.destroy(vkSwapchainKHR_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

// endregion

} // namespace logi
