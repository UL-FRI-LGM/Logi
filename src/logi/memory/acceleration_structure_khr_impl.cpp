/**
 * Project Logi source code
 * Copyright (C) 2020 Aljaz Bogataj
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

#include "logi/memory/acceleration_structure_khr_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

AccelerationStructureKHRImpl::AccelerationStructureKHRImpl(LogicalDeviceImpl& logicalDevice,
                                                           const vk::AccelerationStructureCreateInfoKHR& createInfo,
                                                           const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkAccelerationStructureKHR_ =
    vkDevice.createAccelerationStructureKHR(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

// region Vulkan Definitions

vk::Result AccelerationStructureKHRImpl::
      buildAccelerationStructureKHR(vk::DeferredOperationKHR deferredOperation, 
                                    vk::AccelerationStructureBuildGeometryInfoKHR &info,
                                    const vk::AccelerationStructureBuildRangeInfoKHR *const &pBuildRangeInfo) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  info.dstAccelerationStructure = vkAccelerationStructureKHR_;
  return vkDevice.buildAccelerationStructuresKHR(deferredOperation, info, pBuildRangeInfo, getDispatcher());
}

vk::Result AccelerationStructureKHRImpl::
      copyAccelerationStructureKHR(vk::DeferredOperationKHR deferredOperation, vk::CopyAccelerationStructureInfoKHR &info) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  info.dst = vkAccelerationStructureKHR_;
  return vkDevice.copyAccelerationStructureKHR(deferredOperation, info, getDispatcher());
}

vk::Result AccelerationStructureKHRImpl::
      copyAccelerationStructureToMemoryKHR(vk::DeferredOperationKHR deferredOperation, vk::CopyAccelerationStructureToMemoryInfoKHR &info) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  info.src = vkAccelerationStructureKHR_;
  return vkDevice.copyAccelerationStructureToMemoryKHR(deferredOperation, info, getDispatcher());
}

vk::Result AccelerationStructureKHRImpl::
      copyMemoryToAccelerationStructureKHR(vk::DeferredOperationKHR deferredOperation, vk::CopyMemoryToAccelerationStructureInfoKHR &info) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  info.dst = vkAccelerationStructureKHR_;
  return vkDevice.copyMemoryToAccelerationStructureKHR(deferredOperation, info, getDispatcher());
}

vk::AccelerationStructureBuildSizesInfoKHR 
      AccelerationStructureKHRImpl::getAccelerationStructureBuildSizesKHR(vk::AccelerationStructureBuildTypeKHR buildType, 
                                                                          vk::AccelerationStructureBuildGeometryInfoKHR &buildInfo, 
                                                                          const vk::ArrayProxy<const uint32_t> &maxPrimitiveCounts) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getAccelerationStructureBuildSizesKHR(buildType, buildInfo, maxPrimitiveCounts, getDispatcher());
}

vk::DeviceAddress AccelerationStructureKHRImpl::getAccelerationStructureDeviceAddressKHR() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);

  vk::AccelerationStructureDeviceAddressInfoKHR info;
  info.accelerationStructure = vkAccelerationStructureKHR_;

  return vkDevice.getAccelerationStructureAddressKHR(info, getDispatcher());
}

// endregion

// region Logi Definitions

VulkanInstanceImpl& AccelerationStructureKHRImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& AccelerationStructureKHRImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& AccelerationStructureKHRImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& AccelerationStructureKHRImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void AccelerationStructureKHRImpl::destroy() const {
  logicalDevice_.destroyAccelerationStructureKHR(id());
}

AccelerationStructureKHRImpl::operator const vk::AccelerationStructureKHR&() const {
  return vkAccelerationStructureKHR_;
}

void AccelerationStructureKHRImpl::free() {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkAccelerationStructureKHR_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

// endregion

} // namespace logi
