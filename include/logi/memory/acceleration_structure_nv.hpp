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

#ifndef LOGI_NVIDIA_ACCELERATION_STRUCTURE_NV_HPP
#define LOGI_NVIDIA_ACCELERATION_STRUCTURE_NV_HPP

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"
#include "logi/memory/acceleration_structure_nv_impl.hpp"

namespace logi {

class AccelerationStructureNVImpl;
class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;

class AccelerationStructureNV : public Handle<AccelerationStructureNVImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetAccelerationStructureHandleNV.html">vkGetAccelerationStructureHandleNV</a>
   */
  template <typename T>
  vk::ResultValueType<void>::type getHandleNV(vk::ArrayProxy<T> data) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetAccelerationStructureMemoryRequirementsNV.html">vkGetAccelerationStructureMemoryRequirementsNV</a>
   */
  vk::MemoryRequirements2KHR
    getMemoryRequirementsNV(vk::AccelerationStructureMemoryRequirementsTypeNV type,
                            const ConstVkNextProxy<vk::AccelerationStructureMemoryRequirementsInfoNV>& next = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkBindAccelerationStructureMemoryNV.html">vkBindAccelerationStructureMemoryNV</a>
   */
  vk::ResultValueType<void>::type
    bindMemory(vk::DeviceMemory memory, vk::DeviceSize memoryOffset,
               const vk::ArrayProxy<uint32_t>& deviceIndices = nullptr,
               const ConstVkNextProxy<vk::BindAccelerationStructureMemoryInfoNV>& next = {}) const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::AccelerationStructureNV&() const;

  // endregion
};

template <typename T>
vk::ResultValueType<void>::type AccelerationStructureNV::getHandleNV(vk::ArrayProxy<T> data) const {
  return object_->getHandleNV(data);
}

} // namespace logi

#endif // LOGI_ACCELERATION_STRUCTURE_NV_HPP
