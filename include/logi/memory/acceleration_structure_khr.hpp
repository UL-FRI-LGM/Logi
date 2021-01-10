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

#ifndef LOGI_KHRONOS_ACCELERATION_STRUCTURE_KHR_HPP
#define LOGI_KHRONOS_ACCELERATION_STRUCTURE_KHR_HPP

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"
#include "logi/memory/acceleration_structure_khr_impl.hpp"

namespace logi {

class AccelerationStructureKHRImpl;
class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;

class AccelerationStructureKHR : public Handle<AccelerationStructureKHRImpl> {
 public: 
  using Handle::Handle;

  // region Vulkan Declarations

  /**
   * @brief Build this acceleration structure on host. 
   *        Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkBuildAccelerationStructuresKHR.html">vkBuildAccelerationStructuresKHR</a>
   */
  vk::Result buildAccelerationStructureKHR(vk::DeferredOperationKHR deferredOperation, 
                                           vk::AccelerationStructureBuildGeometryInfoKHR &info,
                                           const vk::AccelerationStructureBuildRangeInfoKHR *const &pBuildRangeInfo) const;

  /**
   * @brief Copy to this (dst=this) acceleration structure on host. 
   *        Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCopyAccelerationStructureKHR.html">vkCopyAccelerationStructureKHR</a>
   */
  vk::Result copyAccelerationStructureKHR(vk::DeferredOperationKHR deferredOperation, vk::CopyAccelerationStructureInfoKHR &info) const;

  /**
   * @brief Copy this (src=this) acceleration structure to memory on host. 
   *        Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCopyAccelerationStructureToMemoryKHR.html">vkCopyAccelerationStructureToMemoryKHR</a>
   */
  vk::Result copyAccelerationStructureToMemoryKHR(vk::DeferredOperationKHR deferredOperation, vk::CopyAccelerationStructureToMemoryInfoKHR &info) const;

  /**
   * @brief Copy memory to this (dst=this) acceleration structure on host. 
   *        Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCopyMemoryToAccelerationStructureKHR.html">vkCopyMemoryToAccelerationStructureKHR</a>
   */
  vk::Result copyMemoryToAccelerationStructureKHR(vk::DeferredOperationKHR deferredOperation, vk::CopyMemoryToAccelerationStructureInfoKHR &info) const;

  /**
   * @brief Get build sizes based on description (dstAccelerationStructure is ignored!).  
   *        Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCopyMemoryToAccelerationStructureKHR.html">vkCopyMemoryToAccelerationStructureKHR</a>
   */
  vk::AccelerationStructureBuildSizesInfoKHR getAccelerationStructureBuildSizesKHR(vk::AccelerationStructureBuildTypeKHR buildType, 
                                                                                   vk::AccelerationStructureBuildGeometryInfoKHR &buildInfo, 
                                                                                   const vk::ArrayProxy<const uint32_t> &maxPrimitiveCounts) const;
  
  /**
   * @brief Get build sizes based on description (dstAccelerationStructure is ignored!).  
   *        Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetAccelerationStructureDeviceAddressKHR.html">vkGetAccelerationStructureDeviceAddressKHR</a>
   */
  vk::DeviceAddress getAccelerationStructureDeviceAddressKHR() const;

  /**
   * @brief Get write properties for this acceleration structure.  
   *        Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkWriteAccelerationStructuresPropertiesKHR.html">vkWriteAccelerationStructuresPropertiesKHR</a>
   */
  template <typename T>
  vk::ResultValueType<void>::type writeAccelerationStructuresPropertiesKHR(vk::QueryType queryType, const vk::ArrayProxy<T> &data, size_t stride) const; 

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::AccelerationStructureKHR&() const;

  // endregion
};

template <typename T>
vk::ResultValueType<void>::type
    AccelerationStructureKHR::writeAccelerationStructuresPropertiesKHR(vk::QueryType queryType, const vk::ArrayProxy<T> &data, size_t stride) const {
  return object_->writeAccelerationStructuresPropertiesKHR(queryType, data, stride);
}

} // namespace logi

#endif // LOGI_KHRONOS_ACCELERATION_STRUCTURE_KHR_HPP