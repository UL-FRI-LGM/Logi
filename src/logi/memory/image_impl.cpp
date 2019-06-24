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

ImageImpl::ImageImpl(MemoryAllocatorImpl& memoryAllocator, const vk::ImageCreateInfo& imageCreateInfo,
                     const VmaAllocationCreateInfo& allocationCreateInfo)
  : memoryAllocator_(&memoryAllocator), allocation_(nullptr) {
  vmaCreateImage(memoryAllocator_->getVmaAllocator(), reinterpret_cast<const VkImageCreateInfo*>(&imageCreateInfo),
                 &allocationCreateInfo, reinterpret_cast<VkImage*>(&image_), &allocation_, nullptr);
}

vk::MemoryRequirements ImageImpl::getMemoryRequirements() const {
  vk::Device vkDevice = getLogicalDevice();
  return vkDevice.getImageMemoryRequirements(image_, getDispatcher());
}

std::vector<vk::SparseImageMemoryRequirements> ImageImpl::getSparseMemoryRequirements() const {
  vk::Device vkDevice = getLogicalDevice();
  return vkDevice.getImageSparseMemoryRequirements(image_, getDispatcher());
}

vk::SubresourceLayout ImageImpl::getImageSubresourceLayout(const vk::ImageSubresource& subresource) const {
  vk::Device vkDevice = getLogicalDevice();
  return vkDevice.getImageSubresourceLayout(image_, subresource, getDispatcher());
}

vk::ResultValueType<vk::ImageDrmFormatModifierPropertiesEXT>::type
  ImageImpl::getDrmFormatModifierPropertiesEXT() const {
  vk::Device vkDevice = getLogicalDevice();
  return vkDevice.getImageDrmFormatModifierPropertiesEXT(image_, getDispatcher());
}

vk::MemoryRequirements2
  ImageImpl::getImageMemoryRequirements2(const ConstVkNextProxy<vk::ImageMemoryRequirementsInfo2>& next) const {
  vk::ImageMemoryRequirementsInfo2 requirementsInfo;
  requirementsInfo.pNext = next;

  auto vkDevice = static_cast<vk::Device>(getLogicalDevice());
  return vkDevice.getImageMemoryRequirements2(requirementsInfo, getDispatcher());
}

vk::MemoryRequirements2KHR
  ImageImpl::getImageMemoryRequirements2KHR(const ConstVkNextProxy<vk::ImageMemoryRequirementsInfo2KHR>& next) const {
  vk::ImageMemoryRequirementsInfo2KHR requirementsInfo;
  requirementsInfo.pNext = next;

  auto vkDevice = static_cast<vk::Device>(getLogicalDevice());
  return vkDevice.getImageMemoryRequirements2KHR(requirementsInfo, getDispatcher());
}

std::vector<vk::SparseImageMemoryRequirements2> ImageImpl::getImageSparseMemoryRequirements2(
  const ConstVkNextProxy<vk::ImageSparseMemoryRequirementsInfo2>& next) const {
  vk::ImageSparseMemoryRequirementsInfo2 requirementsInfo;
  requirementsInfo.pNext = next;

  auto vkDevice = static_cast<vk::Device>(getLogicalDevice());
  return vkDevice.getImageSparseMemoryRequirements2(requirementsInfo, getDispatcher());
}

std::vector<vk::SparseImageMemoryRequirements2KHR> ImageImpl::getImageSparseMemoryRequirements2KHR(
  const ConstVkNextProxy<vk::ImageSparseMemoryRequirementsInfo2KHR>& next) const {
  vk::ImageSparseMemoryRequirementsInfo2KHR requirementsInfo;
  requirementsInfo.pNext = next;

  auto vkDevice = static_cast<vk::Device>(getLogicalDevice());
  return vkDevice.getImageSparseMemoryRequirements2KHR(requirementsInfo, getDispatcher());
}

ImageView ImageImpl::createImageView(const vk::ImageViewCreateInfo& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator) {
  return ImageView(VulkanObjectComposite<ImageViewImpl>::createObject(*this, createInfo, allocator));
}

void ImageImpl::destroyImageView(size_t id) {
  VulkanObjectComposite<ImageViewImpl>::destroyObject(id);
}

VulkanInstanceImpl& ImageImpl::getInstance() const {
  return memoryAllocator_->getInstance();
}

PhysicalDeviceImpl& ImageImpl::getPhysicalDevice() const {
  return memoryAllocator_->getPhysicalDevice();
}

LogicalDeviceImpl& ImageImpl::getLogicalDevice() const {
  return memoryAllocator_->getLogicalDevice();
}

MemoryAllocatorImpl& ImageImpl::getMemoryAllocator() const {
  return *memoryAllocator_;
}

const vk::DispatchLoaderDynamic& ImageImpl::getDispatcher() const {
  return memoryAllocator_->getDispatcher();
}

void ImageImpl::destroy() const {
  memoryAllocator_->destroyImage(id());
}

ImageImpl::operator vk::Image() const {
  return image_;
}

void ImageImpl::free() {
  VulkanObjectComposite<ImageViewImpl>::destroyAllObjects();

  if (memoryAllocator_ != nullptr) {
    vmaDestroyImage(getMemoryAllocator().getVmaAllocator(), static_cast<VkImage>(image_), allocation_);
    memoryAllocator_ = nullptr;
  }

  VulkanObject::free();
}

}