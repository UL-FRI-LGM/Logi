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

#include "logi/memory/memory_allocator_impl.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/memory/buffer.hpp"
#include "logi/memory/image.hpp"

namespace logi {

VmaVulkanFunctions dispatcherToVmaFunctions(const vk::DispatchLoaderDynamic& dispatcher) {
  VmaVulkanFunctions vma_functions = {};
  vma_functions.vkGetPhysicalDeviceProperties = dispatcher.vkGetPhysicalDeviceProperties;
  vma_functions.vkGetPhysicalDeviceMemoryProperties = dispatcher.vkGetPhysicalDeviceMemoryProperties;
  vma_functions.vkAllocateMemory = dispatcher.vkAllocateMemory;
  vma_functions.vkFreeMemory = dispatcher.vkFreeMemory;
  vma_functions.vkMapMemory = dispatcher.vkMapMemory;
  vma_functions.vkUnmapMemory = dispatcher.vkUnmapMemory;
  vma_functions.vkFlushMappedMemoryRanges = dispatcher.vkFlushMappedMemoryRanges;
  vma_functions.vkInvalidateMappedMemoryRanges = dispatcher.vkInvalidateMappedMemoryRanges;
  vma_functions.vkBindBufferMemory = dispatcher.vkBindBufferMemory;
  vma_functions.vkBindImageMemory = dispatcher.vkBindImageMemory;
  vma_functions.vkGetBufferMemoryRequirements = dispatcher.vkGetBufferMemoryRequirements;
  vma_functions.vkGetImageMemoryRequirements = dispatcher.vkGetImageMemoryRequirements;
  vma_functions.vkCreateBuffer = dispatcher.vkCreateBuffer;
  vma_functions.vkDestroyBuffer = dispatcher.vkDestroyBuffer;
  vma_functions.vkCreateImage = dispatcher.vkCreateImage;
  vma_functions.vkDestroyImage = dispatcher.vkDestroyImage;
  vma_functions.vkCmdCopyBuffer = dispatcher.vkCmdCopyBuffer;
#if VMA_DEDICATED_ALLOCATION
  vma_functions.vkGetBufferMemoryRequirements2KHR = dispatcher.vkGetBufferMemoryRequirements2KHR;
  vma_functions.vkGetImageMemoryRequirements2KHR = dispatcher.vkGetImageMemoryRequirements2KHR;
#endif

  return vma_functions;
}

MemoryAllocatorImpl::MemoryAllocatorImpl(LogicalDeviceImpl& logical_device,
                                         vk::DeviceSize preferred_large_heap_block_size, uint32_t frame_in_use_count,
                                         const std::vector<vk::DeviceSize>& heap_size_limits,
                                         const std::optional<vk::AllocationCallbacks>& allocator)
  : logical_device_(logical_device), allocator_(allocator), vma_(nullptr) {
  VmaAllocatorCreateInfo create_info = {};
  create_info.physicalDevice = static_cast<VkPhysicalDevice>(static_cast<vk::PhysicalDevice>(getPhysicalDevice()));
  create_info.device = static_cast<VkDevice>(static_cast<vk::Device>(getLogicalDevice()));
  create_info.preferredLargeHeapBlockSize = preferred_large_heap_block_size;
  create_info.frameInUseCount = frame_in_use_count;
  create_info.pAllocationCallbacks =
    allocator ? reinterpret_cast<const VkAllocationCallbacks*>(&allocator.value()) : nullptr;

  VmaVulkanFunctions functions = dispatcherToVmaFunctions(getDispatcher());
  create_info.pVulkanFunctions = &functions;

  vmaCreateAllocator(&create_info, &vma_);
}

Image MemoryAllocatorImpl::createImage(const vk::ImageCreateInfo& image_create_info,
                                       const VmaAllocationCreateInfo& allocation_create_info) {
  return Image(VulkanObjectComposite<ImageImpl>::createObject(*this, image_create_info, allocation_create_info));
}

void MemoryAllocatorImpl::destroyImage(size_t id) {
  VulkanObjectComposite<ImageImpl>::destroyObject(id);
}

Buffer MemoryAllocatorImpl::createBuffer(const vk::BufferCreateInfo& buffer_create_info,
                                         const VmaAllocationCreateInfo& allocation_create_info) {
  return Buffer(VulkanObjectComposite<BufferImpl>::createObject(*this, buffer_create_info, allocation_create_info));
}

void MemoryAllocatorImpl::destroyBuffer(size_t id) {
  VulkanObjectComposite<BufferImpl>::destroyObject(id);
}

VulkanInstanceImpl& MemoryAllocatorImpl::getInstance() const {
  return logical_device_.getInstance();
}

PhysicalDeviceImpl& MemoryAllocatorImpl::getPhysicalDevice() const {
  return logical_device_.getPhysicalDevice();
}

LogicalDeviceImpl& MemoryAllocatorImpl::getLogicalDevice() const {
  return logical_device_;
}

const vk::DispatchLoaderDynamic& MemoryAllocatorImpl::getDispatcher() const {
  return logical_device_.getDispatcher();
}

VmaAllocator MemoryAllocatorImpl::getVmaAllocator() const {
  return vma_;
}

void MemoryAllocatorImpl::destroy() const {
  // TODO
}

void MemoryAllocatorImpl::free() {
  VulkanObjectComposite<ImageImpl>::destroyAllObjects();
  VulkanObjectComposite<BufferImpl>::destroyAllObjects();
  vmaDestroyAllocator(vma_);
  VulkanObject::free();
}

}