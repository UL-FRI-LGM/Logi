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

#include "logi/memory/device_memory.hpp"
#include "logi/device/logical_device.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/memory/device_memory_impl.hpp"

namespace logi {

// region Vulkan Definitions

vk::DeviceSize DeviceMemory::getCommitment() const {
  return object_->getCommitment();
}

vk::ResultValueType<void*>::type DeviceMemory::mapMemory(vk::DeviceSize offset, vk::DeviceSize size,
                                                         const vk::MemoryMapFlags& flags) const {
  return object_->mapMemory(offset, size, flags);
}

void DeviceMemory::unmapMemory() const {
  object_->unmapMemory();
}

// endregion

// region Vulkan Definitions

VulkanInstance DeviceMemory::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice DeviceMemory::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice DeviceMemory::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& DeviceMemory::getDispatcher() const {
  return object_->getDispatcher();
}

void DeviceMemory::destroy() const {
  if (object_) {
    object_->destroy();
  }
}

DeviceMemory::operator const vk::DeviceMemory&() const {
  static vk::DeviceMemory nullHandle(nullptr);
  return (object_) ? object_->operator const vk::DeviceMemory&() : nullHandle;
}

// endregion
} // namespace logi
