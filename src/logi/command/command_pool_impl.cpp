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

#include "logi/command/command_pool_impl.hpp"
#include "logi/command/command_buffer_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

CommandPoolImpl::CommandPoolImpl(LogicalDeviceImpl& logicalDevice, const vk::CommandPoolCreateInfo& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkCommandPool_ = vkDevice.createCommandPool(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

// region Vulkan Definitions

std::vector<std::shared_ptr<CommandBufferImpl>>
  CommandPoolImpl::allocateCommandBuffers(vk::CommandBufferLevel level, uint32_t commandBufferCount,
                                          const ConstVkNextProxy<vk::CommandBufferAllocateInfo>& next) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vk::CommandBufferAllocateInfo allocateInfo(vkCommandPool_, level, commandBufferCount);
  allocateInfo.pNext = next;

  std::vector<vk::CommandBuffer> cmdBuffers = vkDevice.allocateCommandBuffers(allocateInfo, getDispatcher());
  std::vector<std::shared_ptr<CommandBufferImpl>> logiCmdBuffers;
  logiCmdBuffers.reserve(cmdBuffers.size());

  for (const auto& buffer : cmdBuffers) {
    logiCmdBuffers.emplace_back(VulkanObjectComposite<CommandBufferImpl>::createObject(*this, buffer));
  }

  return logiCmdBuffers;
}

void CommandPoolImpl::freeCommandBuffers(const std::vector<size_t>& cmdBufferIds) {
  std::vector<vk::CommandBuffer> vkCmdBuffers;
  vkCmdBuffers.reserve(cmdBufferIds.size());

  // Collect VK handles and destroy logi command buffers.
  for (size_t id : cmdBufferIds) {
    vkCmdBuffers.emplace_back(static_cast<vk::CommandBuffer>(*VulkanObjectComposite<CommandBufferImpl>::getObject(id)));
    VulkanObjectComposite<CommandBufferImpl>::destroyObject(id);
  }

  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.freeCommandBuffers(vkCommandPool_, vkCmdBuffers, getDispatcher());
}

vk::ResultValueType<void>::type CommandPoolImpl::reset(const vk::CommandPoolResetFlags& flags) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.resetCommandPool(vkCommandPool_, flags, getDispatcher());
}

void CommandPoolImpl::trim(const vk::CommandPoolTrimFlags& flags) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.trimCommandPool(vkCommandPool_, flags, getDispatcher());
}

void CommandPoolImpl::trimKHR(const vk::CommandPoolTrimFlags& flags) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.trimCommandPoolKHR(vkCommandPool_, flags, getDispatcher());
}

// endregion

// region Logi Definitions

VulkanInstanceImpl& CommandPoolImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& CommandPoolImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& CommandPoolImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& CommandPoolImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void CommandPoolImpl::destroy() const {
  logicalDevice_.destroyCommandPool(id());
}

CommandPoolImpl::operator vk::CommandPool() const {
  return vkCommandPool_;
}

void CommandPoolImpl::free() {
  VulkanObjectComposite<CommandBufferImpl>::destroyAllObjects();
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkCommandPool_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

// endregion

}