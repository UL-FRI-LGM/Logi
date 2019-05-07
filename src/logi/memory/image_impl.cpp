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

#include "logi/memory/image_impl.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/memory/image_view.hpp"
#include "logi/memory/memory_allocator_impl.hpp"

namespace logi {

ImageImpl::ImageImpl(MemoryAllocatorImpl& memory_allocator, const vk::ImageCreateInfo& image_create_info,
                     const VmaAllocationCreateInfo& allocation_create_info)
  : memory_allocator_(&memory_allocator), allocation_(nullptr) {
  vmaCreateImage(memory_allocator_->getVmaAllocator(), reinterpret_cast<const VkImageCreateInfo*>(&image_create_info),
                 &allocation_create_info, reinterpret_cast<VkImage*>(&image_), &allocation_, nullptr);
}

ImageView ImageImpl::createImageView(const vk::ImageViewCreateInfo& create_info,
                                     const std::optional<vk::AllocationCallbacks>& allocator) {
  return ImageView(VulkanObjectComposite<ImageViewImpl>::createObject(*this, create_info, allocator));
}

void ImageImpl::destroyImageView(size_t id) {
  VulkanObjectComposite<ImageViewImpl>::destroyObject(id);
}

VulkanInstanceImpl& ImageImpl::getInstance() const {
  return memory_allocator_->getInstance();
}

PhysicalDeviceImpl& ImageImpl::getPhysicalDevice() const {
  return memory_allocator_->getPhysicalDevice();
}

LogicalDeviceImpl& ImageImpl::getLogicalDevice() const {
  return memory_allocator_->getLogicalDevice();
}

MemoryAllocatorImpl& ImageImpl::getMemoryAllocator() const {
  return *memory_allocator_;
}

const vk::DispatchLoaderDynamic& ImageImpl::getDispatcher() const {
  return memory_allocator_->getDispatcher();
}

void ImageImpl::destroy() const {
  memory_allocator_->destroyImage(id());
}

ImageImpl::operator vk::Image() const {
  return image_;
}

void ImageImpl::free() {
  VulkanObjectComposite<ImageViewImpl>::destroyAllObjects();

  if (memory_allocator_ != nullptr) {
    vmaDestroyImage(getMemoryAllocator().getVmaAllocator(), static_cast<VkImage>(image_), allocation_);
    memory_allocator_ = nullptr;
  }

  VulkanObject::free();
}

}