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

#ifndef LOGI_NVIDIA_ACCELERATION_STRUCTURE_NV_IMPL_HPP
#define LOGI_NVIDIA_ACCELERATION_STRUCTURE_NV_IMPL_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/structures/extension.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;

class AccelerationStructureNVImpl : public VulkanObject<AccelerationStructureNVImpl> {
 public:
  AccelerationStructureNVImpl(LogicalDeviceImpl& logicalDevice, const vk::AccelerationStructureCreateInfoNV& createInfo,
                              const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Vulkan Declarations

  template <typename T>
  vk::ResultValueType<void>::type getHandleNV(vk::ArrayProxy<T> data) const;

  vk::MemoryRequirements2KHR
    getMemoryRequirementsNV(vk::AccelerationStructureMemoryRequirementsTypeNV type,
                            const ConstVkNextProxy<vk::AccelerationStructureMemoryRequirementsInfoNV>& next) const;

  vk::ResultValueType<void>::type
    bindMemory(vk::DeviceMemory memory, vk::DeviceSize memoryOffset,
               const vk::ArrayProxy<uint32_t>& deviceIndices = nullptr,
               const ConstVkNextProxy<vk::BindAccelerationStructureMemoryInfoNV>& next = {}) const;

  // endregion

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::AccelerationStructureNV() const;

 protected:
  void free() override;

  // endregion

 private:
  LogicalDeviceImpl& logicalDevice_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::AccelerationStructureNV vkAccelerationStructureNV_;
};

template <typename T>
vk::ResultValueType<void>::type AccelerationStructureNVImpl::getHandleNV(vk::ArrayProxy<T> data) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getAccelerationStructureHandleNV(vkAccelerationStructureNV_, data, getDispatcher());
}

} // namespace logi

#endif // LOGI_NVIDIA_ACCELERATION_STRUCTURE_NV_IMPL_HPP
