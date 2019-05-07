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

#ifndef LOGI_MEMORY_MEMORY_ALLOCATOR_IMPL_HPP
#define LOGI_MEMORY_MEMORY_ALLOCATOR_IMPL_HPP

#include <vk_mem_alloc.h>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;
class BufferImpl;
class Buffer;
class ImageImpl;
class Image;

class MemoryAllocatorImpl : public VulkanObject<MemoryAllocatorImpl>,
                            public VulkanObjectComposite<BufferImpl>,
                            public VulkanObjectComposite<ImageImpl> {
 public:
  explicit MemoryAllocatorImpl(LogicalDeviceImpl& logical_device, vk::DeviceSize preferred_large_heap_block_size = 0u,
                               uint32_t frame_in_use_count = 0u,
                               const std::vector<vk::DeviceSize>& heap_size_limits = {},
                               const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Vulkan Declarations

  // endregion

  // region Logi Declarations

  Image createImage(const vk::ImageCreateInfo& image_create_info,
                    const VmaAllocationCreateInfo& allocation_create_info);

  void destroyImage(size_t id);

  Buffer createBuffer(const vk::BufferCreateInfo& buffer_create_info,
                      const VmaAllocationCreateInfo& allocation_create_info);

  void destroyBuffer(size_t id);

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  VmaAllocator getVmaAllocator() const;

  void destroy() const;

 protected:
  void free() override;

  // endregion

 private:
  LogicalDeviceImpl& logical_device_;
  std::optional<vk::AllocationCallbacks> allocator_;
  VmaAllocator vma_;
};

} // namespace logi

#endif // LOGI_MEMORY_MEMORY_ALLOCATOR_IMPL_HPP
