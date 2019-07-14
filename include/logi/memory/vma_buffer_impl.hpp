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

#ifndef LOGI_MEMORY_VMA_BUFFER_IMPL_HPP
#define LOGI_MEMORY_VMA_BUFFER_IMPL_HPP

#include "logi/memory/buffer_impl.hpp"

namespace logi {

class MemoryAllocatorImpl;

class VMABufferImpl : public BufferImpl {
 public:
  VMABufferImpl(MemoryAllocatorImpl& memoryAllocator, const vk::BufferCreateInfo& bufferCreateInfo,
                const VmaAllocationCreateInfo& allocationCreateInfo,
                const std::optional<vk::AllocationCallbacks>& allocator = {});

  void* mapMemory() const;

  void unmapMemory() const;

  size_t size() const;

  void writeToBuffer(const void* data, size_t size, size_t offset = 0) const;

  bool isMappable() const;

  // region Logi Declarations

  MemoryAllocatorImpl& getMemoryAllocator() const;

  void destroy() const override;

 protected:
  void free() override;

  // endregion

 private:
  MemoryAllocatorImpl& memoryAllocator_;
  VmaAllocation allocation_;
  VmaAllocationInfo allocationInfo_;
};

} // namespace logi

#endif // LOGI_MEMORY_VMA_BUFFER_IMPL_HPP
