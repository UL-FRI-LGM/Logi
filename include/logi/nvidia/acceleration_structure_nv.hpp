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

#include <vulkan/vulkan.hpp>
#include "logi/base/handle.hpp"
#include "logi/nvidia/acceleration_structure_nv_impl.hpp"

namespace logi {

class AccelerationStructureNVImpl;
class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;

class AccelerationStructureNV : public Handle<AccelerationStructureNVImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  template <typename T>
  vk::ResultValueType<void>::type getHandleNV(vk::ArrayProxy<T> data) const;

  vk::MemoryRequirements2KHR
    getMemoryRequirementsNV(vk::AccelerationStructureMemoryRequirementsTypeNV type,
                            const ConstVkNextProxy<vk::AccelerationStructureMemoryRequirementsInfoNV>& next) const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::AccelerationStructureNV() const;

  // endregion
};

template <typename T>
vk::ResultValueType<void>::type AccelerationStructureNV::getHandleNV(vk::ArrayProxy<T> data) const {
  return object_->getHandleNV(data);
}

} // namespace logi

#endif // LOGI_ACCELERATION_STRUCTURE_NV_HPP
