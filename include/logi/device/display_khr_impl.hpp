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

#ifndef LOGI_DEVICE_DISPLAY_KHR_IMPL_HPP
#define LOGI_DEVICE_DISPLAY_KHR_IMPL_HPP

#include <iostream>
#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;

class DisplayKHRImpl : public VulkanObject<DisplayKHRImpl> {
 public:
  DisplayKHRImpl(PhysicalDeviceImpl& physicalDevice, const vk::DisplayPropertiesKHR& displayProperties);

  // region Vulkan Declarations

  // endregion

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  operator const vk::DisplayKHR&() const;

  // endregion

 private:
  PhysicalDeviceImpl& physicalDevice_;
  vk::DisplayKHR vkDisplayKHR_;
  std::string displayName_;
  vk::Extent2D physicalDimensions_;
  vk::Extent2D physicalResolution_;
  vk::SurfaceTransformFlagsKHR supportedTransforms_;
  vk::Bool32 planeReorderPossible_;
  vk::Bool32 persistentContent_;
};

} // namespace logi

#endif // LOGI_DEVICE_DISPLAY_KHR_IMPL_HPP
