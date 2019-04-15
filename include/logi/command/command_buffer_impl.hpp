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

#ifndef LOGI_COMMAND_COMMAND_BUFFER_IMPL_HPP
#define LOGI_COMMAND_COMMAND_BUFFER_IMPL_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;
class CommandPoolImpl;

class CommandBufferImpl : public VulkanObject<CommandBufferImpl> {
 public:
  CommandBufferImpl(CommandPoolImpl& commandPool, const vk::CommandBuffer& vkCommandBuffer);

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  CommandPoolImpl& getCommandPool() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::CommandBuffer() const;

 protected:
  void free() override;

  // endregion

 private:
  CommandPoolImpl& commandPool_;
  vk::CommandBuffer vkCommandBuffer_;
};

} // namespace logi

#endif // LOGI_COMMAND_COMMAND_BUFFER_IMPL_HPP
