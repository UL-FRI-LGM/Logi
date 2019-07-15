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

#include "logi/memory/vma_image.hpp"
#include "logi/memory/memory_allocator.hpp"
#include "logi/memory/vma_image_impl.hpp"

namespace logi {

VMAImage::VMAImage(const std::shared_ptr<VMAImageImpl>& vmaImageImpl)
  : Image(std::static_pointer_cast<ImageImpl>(vmaImageImpl)) {}

VMAImage::VMAImage(const Image& image) : Image(image) {
  if (dynamic_cast<VMAImageImpl*>(object_.get()) == nullptr) {
    object_.reset();
  }
}

void* VMAImage::mapMemory() const {
  return static_cast<VMAImageImpl*>(object_.get())->mapMemory();
}

void VMAImage::unmapMemory() const {
  static_cast<VMAImageImpl*>(object_.get())->unmapMemory();
}

size_t VMAImage::size() const {
  return static_cast<VMAImageImpl*>(object_.get())->size();
}

void VMAImage::writeToImage(const void* data, size_t size, size_t offset) const {
  static_cast<VMAImageImpl*>(object_.get())->writeToImage(data, size, offset);
}

bool VMAImage::isMappable() const {
  return static_cast<VMAImageImpl*>(object_.get())->isMappable();
}

MemoryAllocator VMAImage::getMemoryAllocator() const {
  return MemoryAllocator(static_cast<VMAImageImpl*>(object_.get())->getMemoryAllocator().shared_from_this());
}

}