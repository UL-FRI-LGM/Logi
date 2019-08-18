/**
 * Project Logi source code
 * Copyright (C) 2019 Lana Besevic
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

#include "logi/memory/vma_acceleration_structure_nv_impl.hpp"
#include <vk_mem_alloc.h>
#include "logi/memory/memory_allocator_impl.hpp"

namespace logi {

VMAAccelerationStructureNVImpl::VMAAccelerationStructureNVImpl(
  MemoryAllocatorImpl& memoryAllocator, const vk::AccelerationStructureCreateInfoNV& accelerationStructureCreateInfo,
  const VmaAllocationCreateInfo& allocationCreateInfo, const std::optional<vk::AllocationCallbacks>& allocator)
  : AccelerationStructureNVImpl(memoryAllocator.getLogicalDevice(), accelerationStructureCreateInfo, allocator),
    memoryAllocator_(memoryAllocator), allocation_() {
  auto vmaAllocator = static_cast<const VmaAllocator&>(memoryAllocator_);
  vk::MemoryRequirements2KHR memoryRequirements =
    getMemoryRequirementsNV(vk::AccelerationStructureMemoryRequirementsTypeNV::eObject);

  // Allocate memory.
  VkResult result = vmaAllocateMemory(
    vmaAllocator, reinterpret_cast<const VkMemoryRequirements*>(&memoryRequirements.memoryRequirements),
    &allocationCreateInfo, &allocation_, &allocationInfo_);

  if (result != VK_SUCCESS) {
    throw BadAllocation("Failed to allocate memory for acceleration structure.");
  }

  // Bind memory.
  vk::BindAccelerationStructureMemoryInfoNV bindInfo;
  bindInfo.accelerationStructure = static_cast<const vk::AccelerationStructureNV&>(*this);
  bindInfo.memory = allocationInfo_.deviceMemory;
  bindInfo.memoryOffset = allocationInfo_.offset;

  bindMemory(allocationInfo_.deviceMemory, allocationInfo_.offset);
}

void* VMAAccelerationStructureNVImpl::mapMemory() const {
  void* mappedMemory;
  vmaMapMemory(static_cast<VmaAllocator>(memoryAllocator_), allocation_, &mappedMemory);

  return mappedMemory;
}

void VMAAccelerationStructureNVImpl::unmapMemory() const {
  vmaUnmapMemory(static_cast<VmaAllocator>(memoryAllocator_), allocation_);
}

size_t VMAAccelerationStructureNVImpl::size() const {
  return allocationInfo_.size;
}

bool VMAAccelerationStructureNVImpl::isMappable() const {
  vk::MemoryPropertyFlags memFlags;
  vmaGetMemoryTypeProperties(static_cast<VmaAllocator>(memoryAllocator_), allocationInfo_.memoryType,
                             reinterpret_cast<VkMemoryPropertyFlags*>(&memFlags));

  return static_cast<bool>(memFlags & vk::MemoryPropertyFlagBits::eHostVisible);
}

MemoryAllocatorImpl& VMAAccelerationStructureNVImpl::getMemoryAllocator() const {
  return memoryAllocator_;
}

void VMAAccelerationStructureNVImpl::destroy() const {}

void VMAAccelerationStructureNVImpl::free() {
  AccelerationStructureNVImpl::free();
  // Free memory.
  vmaFreeMemory(static_cast<VmaAllocator>(memoryAllocator_), allocation_);
}

}