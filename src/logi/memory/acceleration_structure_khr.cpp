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

#include "logi/memory/acceleration_structure_khr.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/memory/acceleration_structure_khr_impl.hpp"

namespace logi {

// region Vulkan Declarations

vk::Result AccelerationStructureKHR::
      buildAccelerationStructureKHR(vk::DeferredOperationKHR deferredOperation, 
                                    vk::AccelerationStructureBuildGeometryInfoKHR &info,
                                    const vk::AccelerationStructureBuildRangeInfoKHR *const &pBuildRangeInfo) const {
  return object_->buildAccelerationStructureKHR(deferredOperation, info, pBuildRangeInfo);
}

vk::Result AccelerationStructureKHR::
      copyAccelerationStructureKHR(vk::DeferredOperationKHR deferredOperation, vk::CopyAccelerationStructureInfoKHR &info) const {
  return object_->copyAccelerationStructureKHR(deferredOperation, info);
}

vk::Result AccelerationStructureKHR::
      copyAccelerationStructureToMemoryKHR(vk::DeferredOperationKHR deferredOperation, vk::CopyAccelerationStructureToMemoryInfoKHR &info) const {
  return object_->copyAccelerationStructureToMemoryKHR(deferredOperation, info);
}

vk::Result AccelerationStructureKHR::
      copyMemoryToAccelerationStructureKHR(vk::DeferredOperationKHR deferredOperation, vk::CopyMemoryToAccelerationStructureInfoKHR &info) const {
  return object_->copyMemoryToAccelerationStructureKHR(deferredOperation, info);
}

vk::AccelerationStructureBuildSizesInfoKHR 
      AccelerationStructureKHR::getAccelerationStructureBuildSizesKHR(vk::AccelerationStructureBuildTypeKHR buildType, 
                                                                      vk::AccelerationStructureBuildGeometryInfoKHR &buildInfo, 
                                                                      const vk::ArrayProxy<const uint32_t> &maxPrimitiveCounts) const {
  return object_->getAccelerationStructureBuildSizesKHR(buildType, buildInfo, maxPrimitiveCounts);
}

vk::DeviceAddress AccelerationStructureKHR::getAccelerationStructureDeviceAddressKHR() const {
  return object_->getAccelerationStructureDeviceAddressKHR();
}

// endregion

// region Logi Definitions

VulkanInstance AccelerationStructureKHR::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice AccelerationStructureKHR::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice AccelerationStructureKHR::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& AccelerationStructureKHR::getDispatcher() const {
  return object_->getDispatcher();
}

void AccelerationStructureKHR::destroy() const {
  if (object_) {
    object_->destroy();
  }
}

AccelerationStructureKHR::operator const vk::AccelerationStructureKHR&() const {
  static vk::AccelerationStructureKHR nullHandle(nullptr);
  return (object_) ? object_->operator const vk::AccelerationStructureKHR&() : nullHandle;
}

// endregion

} // namespace logi
