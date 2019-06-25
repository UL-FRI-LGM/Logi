/**
 * Project Logi source code
 * Copyright (C) 2019 Primoz Lavric
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

#include "logi/queue/queue_family.hpp"
#include "logi/command/command_pool.hpp"
#include "logi/command/command_pool_impl.hpp"
#include "logi/device/logical_device.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/queue/queue.hpp"
#include "logi/queue/queue_family_impl.hpp"

namespace logi {

CommandPool QueueFamily::createCommandPool(const vk::CommandPoolCreateFlags& flags,
                                           const ConstVkNextProxy<vk::CommandPoolCreateInfo>& next,
                                           const std::optional<vk::AllocationCallbacks>& allocator) const {
  return CommandPool(object_->createCommandPool(flags, next, allocator));
}

void QueueFamily::destroyCommandPool(const CommandPool& commandPool) const {
  object_->destroyCommandPool(commandPool.id());
}

Queue QueueFamily::getQueue(uint32_t queueIndex) const {
  return Queue(object_->getQueue(queueIndex));
}
Queue QueueFamily::getQueue2(uint32_t queueIndex, const vk::DeviceQueueCreateFlags& flags,
                             const ConstVkNextProxy<vk::DeviceQueueInfo2>& next) const {
  return Queue(object_->getQueue2(queueIndex, flags, next));
}

VulkanInstance QueueFamily::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice QueueFamily::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice QueueFamily::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& QueueFamily::getDispatcher() const {
  return object_->getDispatcher();
}

uint32_t QueueFamily::queueCount() const {
  return object_->queueCount();
}

uint32_t QueueFamily::getIndex() const {
  return object_->getIndex();
}

QueueFamily::operator uint32_t() const {
  return object_->operator uint32_t();
}

}