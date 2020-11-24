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

#ifndef LOGI_VMA_ACCELERATION_STRUCTURE_NV_H
#define LOGI_VMA_ACCELERATION_STRUCTURE_NV_H

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"
#include "logi/memory/acceleration_structure_nv.hpp"

namespace logi {

class VMAAccelerationStructureNVImpl;
class MemoryAllocator;

/**
 * @brief VMA allocation of acceleration structure 
 * @see <a href="https://gpuopen-librariesandsdks.github.io/VulkanMemoryAllocator/html/index.html">VMA(Vulkan Memory Allocator)</a> 
 */
class VMAAccelerationStructureNV : public AccelerationStructureNV {
 public:
  explicit VMAAccelerationStructureNV() = default;

  explicit VMAAccelerationStructureNV(
    const std::shared_ptr<VMAAccelerationStructureNVImpl>& vmaAccelerationStructureNVImpl);

  explicit VMAAccelerationStructureNV(const AccelerationStructureNV& accelerationStructure);

  void* mapMemory() const;

  void unmapMemory() const;

  size_t size() const;

  bool isMappable() const;

  MemoryAllocator getMemoryAllocator() const;
};

} // namespace logi

#endif // LOGI_VMA_ACCELERATION_STRUCTURE_NV_H
