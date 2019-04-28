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

#include "logi/queue/queue_impl.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/queue/queue_family_impl.hpp"

namespace logi {

QueueImpl::QueueImpl(QueueFamilyImpl& queue_family, vk::Queue vk_queue)
  : queue_family_(queue_family), vk_queue_(vk_queue) {}

vk::ResultValueType<void>::type QueueImpl::submit(const std::vector<vk::SubmitInfo>& submits, vk::Fence fence) const {
  return vk_queue_.submit(submits, fence, getDispatcher());
}

vk::ResultValueType<void>::type QueueImpl::bindSparse(const std::vector<vk::BindSparseInfo>& bind_info,
                                                      vk::Fence fence) const {
  return vk_queue_.bindSparse(bind_info, fence, getDispatcher());
}

vk::ResultValueType<void>::type QueueImpl::waitIdle() const {
  return vk_queue_.waitIdle(getDispatcher());
}

vk::Result QueueImpl::presentKHR(const vk::PresentInfoKHR& present_info) const {
  return vk_queue_.presentKHR(present_info, getDispatcher());
}

void QueueImpl::beginDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& label) const {
  vk_queue_.beginDebugUtilsLabelEXT(label, getDispatcher());
}

void QueueImpl::insertDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& label) const {
  vk_queue_.insertDebugUtilsLabelEXT(label, getDispatcher());
}

void QueueImpl::endDebugUtilsLabelEXT() const {
  vk_queue_.endDebugUtilsLabelEXT(getDispatcher());
}

std::vector<vk::CheckpointDataNV> QueueImpl::getCheckpointDataNV() const {
  return vk_queue_.getCheckpointDataNV(getDispatcher());
}

VulkanInstanceImpl& QueueImpl::getInstance() const {
  return queue_family_.getInstance();
}

PhysicalDeviceImpl& QueueImpl::getPhysicalDevice() const {
  return queue_family_.getPhysicalDevice();
}

LogicalDeviceImpl& QueueImpl::getLogicalDevice() const {
  return queue_family_.getLogicalDevice();
}

QueueFamilyImpl& QueueImpl::getQueueFamily() const {
  return queue_family_;
}

const vk::DispatchLoaderDynamic& QueueImpl::getDispatcher() const {
  return queue_family_.getDispatcher();
}

QueueImpl::operator vk::Queue() const {
  return vk_queue_;
}

void QueueImpl::free() {
  vk_queue_ = nullptr;
  VulkanObject::free();
}
void QueueImpl::destroy() {
  queue_family_.destroyQueue(id());
}

} // namespace logi
