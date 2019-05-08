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

#include "logi/nvidia/acceleration_structure_nv_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

AccelerationStructureNVImpl::AccelerationStructureNVImpl(LogicalDeviceImpl& logicalDevice,
                                                         const vk::AccelerationStructureCreateInfoNV& createInfo,
                                                         const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  vk::Device vkDevice = logicalDevice_;
  vkAccelerationStructureNV_ =
    vkDevice.createAccelerationStructureNV(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

VulkanInstanceImpl& AccelerationStructureNVImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& AccelerationStructureNVImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& AccelerationStructureNVImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& AccelerationStructureNVImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void AccelerationStructureNVImpl::destroy() const {
  // TODO
}

AccelerationStructureNVImpl::operator vk::AccelerationStructureNV() const {
  return vkAccelerationStructureNV_;
}

void AccelerationStructureNVImpl::free() {
  vk::Device vkDevice = logicalDevice_;
  vkDevice.destroy(vkAccelerationStructureNV_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

} // namespace logi
