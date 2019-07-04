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

BufferImpl::BufferImpl(LogicalDeviceImpl& logicalDevice, const vk::BufferCreateInfo& createInfo,
                       const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkBuffer_ = vkDevice.createBuffer(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

vk::MemoryRequirements BufferImpl::getMemoryRequirements() const {
  auto vkDevice = static_cast<vk::Device>(getLogicalDevice());
  return vkDevice.getBufferMemoryRequirements(vkBuffer_, getDispatcher());
}

vk::MemoryRequirements2
  BufferImpl::getBufferMemoryRequirements2(const ConstVkNextProxy<vk::BufferMemoryRequirementsInfo2>& next) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);

  vk::BufferMemoryRequirementsInfo2 memoryRequirementsInfo(vkBuffer_);
  memoryRequirementsInfo.pNext = next;

  return vkDevice.getBufferMemoryRequirements2(memoryRequirementsInfo, getDispatcher());
}

vk::MemoryRequirements2KHR BufferImpl::getBufferMemoryRequirements2KHR(
  const ConstVkNextProxy<vk::BufferMemoryRequirementsInfo2KHR>& next) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);

  vk::BufferMemoryRequirementsInfo2KHR memoryRequirementsInfo(vkBuffer_);
  memoryRequirementsInfo.pNext = next;

  return vkDevice.getBufferMemoryRequirements2KHR(memoryRequirementsInfo, getDispatcher());
}

vk::DeviceAddress BufferImpl::getDeviceAddressEXT(const ConstVkNextProxy<vk::BufferDeviceAddressInfoEXT>& next) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);

  vk::BufferDeviceAddressInfoEXT deviceAddressInfo(vkBuffer_);
  deviceAddressInfo.pNext = next;

  return vkDevice.getBufferAddressEXT(deviceAddressInfo, getDispatcher());
}

vk::ResultValueType<void>::type BufferImpl::bindMemory(const vk::DeviceMemory& memory,
                                                       vk::DeviceSize memoryOffset) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.bindBufferMemory(vkBuffer_, memory, memoryOffset, getDispatcher());
}

vk::ResultValueType<void>::type
  BufferImpl::bindMemory2(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset,
                          const ConstVkNextProxy<vk::BindBufferMemoryInfoKHR>& next) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vk::BindBufferMemoryInfo bufferMemoryInfo(vkBuffer_, memory, memoryOffset);
  bufferMemoryInfo.pNext = next;

  return vkDevice.bindBufferMemory2(bufferMemoryInfo, getDispatcher());
}

vk::ResultValueType<void>::type
  BufferImpl::bindMemory2KHR(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset,
                             const ConstVkNextProxy<vk::BindBufferMemoryInfo>& next) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vk::BindBufferMemoryInfoKHR bufferMemoryInfo(vkBuffer_, memory, memoryOffset);
  bufferMemoryInfo.pNext = next;

  return vkDevice.bindBufferMemory2KHR(bufferMemoryInfo, getDispatcher());
}

BufferView BufferImpl::createBufferView(const vk::BufferViewCreateInfo& createInfo,
                                        const std::optional<vk::AllocationCallbacks>& allocator) {
  return BufferView(VulkanObjectComposite<BufferViewImpl>::createObject(*this, createInfo, allocator));
}

void BufferImpl::destroyBufferView(size_t id) {
  VulkanObjectComposite<BufferViewImpl>::destroyObject(id);
}

VulkanInstanceImpl& BufferImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& BufferImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& BufferImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& BufferImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void BufferImpl::destroy() const {
  logicalDevice_.destroyBuffer(id());
}

BufferImpl::operator const vk::Buffer&() const {
  return vkBuffer_;
}

void BufferImpl::free() {
  VulkanObjectComposite<BufferViewImpl>::destroyAllObjects();
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkBuffer_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());

  VulkanObject::free();
}

}