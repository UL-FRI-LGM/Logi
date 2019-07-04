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

#include "logi/queue/queue_family_impl.hpp"
#include "logi/command/command_pool_impl.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/queue/queue_impl.hpp"

namespace logi {

QueueFamilyImpl::QueueFamilyImpl(LogicalDeviceImpl& logicalDevice, const vk::DeviceQueueCreateInfo& createInfo)
  : logicalDevice_(logicalDevice), queueFamilyIndex_(createInfo.queueFamilyIndex), queueCount_(createInfo.queueCount) {}

const std::shared_ptr<CommandPoolImpl>&
  QueueFamilyImpl::createCommandPool(const vk::CommandPoolCreateFlags& flags,
                                     const ConstVkNextProxy<vk::CommandPoolCreateInfo>& next,
                                     const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<CommandPoolImpl>::createObject(*this, flags, next, allocator);
}

void QueueFamilyImpl::destroyCommandPool(size_t id) {
  VulkanObjectComposite<CommandPoolImpl>::destroyObject(id);
}

const std::shared_ptr<QueueImpl>& QueueFamilyImpl::getQueue(uint32_t queueIndex) {
  vk::Device vk_device = logicalDevice_;
  return VulkanObjectComposite<QueueImpl>::createObject(
    *this, vk_device.getQueue(queueFamilyIndex_, queueIndex, getDispatcher()));
}

const std::shared_ptr<QueueImpl>& QueueFamilyImpl::getQueue2(uint32_t queueIndex,
                                                             const vk::DeviceQueueCreateFlags& flags,
                                                             const ConstVkNextProxy<vk::DeviceQueueInfo2>& next) {
  vk::Device vk_device = logicalDevice_;
  vk::DeviceQueueInfo2 queueInfo(flags, queueFamilyIndex_, queueIndex);
  queueInfo.pNext = next;

  return VulkanObjectComposite<QueueImpl>::createObject(*this, vk_device.getQueue2(queueInfo, getDispatcher()));
}

void QueueFamilyImpl::destroyQueue(size_t id) {
  VulkanObjectComposite<QueueImpl>::destroyObject(id);
}

VulkanInstanceImpl& QueueFamilyImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& QueueFamilyImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& QueueFamilyImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& QueueFamilyImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

uint32_t QueueFamilyImpl::queueCount() const {
  return queueCount_;
}

uint32_t QueueFamilyImpl::getIndex() const {
  return queueFamilyIndex_;
}

QueueFamilyImpl::operator uint32_t() const {
  return queueFamilyIndex_;
}

void QueueFamilyImpl::free() {
  VulkanObjectComposite<QueueImpl>::destroyAllObjects();
  VulkanObjectComposite<CommandPoolImpl>::destroyAllObjects();
  VulkanObject::free();
}

} // namespace logi
