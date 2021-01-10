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

#ifndef LOGI_KHRONOS_ACCELERATION_STRUCTURE_KHR_IMPL_HPP
#define LOGI_KHRONOS_ACCELERATION_STRUCTURE_KHR_IMPL_HPP

#include "logi/base/common.hpp"
#include "logi/base/vulkan_object.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/structures/extension.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;

class AccelerationStructureKHRImpl : public VulkanObject,
                                     public std::enable_shared_from_this<AccelerationStructureKHRImpl> {
 public:
  AccelerationStructureKHRImpl(LogicalDeviceImpl& LogicalDevice, const vk::AccelerationStructureCreateInfoKHR& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Vulkan Declarations

  vk::Result buildAccelerationStructureKHR(vk::DeferredOperationKHR deferredOperation, 
                                           vk::AccelerationStructureBuildGeometryInfoKHR &info,
                                           const vk::AccelerationStructureBuildRangeInfoKHR *const &pBuildRangeInfo) const;

  vk::Result copyAccelerationStructureKHR(vk::DeferredOperationKHR deferredOperation, vk::CopyAccelerationStructureInfoKHR &info) const;  

  vk::Result copyAccelerationStructureToMemoryKHR(vk::DeferredOperationKHR deferredOperation, vk::CopyAccelerationStructureToMemoryInfoKHR &info) const;

  vk::Result copyMemoryToAccelerationStructureKHR(vk::DeferredOperationKHR deferredOperation, vk::CopyMemoryToAccelerationStructureInfoKHR &info) const;

  vk::AccelerationStructureBuildSizesInfoKHR getAccelerationStructureBuildSizesKHR(vk::AccelerationStructureBuildTypeKHR buildType, 
                                                                                   vk::AccelerationStructureBuildGeometryInfoKHR &buildInfo, 
                                                                                   const vk::ArrayProxy<const uint32_t> &maxPrimitiveCounts) const;  

  vk::DeviceAddress getAccelerationStructureDeviceAddressKHR() const;

  template <typename T>
  vk::ResultValueType<void>::type writeAccelerationStructuresPropertiesKHR(vk::QueryType queryType, const vk::ArrayProxy<T> &data, size_t stride) const; 
                                                                                 
  // endregion

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  virtual void destroy() const;

  operator const vk::AccelerationStructureKHR&() const;

 protected:
  void free() override;

  // endregion

 private:
  LogicalDeviceImpl& logicalDevice_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::AccelerationStructureKHR vkAccelerationStructureKHR_;
};

template <typename T>
vk::ResultValueType<void>::type
    AccelerationStructureKHRImpl::writeAccelerationStructuresPropertiesKHR(vk::QueryType queryType, const vk::ArrayProxy<T> &data, size_t stride) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.writeAccelerationStructuresPropertiesKHR(vkAccelerationStructureKHR_, queryType, data, stride, getDispatcher());
}

} // namespace logi

#endif // LOGI_NVIDIA_ACCELERATION_STRUCTURE_NV_IMPL_HPP
