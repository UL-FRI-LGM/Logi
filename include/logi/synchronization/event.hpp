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

#ifndef LOGI_SYNCHRONIZATION_EVENT_HPP
#define LOGI_SYNCHRONIZATION_EVENT_HPP

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"

namespace logi {

class EventImpl;
class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;

class Event : public Handle<EventImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetEventStatus.html">vkGetEventStatus</a>
   */
  bool getStatus() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkResetEvent.html">vkResetEvent</a>
   */
  vk::ResultValueType<void>::type reset() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkSetEvent.html">vkSetEvent</a>
   */
  vk::ResultValueType<void>::type set() const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::Event&() const;

  // endregion
};

} // namespace logi

#endif // LOGI_SYNCHRONIZATION_EVENT_HPP
