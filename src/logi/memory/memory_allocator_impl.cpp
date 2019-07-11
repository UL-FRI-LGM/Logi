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

#define VMA_IMPLEMENTATION
#include "logi/memory/memory_allocator_impl.hpp"
#include <vk_mem_alloc.h>
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/memory/buffer.hpp"
#include "logi/memory/image.hpp"
#include "logi/memory/vma_buffer_impl.hpp"

namespace logi {

VmaVulkanFunctions dispatcherToVmaFunctions(const vk::DispatchLoaderDynamic& dispatcher) {
  VmaVulkanFunctions vmaFunctions = {};
  vmaFunctions.vkGetPhysicalDeviceProperties = dispatcher.vkGetPhysicalDeviceProperties;
  vmaFunctions.vkGetPhysicalDeviceMemoryProperties = dispatcher.vkGetPhysicalDeviceMemoryProperties;
  vmaFunctions.vkAllocateMemory = dispatcher.vkAllocateMemory;
  vmaFunctions.vkFreeMemory = dispatcher.vkFreeMemory;
  vmaFunctions.vkMapMemory = dispatcher.vkMapMemory;
  vmaFunctions.vkUnmapMemory = dispatcher.vkUnmapMemory;
  vmaFunctions.vkFlushMappedMemoryRanges = dispatcher.vkFlushMappedMemoryRanges;
  vmaFunctions.vkInvalidateMappedMemoryRanges = dispatcher.vkInvalidateMappedMemoryRanges;
  vmaFunctions.vkBindBufferMemory = dispatcher.vkBindBufferMemory;
  vmaFunctions.vkBindImageMemory = dispatcher.vkBindImageMemory;
  vmaFunctions.vkGetBufferMemoryRequirements = dispatcher.vkGetBufferMemoryRequirements;
  vmaFunctions.vkGetImageMemoryRequirements = dispatcher.vkGetImageMemoryRequirements;
  vmaFunctions.vkCreateBuffer = dispatcher.vkCreateBuffer;
  vmaFunctions.vkDestroyBuffer = dispatcher.vkDestroyBuffer;
  vmaFunctions.vkCreateImage = dispatcher.vkCreateImage;
  vmaFunctions.vkDestroyImage = dispatcher.vkDestroyImage;
  vmaFunctions.vkCmdCopyBuffer = dispatcher.vkCmdCopyBuffer;
#if VMA_DEDICATED_ALLOCATION
  vmaFunctions.vkGetBufferMemoryRequirements2KHR = dispatcher.vkGetBufferMemoryRequirements2KHR;
  vmaFunctions.vkGetImageMemoryRequirements2KHR = dispatcher.vkGetImageMemoryRequirements2KHR;
#endif

  return vmaFunctions;
}

MemoryAllocatorImpl::MemoryAllocatorImpl(LogicalDeviceImpl& logicalDevice, vk::DeviceSize preferredLargeHeapBlockSize,
                                         uint32_t frameInUseCount, const std::vector<vk::DeviceSize>& heapSizeLimits,
                                         const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator), vma_(nullptr) {
  VmaAllocatorCreateInfo createInfo = {};
  createInfo.physicalDevice = static_cast<VkPhysicalDevice>(static_cast<vk::PhysicalDevice>(getPhysicalDevice()));
  createInfo.device = static_cast<VkDevice>(static_cast<vk::Device>(getLogicalDevice()));
  createInfo.preferredLargeHeapBlockSize = preferredLargeHeapBlockSize;
  createInfo.frameInUseCount = frameInUseCount;
  createInfo.pAllocationCallbacks =
    allocator ? reinterpret_cast<const VkAllocationCallbacks*>(&allocator.value()) : nullptr;

  VmaVulkanFunctions functions = dispatcherToVmaFunctions(getDispatcher());
  createInfo.pVulkanFunctions = &functions;

  vmaCreateAllocator(&createInfo, &vma_);
}

const std::shared_ptr<VMABufferImpl>&
  MemoryAllocatorImpl::createBuffer(const vk::BufferCreateInfo& bufferCreateInfo,
                                    const VmaAllocationCreateInfo& allocationCreateInfo,
                                    const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<VMABufferImpl>::createObject(*this, bufferCreateInfo, allocationCreateInfo, allocator);
}

void MemoryAllocatorImpl::destroyBuffer(size_t id) {
  VulkanObjectComposite<VMABufferImpl>::destroyObject(id);
}

VulkanInstanceImpl& MemoryAllocatorImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& MemoryAllocatorImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& MemoryAllocatorImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& MemoryAllocatorImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

MemoryAllocatorImpl::operator const VmaAllocator&() const {
  return vma_;
}

void MemoryAllocatorImpl::destroy() const {
  logicalDevice_.destroyMemoryAllocator(id());
}

void MemoryAllocatorImpl::free() {
  VulkanObjectComposite<VMABufferImpl>::destroyAllObjects();
  vmaDestroyAllocator(vma_);
  VulkanObject::free();
}

}