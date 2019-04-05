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

#include "logi/memory/image.hpp"
#include "logi/device/logical_device.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/memory/image_view.hpp"
#include "logi/memory/memory_allocator.hpp"

namespace logi {

ImageView Image::createImageView(const vk::ImageViewCreateInfo& create_info,
                                 const std::optional<vk::AllocationCallbacks>& allocator) {
  return object_->createImageView(create_info, allocator);
}

void Image::destroyImageView(const ImageView& image) {
  object_->destroyImageView(image.id());
}

VulkanInstance Image::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice Image::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice Image::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

MemoryAllocator Image::getMemoryAllocator() const {
  return MemoryAllocator(object_->getMemoryAllocator().shared_from_this());
}

const vk::DispatchLoaderDynamic& Image::getDispatcher() const {
  return object_->getDispatcher();
}

void Image::destroy() const {
  object_->destroy();
}

Image::operator vk::Image() const {
  return object_->operator vk::Image();
}

}