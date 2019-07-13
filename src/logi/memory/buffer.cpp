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

vk::MemoryRequirements Buffer::getMemoryRequirements() const {
  return object_->getMemoryRequirements();
}

vk::MemoryRequirements2
  Buffer::getBufferMemoryRequirements2(const ConstVkNextProxy<vk::BufferMemoryRequirementsInfo2>& next) const {
  return object_->getBufferMemoryRequirements2(next);
}

vk::MemoryRequirements2KHR
  Buffer::getBufferMemoryRequirements2KHR(const ConstVkNextProxy<vk::BufferMemoryRequirementsInfo2KHR>& next) const {
  return object_->getBufferMemoryRequirements2KHR(next);
}

vk::DeviceAddress Buffer::getDeviceAddressEXT(const ConstVkNextProxy<vk::BufferDeviceAddressInfoEXT>& next) const {
  return object_->getDeviceAddressEXT(next);
}

vk::ResultValueType<void>::type Buffer::bindMemory(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset) const {
  return object_->bindMemory(memory, memoryOffset);
}

vk::ResultValueType<void>::type Buffer::bindMemory2(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset,
                                                    const ConstVkNextProxy<vk::BindBufferMemoryInfo>& next) const {
  return object_->bindMemory2(memory, memoryOffset, next);
}

vk::ResultValueType<void>::type Buffer::bindMemory2KHR(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset,
                                                       const ConstVkNextProxy<vk::BindBufferMemoryInfo>& next) const {
  return object_->bindMemory2KHR(memory, memoryOffset, next);
}

BufferView Buffer::createBufferView(const vk::BufferViewCreateInfo& createInfo,
                                    const std::optional<vk::AllocationCallbacks>& allocator) const {
  return object_->createBufferView(createInfo, allocator);
}

void Buffer::destroyBufferView(const BufferView& bufferView) const {
  object_->destroyBufferView(bufferView.id());
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

const vk::DispatchLoaderDynamic& Buffer::getDispatcher() const {
  return object_->getDispatcher();
}

void Buffer::destroy() const {
  object_->destroy();
}

Buffer::operator const vk::Buffer&() const {
  static vk::Buffer nullHandle(nullptr);
return (object_) ? object_->operator const vk::Buffer&() : nullHandle;
}

}