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

#include "logi/memory/vma_acceleration_structure_nv.hpp"
#include "logi/memory/memory_allocator.hpp"
#include "logi/memory/vma_acceleration_structure_nv_impl.hpp"

namespace logi {
VMAAccelerationStructureNV::VMAAccelerationStructureNV(
  const std::shared_ptr<VMAAccelerationStructureNVImpl>& VMAAccelerationStructureNVImpl) {}

VMAAccelerationStructureNV::VMAAccelerationStructureNV(const AccelerationStructureNV& accelerationStructure) {}

void* VMAAccelerationStructureNV::mapMemory() const {
  return static_cast<VMAAccelerationStructureNVImpl*>(object_.get())->mapMemory();
}

void VMAAccelerationStructureNV::unmapMemory() const {
  static_cast<VMAAccelerationStructureNVImpl*>(object_.get())->unmapMemory();
}

size_t VMAAccelerationStructureNV::size() const {
  return static_cast<VMAAccelerationStructureNVImpl*>(object_.get())->size();
}

bool VMAAccelerationStructureNV::isMappable() const {
  return static_cast<VMAAccelerationStructureNVImpl*>(object_.get())->isMappable();
}

MemoryAllocator VMAAccelerationStructureNV::getMemoryAllocator() const {
  return MemoryAllocator(
    static_cast<VMAAccelerationStructureNVImpl*>(object_.get())->getMemoryAllocator().shared_from_this());
}
} // namespace logi
