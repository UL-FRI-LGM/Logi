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

#include "logi/device/display_mode_khr_impl.hpp"
#include "logi/device/display_khr_impl.hpp"
#include "logi/device/physical_device_impl.hpp"

namespace logi {

DisplayModeKHRImpl::DisplayModeKHRImpl(DisplayKHRImpl& display, const vk::DisplayModeCreateInfoKHR& createInfo,
                                       const std::optional<vk::AllocationCallbacks>& allocator)
  : display_(display), allocator_(allocator) {}

// region Vulkan Definitions

vk::ResultValueType<vk::DisplayPlaneCapabilitiesKHR>::type
  DisplayModeKHRImpl::getDisplayPlaneCapabilitiesKHR(uint32_t planeIndex) const {
  vk::PhysicalDevice vkPhysicalDevice = getPhysicalDevice();
  return vkPhysicalDevice.getDisplayPlaneCapabilitiesKHR(vkDisplayModeKHR_, planeIndex, getDispatcher());
}

// endregion

// region Logi Definitions

VulkanInstanceImpl& DisplayModeKHRImpl::getInstance() const {
  return display_.getInstance();
}

PhysicalDeviceImpl& DisplayModeKHRImpl::getPhysicalDevice() const {
  return display_.getPhysicalDevice();
}

DisplayKHRImpl& DisplayModeKHRImpl::getDisplay() const {
  return display_;
}

const vk::DispatchLoaderDynamic& DisplayModeKHRImpl::getDispatcher() const {
  return display_.getDispatcher();
}

DisplayModeKHRImpl::operator const vk::DisplayModeKHR&() const {
  return vkDisplayModeKHR_;
}

void DisplayModeKHRImpl::free() {
  // TODO
}

// endregion

} // namespace logi
