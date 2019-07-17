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

#include "logi/command/command_pool.hpp"
#include "logi/command/command_buffer_impl.hpp"
#include "logi/command/command_pool_impl.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/queue/queue_family.hpp"
#include "logi/queue/queue_family_impl.hpp"

namespace logi {

std::vector<CommandBuffer>
  CommandPool::allocateCommandBuffers(vk::CommandBufferLevel level, uint32_t commandBufferCount,
                                      const ConstVkNextProxy<vk::CommandBufferAllocateInfo>& next) const {
  std::vector<std::shared_ptr<CommandBufferImpl>> cmdBuffersImpl =
    object_->allocateCommandBuffers(level, commandBufferCount, next);
  return std::vector<CommandBuffer>(cmdBuffersImpl.begin(), cmdBuffersImpl.end());
}

CommandBuffer CommandPool::allocateCommandBuffer(vk::CommandBufferLevel level,
                                                 const ConstVkNextProxy<vk::CommandBufferAllocateInfo>& next) const {
  return CommandBuffer(object_->allocateCommandBuffer(level, next));
}

// region Vulkan Definitions

void CommandPool::freeCommandBuffers(const std::vector<CommandBuffer>& commandBuffers) const {
  std::vector<size_t> ids;
  ids.reserve(commandBuffers.size());

  for (const auto& cmdBuffer : commandBuffers) {
    ids.emplace_back(cmdBuffer.id());
  }

  object_->freeCommandBuffers(ids);
}

vk::ResultValueType<void>::type CommandPool::reset(const vk::CommandPoolResetFlags& flags) const {
  return object_->reset(flags);
}

void CommandPool::trim(const vk::CommandPoolTrimFlags& flags) const {
  object_->trim(flags);
}

void CommandPool::trimKHR(const vk::CommandPoolTrimFlags& flags) const {
  object_->trimKHR(flags);
}

// endregion

// region Logi Definitions

VulkanInstance CommandPool::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice CommandPool::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice CommandPool::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

QueueFamily CommandPool::getQueueFamily() const {
  return QueueFamily(object_->getQueueFamily().shared_from_this());
}

const vk::DispatchLoaderDynamic& CommandPool::getDispatcher() const {
  return object_->getDispatcher();
}

void CommandPool::destroy() const {
  object_->destroy();
}

CommandPool::operator const vk::CommandPool&() const {
  static vk::CommandPool nullHandle(nullptr);
  return (object_) ? object_->operator const vk::CommandPool&() : nullHandle;
}

// endregion

} // namespace logi
