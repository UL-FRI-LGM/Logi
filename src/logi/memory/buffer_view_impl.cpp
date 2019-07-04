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

#include "logi/memory/buffer_view_impl.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/memory/buffer_impl.hpp"
#include "logi/memory/memory_allocator_impl.hpp"

namespace logi {

BufferViewImpl::BufferViewImpl(BufferImpl& buffer, vk::BufferViewCreateInfo createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : buffer_(buffer), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(getLogicalDevice());
  createInfo.buffer = buffer_;
  vkBufferView_ = vkDevice.createBufferView(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

VulkanInstanceImpl& BufferViewImpl::getInstance() const {
  return buffer_.getInstance();
}

PhysicalDeviceImpl& BufferViewImpl::getPhysicalDevice() const {
  return buffer_.getPhysicalDevice();
}

LogicalDeviceImpl& BufferViewImpl::getLogicalDevice() const {
  return buffer_.getLogicalDevice();
}

BufferImpl& BufferViewImpl::getBuffer() const {
  return buffer_;
}

const vk::DispatchLoaderDynamic& BufferViewImpl::getDispatcher() const {
  return buffer_.getDispatcher();
}

void BufferViewImpl::destroy() const {
  buffer_.destroyBufferView(id());
}

BufferViewImpl::operator const vk::BufferView&() const {
  return vkBufferView_;
}

void BufferViewImpl::free() {
  vk::Device vk_device = getLogicalDevice();
  vk_device.destroy(vkBufferView_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

}