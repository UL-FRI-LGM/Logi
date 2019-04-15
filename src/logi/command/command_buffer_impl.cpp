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

#include "logi/command/command_buffer_impl.hpp"
#include "logi/command/command_pool_impl.hpp"

namespace logi {

CommandBufferImpl::CommandBufferImpl(CommandPoolImpl& commandPool, const vk::CommandBuffer& vkCommandBuffer)
  : commandPool_(commandPool), vkCommandBuffer_(vkCommandBuffer) {}

VulkanInstanceImpl& CommandBufferImpl::getInstance() const {
  return commandPool_.getInstance();
}

PhysicalDeviceImpl& CommandBufferImpl::getPhysicalDevice() const {
  return commandPool_.getPhysicalDevice();
}

LogicalDeviceImpl& CommandBufferImpl::getLogicalDevice() const {
  return commandPool_.getLogicalDevice();
}

CommandPoolImpl& CommandBufferImpl::getCommandPool() const {
  return commandPool_;
}

const vk::DispatchLoaderDynamic& CommandBufferImpl::getDispatcher() const {
  return commandPool_.getDispatcher();
}

void CommandBufferImpl::destroy() const {
  // TODO
}

CommandBufferImpl::operator vk::CommandBuffer() const {
  return vkCommandBuffer_;
}

void CommandBufferImpl::free() {
  vkCommandBuffer_ = nullptr;
  VulkanObject::free();
}

}