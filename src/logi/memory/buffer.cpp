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

#include "logi/memory/buffer.hpp"
#include "logi/device/logical_device.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/memory/buffer_view.hpp"
#include "logi/memory/memory_allocator.hpp"

namespace logi {

BufferView Buffer::createBufferView(const vk::BufferViewCreateInfo& create_info,
                                    const std::optional<vk::AllocationCallbacks>& allocator) const {
  return object_->createBufferView(create_info, allocator);
}

void Buffer::destroyBufferView(const BufferView& buffer_view) const {
  object_->destroyBufferView(buffer_view.id());
}

VulkanInstance Buffer::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice Buffer::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice Buffer::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

MemoryAllocator Buffer::getMemoryAllocator() const {
  return MemoryAllocator(object_->getMemoryAllocator().shared_from_this());
}

const vk::DispatchLoaderDynamic& Buffer::getDispatcher() const {
  return object_->getDispatcher();
}

void Buffer::destroy() const {
  object_->destroy();
}

Buffer::operator vk::Buffer() const {
  return object_->operator vk::Buffer();
}

}