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

#include "logi/surface/surface_khr.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/surface/surface_khr_impl.hpp"

namespace logi {

VulkanInstance SurfaceKHR::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

const vk::DispatchLoaderDynamic& SurfaceKHR::getDispatcher() const {
  return object_->getDispatcher();
}

void SurfaceKHR::destroy() const {
  if (object_) {
    object_->destroy();
  }
}

SurfaceKHR::operator const vk::SurfaceKHR&() const {
  static vk::SurfaceKHR nullHandle(nullptr);
  return (object_) ? object_->operator const vk::SurfaceKHR&() : nullHandle;
}

} // namespace logi