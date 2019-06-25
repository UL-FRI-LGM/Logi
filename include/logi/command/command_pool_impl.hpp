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

#ifndef LOGI_COMMAND_COMMAND_POOL_IMPL_HPP
#define LOGI_COMMAND_COMMAND_POOL_IMPL_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"
#include "logi/queue/queue_family_impl.hpp"
#include "logi/structures/extension.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class CommandBufferImpl;

class CommandPoolImpl : public VulkanObject<CommandPoolImpl>, public VulkanObjectComposite<CommandBufferImpl> {
 public:
  CommandPoolImpl(QueueFamilyImpl& queueFamily, const vk::CommandPoolCreateFlags& flags = {},
                  const ConstVkNextProxy<vk::CommandPoolCreateInfo>& next = {},
                  const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Sub-Handles
  std::vector<std::shared_ptr<CommandBufferImpl>>
    allocateCommandBuffers(vk::CommandBufferLevel level, uint32_t commandBufferCount,
                           const ConstVkNextProxy<vk::CommandBufferAllocateInfo>& next = {});

  // endregion

  // region Vulkan Declarations

  void freeCommandBuffers(const std::vector<size_t>& cmdBufferIds);

  vk::ResultValueType<void>::type reset(const vk::CommandPoolResetFlags& flags = vk::CommandPoolResetFlags()) const;

  void trim(const vk::CommandPoolTrimFlags& flags = vk::CommandPoolTrimFlags()) const;

  void trimKHR(const vk::CommandPoolTrimFlags& flags = vk::CommandPoolTrimFlags()) const;

  // endregion

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  QueueFamilyImpl& getQueueFamily() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::CommandPool() const;

 protected:
  void free() override;

  // endregion

 private:
  QueueFamilyImpl& queueFamily_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::CommandPool vkCommandPool_;
};

} // namespace logi

#endif // LOGI_COMMAND_COMMAND_POOL_IMPL_HPP
