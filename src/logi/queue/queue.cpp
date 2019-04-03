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

#include "logi/queue/queue.hpp"
#include "logi/device/logical_device.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/queue/queue_family.hpp"
#include "logi/queue/queue_family_impl.hpp"

namespace logi {

// region Vulkan Definitions

vk::ResultValueType<void>::type Queue::submit(const std::vector<vk::SubmitInfo>& submits, vk::Fence fence) const {
  return object_->submit(submits, fence);
}

vk::ResultValueType<void>::type Queue::bindSparse(const std::vector<vk::BindSparseInfo>& bind_info,
                                                  vk::Fence fence) const {
  return object_->bindSparse(bind_info, fence);
}

vk::ResultValueType<void>::type Queue::waitIdle() const {
  return object_->waitIdle();
}

vk::Result Queue::presentKHR(const vk::PresentInfoKHR& present_info) const {
  return object_->presentKHR(present_info);
}

void Queue::beginDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& label) const {
  return object_->beginDebugUtilsLabelEXT(label);
}

void Queue::insertDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& label) const {
  return object_->insertDebugUtilsLabelEXT(label);
}

void Queue::endDebugUtilsLabelEXT() const {
  return object_->endDebugUtilsLabelEXT();
}

std::vector<vk::CheckpointDataNV> Queue::getCheckpointDataNV() const {
  return object_->getCheckpointDataNV();
}

// endregion

// region Logi Definitions

VulkanInstance Queue::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice Queue::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice Queue::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

QueueFamily Queue::getQueueFamily() const {
  return QueueFamily(object_->getQueueFamily().shared_from_this());
}

const vk::DispatchLoaderDynamic& Queue::getDispatcher() const {
  return object_->getDispatcher();
}

Queue::operator vk::Queue() const {
  return object_->operator vk::Queue();
}

void Queue::destroy() {
  object_->getQueueFamily().destroyQueue(id());
}

// endregion

}