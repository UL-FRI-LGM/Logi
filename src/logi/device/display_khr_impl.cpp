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

#include "logi/device/display_khr_impl.hpp"
#include "logi/device/physical_device_impl.hpp"

namespace logi {

DisplayKHRImpl::DisplayKHRImpl(PhysicalDeviceImpl& physicalDevice, const vk::DisplayPropertiesKHR& displayProperties)
  : physicalDevice_(physicalDevice), vkDisplayKHR_(displayProperties.display),
    displayName_(displayProperties.displayName), physicalDimensions_(displayProperties.physicalDimensions),
    physicalResolution_(displayProperties.physicalResolution),
    supportedTransforms_(displayProperties.supportedTransforms),
    planeReorderPossible_(displayProperties.planeReorderPossible),
    persistentContent_(displayProperties.persistentContent) {}

// region Vulkan Definitions

// endregion

VulkanInstanceImpl& DisplayKHRImpl::getInstance() const {
  return physicalDevice_.getInstance();
}

PhysicalDeviceImpl& DisplayKHRImpl::getPhysicalDevice() const {
  return physicalDevice_;
}

const vk::DispatchLoaderDynamic& DisplayKHRImpl::getDispatcher() const {
  return physicalDevice_.getDispatcher();
}

DisplayKHRImpl::operator const vk::DisplayKHR&() const {
  return vkDisplayKHR_;
}

}