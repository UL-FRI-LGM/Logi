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

#include "logi/device/logical_device_impl.hpp"
#include "logi/command/command_pool_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/program/descriptor_set_layout_impl.hpp"
#include "logi/program/pipeline_cache_impl.hpp"
#include "logi/program/pipeline_layout_impl.hpp"
#include "logi/program/shader_module_impl.hpp"
#include "logi/query/query_pool_impl.hpp"
#include "logi/queue/queue_family_impl.hpp"
#include "logi/synchronization/event_impl.hpp"
#include "logi/synchronization/fence_impl.hpp"
#include "logi/synchronization/semaphore_impl.hpp"

namespace logi {

LogicalDeviceImpl::LogicalDeviceImpl(PhysicalDeviceImpl& physicalDevice, const vk::DeviceCreateInfo& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator)
  : physicalDevice_(physicalDevice), allocator_(allocator) {
  vk::Instance vkInstance = physicalDevice.getInstance();
  vk::PhysicalDevice vkPhysicalDevice = physicalDevice;
  const vk::DispatchLoaderDynamic& instanceDispatcher = physicalDevice.getDispatcher();

  vkDevice_ = vkPhysicalDevice.createDevice(createInfo, allocator_ ? &allocator.value() : nullptr, instanceDispatcher);

  // Initialize device dispatcher.
  dispatcher_ = vk::DispatchLoaderDynamic(static_cast<VkInstance>(vkInstance), instanceDispatcher.vkGetInstanceProcAddr,
                                          static_cast<VkDevice>(vkDevice_), instanceDispatcher.vkGetDeviceProcAddr);

  // Initialize queue families.
  for (uint32_t i = 0u; i < createInfo.queueCreateInfoCount; i++) {
    VulkanObjectComposite<QueueFamilyImpl>::createObject(*this, createInfo.pQueueCreateInfos[i]);
  }
}

const std::shared_ptr<ShaderModuleImpl>&
  LogicalDeviceImpl::createShaderModule(const vk::ShaderModuleCreateInfo& createInfo,
                                        const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<ShaderModuleImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyShaderModule(size_t id) {
  VulkanObjectComposite<ShaderModuleImpl>::destroyObject(id);
}

const std::shared_ptr<PipelineCacheImpl>&
  LogicalDeviceImpl::createPipelineCache(const vk::PipelineCacheCreateInfo& createInfo,
                                         const std::optional<AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<PipelineCacheImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyPipelineCache(size_t id) {
  VulkanObjectComposite<PipelineCacheImpl>::destroyObject(id);
}

const std::shared_ptr<DescriptorSetLayoutImpl>&
  LogicalDeviceImpl::createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& createInfo,
                                               const std::optional<AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<DescriptorSetLayoutImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyDescriptorSetLayout(size_t id) {
  VulkanObjectComposite<DescriptorSetLayoutImpl>::destroyObject(id);
}

const std::shared_ptr<PipelineLayoutImpl>&
  LogicalDeviceImpl::createPipelineLayout(const vk::PipelineLayoutCreateInfo& createInfo,
                                          const std::optional<AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<PipelineLayoutImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyPipelineLayout(size_t id) {
  VulkanObjectComposite<PipelineLayoutImpl>::destroyObject(id);
}

const std::shared_ptr<CommandPoolImpl>&
  LogicalDeviceImpl::createCommandPool(const vk::CommandPoolCreateInfo& createInfo,
                                       const std::optional<AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<CommandPoolImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyCommandPool(size_t id) {
  VulkanObjectComposite<CommandPoolImpl>::destroyObject(id);
}

const std::shared_ptr<QueryPoolImpl>&
  LogicalDeviceImpl::createQueryPool(const vk::QueryPoolCreateInfo& createInfo,
                                     const std::optional<AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<QueryPoolImpl>::createObject(*this, createInfo, allocator);
}
void LogicalDeviceImpl::destroyQueryPool(size_t id) {
  VulkanObjectComposite<QueryPoolImpl>::destroyObject(id);
}

const std::shared_ptr<EventImpl>&
  LogicalDeviceImpl::createEvent(const vk::EventCreateInfo& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<EventImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyEvent(size_t id) {
  VulkanObjectComposite<EventImpl>::destroyObject(id);
}

const std::shared_ptr<FenceImpl>&
  LogicalDeviceImpl::createFence(const vk::FenceCreateInfo& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<FenceImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyFence(size_t id) {
  VulkanObjectComposite<FenceImpl>::destroyObject(id);
}

const std::shared_ptr<SemaphoreImpl>&
  LogicalDeviceImpl::createSemaphore(const vk::SemaphoreCreateInfo& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SemaphoreImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroySemaphore(size_t id) {
  VulkanObjectComposite<SemaphoreImpl>::destroyObject(id);
}

std::vector<std::shared_ptr<QueueFamilyImpl>> LogicalDeviceImpl::enumerateQueueFamilies() const {
  std::unordered_map<size_t, std::shared_ptr<QueueFamilyImpl>> familiesMap =
    VulkanObjectComposite<QueueFamilyImpl>::getHandles();

  std::vector<std::shared_ptr<QueueFamilyImpl>> queueFamilies;
  queueFamilies.reserve(familiesMap.size());

  for (const auto& entry : familiesMap) {
    queueFamilies.emplace_back(entry.second);
  }

  return queueFamilies;
}

VulkanInstanceImpl& LogicalDeviceImpl::getInstance() const {
  return physicalDevice_.getInstance();
}

PhysicalDeviceImpl& LogicalDeviceImpl::getPhysicalDevice() const {
  return physicalDevice_;
}

const vk::DispatchLoaderDynamic& LogicalDeviceImpl::getDispatcher() const {
  return dispatcher_;
}

void LogicalDeviceImpl::destroy() const {
  physicalDevice_.destroyLogicalDevice(id());
}

LogicalDeviceImpl::operator const vk::Device() const {
  return vkDevice_;
}

void LogicalDeviceImpl::free() {
  // TODO (Destroy composited objects)
  VulkanObjectComposite<QueueFamilyImpl>::destroyAllObjects();
  vkDevice_.destroy(allocator_ ? &allocator_.value() : nullptr, dispatcher_);
  vkDevice_ = nullptr;
  VulkanObject::free();
}

} // namespace logi
