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

#include "logi/memory/buffer_view.hpp"
#include "logi/device/logical_device.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/memory/buffer.hpp"
#include "logi/memory/memory_allocator.hpp"

namespace logi {

VulkanInstance BufferView::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice BufferView::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice BufferView::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

Buffer BufferView::getBuffer() const {
  return Buffer(object_->getBuffer().shared_from_this());
}

const vk::DispatchLoaderDynamic& BufferView::getDispatcher() const {
  return object_->getDispatcher();
}

void BufferView::destroy() const {
  object_->destroy();
}

BufferView::operator vk::BufferView() const {
  return object_->operator vk::BufferView();
}

} // namespace logi
