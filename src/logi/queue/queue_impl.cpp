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

QueueImpl::QueueImpl(QueueFamilyImpl& queueFamily, vk::Queue vkQueue) : queueFamily_(queueFamily), vkQueue_(vkQueue) {}

vk::ResultValueType<void>::type QueueImpl::submit(const vk::ArrayProxy<const vk::SubmitInfo>& submits,
                                                  vk::Fence fence) const {
  return vkQueue_.submit(submits, fence, getDispatcher());
}

vk::ResultValueType<void>::type QueueImpl::bindSparse(const vk::ArrayProxy<const vk::BindSparseInfo>& bindInfo,
                                                      vk::Fence fence) const {
  return vkQueue_.bindSparse(bindInfo, fence, getDispatcher());
}

vk::ResultValueType<void>::type QueueImpl::waitIdle() const {
  return vkQueue_.waitIdle(getDispatcher());
}

vk::Result QueueImpl::presentKHR(const vk::PresentInfoKHR& presentInfo) const {
  return vkQueue_.presentKHR(presentInfo, getDispatcher());
}

void QueueImpl::beginDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& label) const {
  vkQueue_.beginDebugUtilsLabelEXT(label, getDispatcher());
}

void QueueImpl::insertDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& label) const {
  vkQueue_.insertDebugUtilsLabelEXT(label, getDispatcher());
}

void QueueImpl::endDebugUtilsLabelEXT() const {
  vkQueue_.endDebugUtilsLabelEXT(getDispatcher());
}

std::vector<vk::CheckpointDataNV> QueueImpl::getCheckpointDataNV() const {
  return vkQueue_.getCheckpointDataNV(getDispatcher());
}

VulkanInstanceImpl& QueueImpl::getInstance() const {
  return queueFamily_.getInstance();
}

PhysicalDeviceImpl& QueueImpl::getPhysicalDevice() const {
  return queueFamily_.getPhysicalDevice();
}

LogicalDeviceImpl& QueueImpl::getLogicalDevice() const {
  return queueFamily_.getLogicalDevice();
}

QueueFamilyImpl& QueueImpl::getQueueFamily() const {
  return queueFamily_;
}

const vk::DispatchLoaderDynamic& QueueImpl::getDispatcher() const {
  return queueFamily_.getDispatcher();
}

QueueImpl::operator const vk::Queue&() const {
  return vkQueue_;
}

void QueueImpl::free() {
  vkQueue_ = nullptr;
  VulkanObject::free();
}
void QueueImpl::destroy() const {
  queueFamily_.destroyQueue(id());
}

} // namespace logi
