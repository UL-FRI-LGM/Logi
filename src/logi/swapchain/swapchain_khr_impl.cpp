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
#include "logi/memory/image_impl.hpp"

namespace logi {

SwapchainKHRImpl::SwapchainKHRImpl(LogicalDeviceImpl& logicalDevice, const vk::SwapchainCreateInfoKHR& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkSwapchainKHR_ =
    vkDevice.createSwapchainKHR(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

SwapchainKHRImpl::SwapchainKHRImpl(LogicalDeviceImpl& logicalDevice, const vk::SwapchainKHR& swapchain,
                                   const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator), vkSwapchainKHR_(swapchain) {}

// region Vulkan Definitions

std::vector<std::shared_ptr<ImageImpl>> SwapchainKHRImpl::getImagesKHR() {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  std::vector<vk::Image> vkImages = vkDevice.getSwapchainImagesKHR(vkSwapchainKHR_, getDispatcher());

  std::vector<std::shared_ptr<ImageImpl>> logiImages;
  logiImages.reserve(vkImages.size());

  for (const auto& vkImage : vkImages) {
    logiImages.emplace_back(VulkanObjectComposite<ImageImpl>::createObject(*this, vkImage));
  }

  return logiImages;
}

vk::ResultValue<uint32_t> SwapchainKHRImpl::acquireNextImageKHR(uint64_t timeout, const vk::Semaphore& semaphore,
                                                                const vk::Fence& fence) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.acquireNextImageKHR(vkSwapchainKHR_, timeout, semaphore, fence, getDispatcher());
}

vk::ResultValue<uint32_t>
  SwapchainKHRImpl::acquireNextImage2KHR(uint64_t timeout, const vk::Semaphore& semaphore, const vk::Fence& fence,
                                         uint32_t deviceMask,
                                         const ConstVkNextProxy<vk::AcquireNextImageInfoKHR>& next = {}) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);

  vk::AcquireNextImageInfoKHR acquireImageInfo(vkSwapchainKHR_, timeout, semaphore, fence, deviceMask);
  acquireImageInfo.pNext = next;

  return vkDevice.acquireNextImage2KHR(acquireImageInfo, getDispatcher());
}

vk::ResultValueType<uint64_t>::type SwapchainKHRImpl::getCounterEXT(vk::SurfaceCounterFlagBitsEXT counter) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getSwapchainCounterEXT(vkSwapchainKHR_, counter, getDispatcher());
}

vk::Result SwapchainKHRImpl::getStatusKHR() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getSwapchainStatusKHR(vkSwapchainKHR_, getDispatcher());
}

void SwapchainKHRImpl::setLocalDimmingAMD(vk::Bool32 localDimmingEnable) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.setLocalDimmingAMD(vkSwapchainKHR_, localDimmingEnable, getDispatcher());
}

typename vk::ResultValueType<std::vector<vk::PastPresentationTimingGOOGLE>>::type
  SwapchainKHRImpl::getPastPresentationTimingGOOGLE() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getPastPresentationTimingGOOGLE(vkSwapchainKHR_, getDispatcher());
}

typename vk::ResultValueType<vk::RefreshCycleDurationGOOGLE>::type
  SwapchainKHRImpl::getRefreshCycleDurationGOOGLE() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getRefreshCycleDurationGOOGLE(vkSwapchainKHR_, getDispatcher());
}

#ifdef VK_USE_PLATFORM_WIN32_KHR
vk::ResultValueType<void>::type SwapchainKHRImpl::acquireFullScreenExclusiveModeEXT() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.acquireFullScreenExclusiveModeEXT(vkSwapchainKHR_, getDispatcher());
}

vk::ResultValueType<void>::type SwapchainKHRImpl::releaseFullScreenExclusiveModeEXT() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.releaseFullScreenExclusiveModeEXT(vkSwapchainKHR_, getDispatcher());
}
#endif

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
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkSwapchainKHR_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

// endregion

} // namespace logi
