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

#include "logi/memory/buffer_impl.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/memory/buffer_view.hpp"
#include "logi/memory/memory_allocator_impl.hpp"

namespace logi {

BufferImpl::BufferImpl(MemoryAllocatorImpl& memoryAllocator, const vk::BufferCreateInfo& bufferCreateInfo,
                       const VmaAllocationCreateInfo& allocationCreateInfo)
  : memoryAllocator_(&memoryAllocator), allocation_(nullptr) {
  vmaCreateBuffer(memoryAllocator_->getVmaAllocator(), reinterpret_cast<const VkBufferCreateInfo*>(&bufferCreateInfo),
                  &allocationCreateInfo, reinterpret_cast<VkBuffer*>(&buffer_), &allocation_, nullptr);
}

vk::MemoryRequirements BufferImpl::getMemoryRequirements() const {
  vk::Device vkDevice = getLogicalDevice();
  return vkDevice.getBufferMemoryRequirements(buffer_);
}

BufferView BufferImpl::createBufferView(const vk::BufferViewCreateInfo& createInfo,
                                        const std::optional<vk::AllocationCallbacks>& allocator) {
  return BufferView(VulkanObjectComposite<BufferViewImpl>::createObject(*this, createInfo, allocator));
}

void BufferImpl::destroyBufferView(size_t id) {
  VulkanObjectComposite<BufferViewImpl>::destroyObject(id);
}

VulkanInstanceImpl& BufferImpl::getInstance() const {
  return memoryAllocator_->getInstance();
}

PhysicalDeviceImpl& BufferImpl::getPhysicalDevice() const {
  return memoryAllocator_->getPhysicalDevice();
}

LogicalDeviceImpl& BufferImpl::getLogicalDevice() const {
  return memoryAllocator_->getLogicalDevice();
}

MemoryAllocatorImpl& BufferImpl::getMemoryAllocator() const {
  return *memoryAllocator_;
}

const vk::DispatchLoaderDynamic& BufferImpl::getDispatcher() const {
  return memoryAllocator_->getDispatcher();
}

void BufferImpl::destroy() const {
  memoryAllocator_->destroyBuffer(id());
}

BufferImpl::operator vk::Buffer() const {
  return buffer_;
}

void BufferImpl::free() {
  VulkanObjectComposite<BufferViewImpl>::destroyAllObjects();

  if (memoryAllocator_ != nullptr) {
    vmaDestroyBuffer(getMemoryAllocator().getVmaAllocator(), static_cast<VkBuffer>(buffer_), allocation_);
    memoryAllocator_ = nullptr;
  }

  VulkanObject::free();
}

}