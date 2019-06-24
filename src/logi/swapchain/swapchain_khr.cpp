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

#include "logi/swapchain/swapchain_khr.hpp"
#include "logi/device/logical_device.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/memory/image.hpp"

namespace logi {

// region Vulkan Definitions

std::vector<Image> SwapchainKHR::getSwapchainImagesKHR() const {
  std::vector<std::shared_ptr<ImageImpl>> imageImpls = object_->getSwapchainImagesKHR();
  return std::vector<Image>(imageImpls.begin(), imageImpls.end());
}

#ifdef VK_USE_PLATFORM_WIN32_KHR
vk::ResultValueType<void>::type SwapchainKHR::acquireFullScreenExclusiveModeEXT() const {
  return object_->acquireFullScreenExclusiveModeEXT();
}
#endif

vk::ResultValue<uint32_t> SwapchainKHR::acquireNextImageKHR(uint64_t timeout, const vk::Semaphore& semaphore,
                                                            const vk::Fence& fence) const {
  return object_->acquireNextImageKHR(timeout, semaphore, fence);
}

vk::ResultValue<uint32_t>
  SwapchainKHR::acquireNextImage2KHR(uint64_t timeout, const vk::Semaphore& semaphore, const vk::Fence& fence,
                                     uint32_t deviceMask,
                                     const ConstVkNextProxy<vk::AcquireNextImageInfoKHR>& next) const {
  return object_->acquireNextImage2KHR(timeout, semaphore, fence, deviceMask, next);
}

vk::ResultValueType<uint64_t>::type SwapchainKHR::getCounterEXT(vk::SurfaceCounterFlagBitsEXT counter) const {
  return object_->getCounterEXT(counter);
}

vk::Result SwapchainKHR::getStatusKHR() const {
  return object_->getStatusKHR();
}

void SwapchainKHR::setLocalDimmingAMD(vk::Bool32 localDimmingEnable) const {
  object_->setLocalDimmingAMD(localDimmingEnable);
}

typename vk::ResultValueType<std::vector<vk::PastPresentationTimingGOOGLE>>::type
  SwapchainKHR::getPastPresentationTimingGOOGLE() const {
  return object_->getPastPresentationTimingGOOGLE();
}

typename vk::ResultValueType<vk::RefreshCycleDurationGOOGLE>::type SwapchainKHR::getRefreshCycleDurationGOOGLE() const {
  return object_->getRefreshCycleDurationGOOGLE();
}

// endregion

// region Vulkan Definitions

VulkanInstance SwapchainKHR::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice SwapchainKHR::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice SwapchainKHR::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& SwapchainKHR::getDispatcher() const {
  return object_->getDispatcher();
}

void SwapchainKHR::destroy() const {
  object_->destroy();
}

SwapchainKHR::operator vk::SwapchainKHR() const {
  return object_->operator vk::SwapchainKHR();
}

// endregion

} // namespace logi
