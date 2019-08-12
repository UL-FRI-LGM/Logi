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

#ifndef LOGI_DEVICE_DISPLAY_MODE_KHR_HPP
#define LOGI_DEVICE_DISPLAY_MODE_KHR_HPP

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"
#include "logi/device/display_mode_khr_impl.hpp"

namespace logi {

class VulkanInstance;
class PhysicalDevice;
class DisplayKHR;

class DisplayModeKHR : public Handle<DisplayModeKHRImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  vk::ResultValueType<vk::DisplayPlaneCapabilitiesKHR>::type getDisplayPlaneCapabilitiesKHR(uint32_t planeIndex) const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  DisplayKHR getDisplay() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  operator const vk::DisplayModeKHR&() const;

  // endregion
};

} // namespace logi

#endif // LOGI_DEVICE_DISPLAY_MODE_KHR_HPP
