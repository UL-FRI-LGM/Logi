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
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/queue/queue.hpp"
#include "logi/queue/queue_impl.hpp"

namespace logi {

QueueFamilyImpl::QueueFamilyImpl(LogicalDeviceImpl& logical_device, const vk::DeviceQueueCreateInfo& queue_create_info)
  : logical_device_(logical_device), queue_family_index_(queue_create_info.queueFamilyIndex),
    queue_count_(queue_create_info.queueCount) {}

Queue QueueFamilyImpl::getQueue(uint32_t queue_index) {
  vk::Device vk_device = logical_device_;
  return Queue(VulkanObjectComposite<QueueImpl>::createObject(
    *this, vk_device.getQueue(queue_family_index_, queue_index, getDispatcher())));
}

Queue QueueFamilyImpl::getQueue2(uint32_t queue_index, const vk::DeviceQueueCreateFlags& flags) {
  vk::Device vk_device = logical_device_;
  return Queue(VulkanObjectComposite<QueueImpl>::createObject(
    *this, vk_device.getQueue2(vk::DeviceQueueInfo2(flags, queue_family_index_, queue_index), getDispatcher())));
}

void QueueFamilyImpl::destroyQueue(size_t id) {
  VulkanObjectComposite<QueueImpl>::destroyObject(id);
}

VulkanInstanceImpl& QueueFamilyImpl::getInstance() const {
  return logical_device_.getInstance();
}

PhysicalDeviceImpl& QueueFamilyImpl::getPhysicalDevice() const {
  return logical_device_.getPhysicalDevice();
}

LogicalDeviceImpl& QueueFamilyImpl::getLogicalDevice() const {
  return logical_device_;
}

const vk::DispatchLoaderDynamic& QueueFamilyImpl::getDispatcher() const {
  return logical_device_.getDispatcher();
}

uint32_t QueueFamilyImpl::queueCount() const {
  return queue_count_;
}

uint32_t QueueFamilyImpl::getIndex() const {
  return queue_family_index_;
}

QueueFamilyImpl::operator uint32_t() const {
  return queue_family_index_;
}

void QueueFamilyImpl::free() {
  VulkanObjectComposite<QueueImpl>::destroyAllObjects();
  VulkanObject::free();
}

} // namespace logi
