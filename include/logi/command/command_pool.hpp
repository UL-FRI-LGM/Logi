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

#ifndef LOGI_COMMAND_COMMAND_POOL_HPP
#define LOGI_COMMAND_COMMAND_POOL_HPP

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"
#include "logi/command/command_buffer.hpp"
#include "logi/command/command_buffer_impl.hpp"
#include "logi/command/command_pool_impl.hpp"

namespace logi {

class CommandPoolImpl;
class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;
class QueueFamily;
class CommandBuffer;

class CommandPool : public Handle<CommandPoolImpl> {
 public:
  using Handle::Handle;

  // region Sub-Handles
  
  /**
   * @brief Allocate multiple command buffers. Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkAllocateCommandBuffers.html">vkAllocateCommandBuffers</a>
   */
  std::vector<CommandBuffer>
    allocateCommandBuffers(vk::CommandBufferLevel level, uint32_t commandBufferCount,
                           const ConstVkNextProxy<vk::CommandBufferAllocateInfo>& next = {}) const;

  /**
   * @brief Allocate one command buffers. Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkAllocateCommandBuffers.html">vkAllocateCommandBuffers</a>
   */
  CommandBuffer allocateCommandBuffer(vk::CommandBufferLevel level,
                                      const ConstVkNextProxy<vk::CommandBufferAllocateInfo>& next = {}) const;

  // endregion

  // region Vulkan Declarations

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkFreeCommandBuffers.html">vkFreeCommandBuffers</a>
   */
  void freeCommandBuffers(const std::vector<CommandBuffer>& commandBuffers) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkResetCommandPool.html">vkResetCommandPool</a>
   */
  vk::ResultValueType<void>::type reset(const vk::CommandPoolResetFlags& flags = vk::CommandPoolResetFlags()) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkTrimCommandPool.html">vkTrimCommandPool</a>
   */
  void trim(const vk::CommandPoolTrimFlags& flags = vk::CommandPoolTrimFlags()) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkTrimCommandPoolKHR.html">vkTrimCommandPoolKHR</a>
   */
  void trimKHR(const vk::CommandPoolTrimFlags& flags = vk::CommandPoolTrimFlags()) const;

  // end region

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  QueueFamily getQueueFamily() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::CommandPool&() const;

  // endregion
};

} // namespace logi

#endif // LOGI_COMMAND_COMMAND_POOL_HPP
