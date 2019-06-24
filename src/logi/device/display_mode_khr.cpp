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

#include "logi/device/display_mode_khr.hpp"
#include "logi/device/display_khr.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/vulkan_instance.hpp"

namespace logi {

// region Vulkan Definitions

vk::ResultValueType<vk::DisplayPlaneCapabilitiesKHR>::type
  DisplayModeKHR::getDisplayPlaneCapabilitiesKHR(uint32_t planeIndex) const {
  return object_->getDisplayPlaneCapabilitiesKHR(planeIndex);
}

// endregion

// region Logi Definitions

VulkanInstance DisplayModeKHR::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice DisplayModeKHR::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

DisplayKHR DisplayModeKHR::getDisplay() const {
  return DisplayKHR(object_->getDisplay().shared_from_this());
}

const vk::DispatchLoaderDynamic& DisplayModeKHR::getDispatcher() const {
  return object_->getDispatcher();
}

DisplayModeKHR::operator vk::DisplayModeKHR() const {
  return object_->operator vk::DisplayModeKHR();
}

// endregion

} // namespace logi
