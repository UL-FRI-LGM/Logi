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

#ifndef LOGI_MEMORY_BUFFER_IMPL_HPP
#define LOGI_MEMORY_BUFFER_IMPL_HPP

#include <vk_mem_alloc.h>
#include "logi/base/vulkan_object.hpp"
#include "logi/structures/extension.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;
class BufferView;
class BufferViewImpl;

class BufferImpl : public VulkanObject<BufferImpl>, public VulkanObjectComposite<BufferViewImpl> {
 public:
  BufferImpl(LogicalDeviceImpl& logicalDevice, const vk::BufferCreateInfo& bufferCreateInfo,
             const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Vulkan Declarations

  vk::MemoryRequirements getMemoryRequirements() const;

  vk::MemoryRequirements2
    getBufferMemoryRequirements2(const ConstVkNextProxy<vk::BufferMemoryRequirementsInfo2>& next = {}) const;

  vk::MemoryRequirements2KHR
    getBufferMemoryRequirements2KHR(const ConstVkNextProxy<vk::BufferMemoryRequirementsInfo2KHR>& next = {}) const;

  vk::DeviceAddress getDeviceAddressEXT(const ConstVkNextProxy<vk::BufferDeviceAddressInfoEXT>& next = {}) const;

  vk::ResultValueType<void>::type bindMemory(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset) const;

  vk::ResultValueType<void>::type bindMemory2(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset,
                                              const ConstVkNextProxy<vk::BindBufferMemoryInfo>& next) const;

  vk::ResultValueType<void>::type bindMemory2KHR(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset,
                                                 const ConstVkNextProxy<vk::BindBufferMemoryInfoKHR>& next) const;

  // endregion

  // region Logi Declarations

  BufferView createBufferView(const vk::BufferViewCreateInfo& createInfo,
                              const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyBufferView(size_t id);

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::Buffer() const;

 protected:
  void free() override;

  // endregion

 private:
  LogicalDeviceImpl& logicalDevice_;
  vk::Buffer vkBuffer_;
  std::optional<vk::AllocationCallbacks> allocator_;
};

}; // namespace logi

#endif // LOGI_MEMORY_BUFFER_IMPL_HPP
