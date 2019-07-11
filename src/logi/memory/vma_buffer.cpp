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

#include "logi/memory/vma_buffer.hpp"
#include "logi/memory/memory_allocator.hpp"
#include "logi/memory/vma_buffer_impl.hpp"

namespace logi {

VMABuffer::VMABuffer(const std::shared_ptr<VMABufferImpl>& vmaBufferImpl)
  : Buffer(std::static_pointer_cast<BufferImpl>(vmaBufferImpl)) {}

VMABuffer::VMABuffer(const Buffer& buffer) : Buffer(buffer) {
  if (dynamic_cast<VMABufferImpl*>(object_.get()) == nullptr) {
    object_.reset();
  }
}

void* VMABuffer::mapMemory() const {
  return static_cast<VMABufferImpl*>(object_.get())->mapMemory();
}

void VMABuffer::unmapMemory() const {
  static_cast<VMABufferImpl*>(object_.get())->unmapMemory();
}

size_t VMABuffer::size() const {
  return static_cast<VMABufferImpl*>(object_.get())->size();
}

void VMABuffer::writeToBuffer(const void* data, size_t offset, size_t size) const {
  static_cast<VMABufferImpl*>(object_.get())->writeToBuffer(data, offset, size);
}

bool VMABuffer::isMappable() const {
  return static_cast<VMABufferImpl*>(object_.get())->isMappable();
}

MemoryAllocator VMABuffer::getMemoryAllocator() const {
  return MemoryAllocator(static_cast<VMABufferImpl*>(object_.get())->getMemoryAllocator().shared_from_this());
}

}