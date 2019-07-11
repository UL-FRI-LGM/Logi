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

#include "logi/base/vulkan_object.hpp"

namespace logi {

size_t generateUniqueId() {
  static std::atomic<size_t> idGenerator = 0u;

  return idGenerator++;
}

VulkanObject::VulkanObject(bool valid) : id_(generateUniqueId()), valid_(valid) {}

void VulkanObject::free() {
  valid_.store(false, std::memory_order::memory_order_relaxed);
}

size_t VulkanObject::id() const {
  return id_;
}

VulkanObject::operator bool() const {
  return valid_.load(std::memory_order::memory_order_relaxed);
}

bool VulkanObject::operator!() const {
  return !valid_.load(std::memory_order::memory_order_relaxed);
}

bool VulkanObject::valid() const {
  return valid_.load(std::memory_order::memory_order_relaxed);
}

} // namespace logi
