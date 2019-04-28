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

#include "logi/surface/surface_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"

namespace logi {

SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::SurfaceKHR& vk_surface,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator), vk_surface_(vk_surface) {}

VulkanInstanceImpl& SurfaceKHRImpl::getInstance() const {
  return instance_;
}

const vk::DispatchLoaderDynamic& SurfaceKHRImpl::getDispatcher() const {
  return instance_.getDispatcher();
}

void SurfaceKHRImpl::destroy() const {
  instance_.destroySurfaceKHR(id());
}

SurfaceKHRImpl::operator vk::SurfaceKHR() const {
  return vk_surface_;
}

void SurfaceKHRImpl::free() {
  vk::Instance vk_instance = instance_;
  vk_instance.destroySurfaceKHR(vk_surface_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  vk_surface_ = nullptr;
  VulkanObject::free();
}

} // namespace logi