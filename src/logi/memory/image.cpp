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

vk::MemoryRequirements Image::getMemoryRequirements() const {
  return object_->getMemoryRequirements();
}

std::vector<vk::SparseImageMemoryRequirements> Image::getSparseMemoryRequirements() const {
  return object_->getSparseMemoryRequirements();
}
vk::SubresourceLayout Image::getImageSubresourceLayout(const vk::ImageSubresource& subresource) const {
  return object_->getImageSubresourceLayout(subresource);
}

vk::ResultValueType<vk::ImageDrmFormatModifierPropertiesEXT>::type Image::getDrmFormatModifierPropertiesEXT() const {
  return object_->getDrmFormatModifierPropertiesEXT();
}

vk::MemoryRequirements2
  Image::getImageMemoryRequirements2(const ConstVkNextProxy<vk::ImageMemoryRequirementsInfo2>& next) const {
  return object_->getImageMemoryRequirements2(next);
}
vk::MemoryRequirements2KHR
  Image::getImageMemoryRequirements2KHR(const ConstVkNextProxy<vk::ImageMemoryRequirementsInfo2KHR>& next) const {
  return object_->getImageMemoryRequirements2KHR(next);
}

std::vector<vk::SparseImageMemoryRequirements2>
  Image::getImageSparseMemoryRequirements2(const ConstVkNextProxy<vk::ImageSparseMemoryRequirementsInfo2>& next) const {
  return object_->getImageSparseMemoryRequirements2(next);
}

std::vector<vk::SparseImageMemoryRequirements2KHR> Image::getImageSparseMemoryRequirements2KHR(
  const ConstVkNextProxy<vk::ImageSparseMemoryRequirementsInfo2KHR>& next) const {
  return object_->getImageSparseMemoryRequirements2KHR(next);
}

ImageView Image::createImageView(const vk::ImageViewCreateFlags& flags, vk::ImageViewType viewType, vk::Format format,
                                 const vk::ComponentMapping& components,
                                 const vk::ImageSubresourceRange& subresourceRange,
                                 const ConstVkNextProxy<vk::ImageViewCreateInfo>& next,
                                 const std::optional<vk::AllocationCallbacks>& allocator) const {
  return ImageView(object_->createImageView(flags, viewType, format, components, subresourceRange, next, allocator));
}

void Image::destroyImageView(const ImageView& image) const {
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

const vk::DispatchLoaderDynamic& Image::getDispatcher() const {
  return object_->getDispatcher();
}

void Image::destroy() const {
  object_->destroy();
}

Image::operator const vk::Image&() const {
  return object_->operator const vk::Image&();
}

vk::ResultValueType<void>::type Image::bindMemory(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset) const {
  return object_->bindMemory(memory, memoryOffset);
}

vk::ResultValueType<void>::type Image::bindMemory2(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset,
                                                   const ConstVkNextProxy<vk::BindImageMemoryInfo>& next) const {
  return object_->bindMemory2(memory, memoryOffset, next);
}

vk::ResultValueType<void>::type Image::bindMemory2KHR(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset,
                                                      const ConstVkNextProxy<vk::BindImageMemoryInfoKHR>& next) const {
  return object_->bindMemory2KHR(memory, memoryOffset, next);
}

}