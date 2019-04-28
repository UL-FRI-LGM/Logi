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

#include "logi/memory/image_view_impl.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/memory/image_impl.hpp"
#include "logi/memory/memory_allocator_impl.hpp"

namespace logi {

ImageViewImpl::ImageViewImpl(logi::ImageImpl& image, vk::ImageViewCreateInfo create_info,
                             const std::optional<vk::AllocationCallbacks>& allocator)
  : image_(image), allocator_(allocator) {
  vk::Device vk_device = getLogicalDevice();
  create_info.image = image_;
  vk_image_view_ = vk_device.createImageView(create_info, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

VulkanInstanceImpl& logi::ImageViewImpl::getInstance() const {
  return image_.getInstance();
}

PhysicalDeviceImpl& logi::ImageViewImpl::getPhysicalDevice() const {
  return image_.getPhysicalDevice();
}

LogicalDeviceImpl& logi::ImageViewImpl::getLogicalDevice() const {
  return image_.getLogicalDevice();
}

MemoryAllocatorImpl& logi::ImageViewImpl::getMemoryAllocator() const {
  return image_.getMemoryAllocator();
}

ImageImpl& logi::ImageViewImpl::getImage() const {
  return image_;
}

const vk::DispatchLoaderDynamic& ImageViewImpl::getDispatcher() const {
  return image_.getDispatcher();
}

void ImageViewImpl::destroy() const {
  image_.destroyImageView(id());
}

ImageViewImpl::operator vk::ImageView() const {
  return vk_image_view_;
}

void ImageViewImpl::free() {
  vk::Device vk_device = getLogicalDevice();
  vk_device.destroy(vk_image_view_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

}