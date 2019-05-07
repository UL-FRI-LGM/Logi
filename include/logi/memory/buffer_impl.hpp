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

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;
class MemoryAllocatorImpl;
class BufferView;
class BufferViewImpl;

class BufferImpl : public VulkanObject<BufferImpl>, public VulkanObjectComposite<BufferViewImpl> {
 public:
  BufferImpl(MemoryAllocatorImpl& memory_allocator, const vk::BufferCreateInfo& buffer_create_info,
             const VmaAllocationCreateInfo& allocation_create_info);

  // region Vulkan Declarations
  // endregion

  // region Logi Declarations

  BufferView createBufferView(const vk::BufferViewCreateInfo& create_info,
                              const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyBufferView(size_t id);

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  MemoryAllocatorImpl& getMemoryAllocator() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::Buffer() const;

 protected:
  void free() override;

  // endregion

 private:
  MemoryAllocatorImpl* memory_allocator_;
  vk::Buffer buffer_;
  VmaAllocation allocation_;
};

}; // namespace logi

#endif // LOGI_MEMORY_BUFFER_IMPL_HPP
