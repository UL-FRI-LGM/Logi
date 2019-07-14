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

#include "logi/memory/vma_image_impl.hpp"
#include "logi/memory/memory_allocator_impl.hpp"

namespace logi {

VMAImageImpl::VMAImageImpl(MemoryAllocatorImpl& memoryAllocator, const vk::ImageCreateInfo& bufferCreateInfo,
                           const VmaAllocationCreateInfo& allocationCreateInfo,
                           const std::optional<vk::AllocationCallbacks>& allocator)
  : ImageImpl(memoryAllocator.getLogicalDevice(), bufferCreateInfo, allocator), memoryAllocator_(memoryAllocator),
    allocation_() {
  auto vmaAllocator = static_cast<const VmaAllocator&>(memoryAllocator_);
  vk::MemoryRequirements memoryRequirements = getMemoryRequirements();

  // Allocate memory.
  VkResult result = vmaAllocateMemory(vmaAllocator, reinterpret_cast<const VkMemoryRequirements*>(&memoryRequirements),
                                      &allocationCreateInfo, &allocation_, &allocationInfo_);

  if (result != VK_SUCCESS) {
    throw BadAllocation("Failed to allocate memory for buffer.");
  }

  // Bind memory.
  result = vmaBindImageMemory(vmaAllocator, allocation_, static_cast<VkImage>(vkImage_));

  if (result != VK_SUCCESS) {
    throw BadAllocation("Failed to allocate memory for buffer.");
  }
}

void* VMAImageImpl::mapMemory() const {
  void* mappedMemory;
  vmaMapMemory(static_cast<VmaAllocator>(memoryAllocator_), allocation_, &mappedMemory);

  return mappedMemory;
}

void VMAImageImpl::unmapMemory() const {
  vmaUnmapMemory(static_cast<VmaAllocator>(memoryAllocator_), allocation_);
}

size_t VMAImageImpl::size() const {
  return allocationInfo_.size;
}

void VMAImageImpl::writeToImage(const void* data, size_t offset, size_t size) const {
  std::byte* mappedMemory = reinterpret_cast<std::byte*>(mapMemory()) + offset;
  std::memcpy(mappedMemory, data, size);
}

bool VMAImageImpl::isMappable() const {
  vk::MemoryPropertyFlags memFlags;
  vmaGetMemoryTypeProperties(static_cast<VmaAllocator>(memoryAllocator_), allocationInfo_.memoryType,
                             reinterpret_cast<VkMemoryPropertyFlags*>(&memFlags));

  return static_cast<bool>(memFlags & vk::MemoryPropertyFlagBits::eHostVisible);
}

MemoryAllocatorImpl& VMAImageImpl::getMemoryAllocator() const {
  return memoryAllocator_;
}

void VMAImageImpl::destroy() const {
  memoryAllocator_.destroyImage(id());
}

void VMAImageImpl::free() {
  ImageImpl::free();
  // Free memory.
  vmaFreeMemory(static_cast<VmaAllocator>(memoryAllocator_), allocation_);
}

}