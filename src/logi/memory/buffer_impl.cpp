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

BufferImpl::BufferImpl(MemoryAllocatorImpl& memory_allocator, const vk::BufferCreateInfo& buffer_create_info,
                       const VmaAllocationCreateInfo& allocation_create_info)
  : memory_allocator_(&memory_allocator), allocation_(nullptr) {
  vmaCreateBuffer(memory_allocator_->getVmaAllocator(),
                  reinterpret_cast<const VkBufferCreateInfo*>(&buffer_create_info), &allocation_create_info,
                  reinterpret_cast<VkBuffer*>(&buffer_), &allocation_, nullptr);
}

BufferView BufferImpl::createBufferView(const vk::BufferViewCreateInfo& create_info,
                                        const std::optional<vk::AllocationCallbacks>& allocator) {
  return BufferView(VulkanObjectComposite<BufferViewImpl>::createObject(*this, create_info, allocator));
}

void BufferImpl::destroyBufferView(size_t id) {
  VulkanObjectComposite<BufferViewImpl>::destroyObject(id);
}

VulkanInstanceImpl& BufferImpl::getInstance() const {
  return memory_allocator_->getInstance();
}

PhysicalDeviceImpl& BufferImpl::getPhysicalDevice() const {
  return memory_allocator_->getPhysicalDevice();
}

LogicalDeviceImpl& BufferImpl::getLogicalDevice() const {
  return memory_allocator_->getLogicalDevice();
}

MemoryAllocatorImpl& BufferImpl::getMemoryAllocator() const {
  return *memory_allocator_;
}

const vk::DispatchLoaderDynamic& BufferImpl::getDispatcher() const {
  return memory_allocator_->getDispatcher();
}

void BufferImpl::destroy() const {
  memory_allocator_->destroyBuffer(id());
}

BufferImpl::operator vk::Buffer() const {
  return buffer_;
}

void BufferImpl::free() {
  VulkanObjectComposite<BufferViewImpl>::destroyAllObjects();

  if (memory_allocator_ != nullptr) {
    vmaDestroyBuffer(getMemoryAllocator().getVmaAllocator(), static_cast<VkBuffer>(buffer_), allocation_);
    memory_allocator_ = nullptr;
  }

  VulkanObject::free();
}

}