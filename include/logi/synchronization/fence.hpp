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

#ifndef LOGI_SYNCHRONIZATION_FENCE_HPP
#define LOGI_SYNCHRONIZATION_FENCE_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/handle.hpp"

namespace logi {

class FenceImpl;
class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;

class Fence : public Handle<FenceImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  bool getStatus() const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::Fence() const;

  // endregion
};

} // namespace logi

#endif // LOGI_SYNCHRONIZATION_FENCE_HPP
