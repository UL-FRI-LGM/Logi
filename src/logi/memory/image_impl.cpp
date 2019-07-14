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
#include "logi/swapchain/swapchain_khr_impl.hpp"

namespace logi {

ImageImpl::ImageImpl(LogicalDeviceImpl& logicalDevice, const vk::ImageCreateInfo& createInfo,
                     const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.createImage(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

ImageImpl::ImageImpl(LogicalDeviceImpl& logicalDevice, const vk::Image& image)
  : logicalDevice_(logicalDevice), vkImage_(image), allocator_(std::nullopt) {}

vk::MemoryRequirements ImageImpl::getMemoryRequirements() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getImageMemoryRequirements(vkImage_, getDispatcher());
}

std::vector<vk::SparseImageMemoryRequirements> ImageImpl::getSparseMemoryRequirements() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getImageSparseMemoryRequirements(vkImage_, getDispatcher());
}

vk::SubresourceLayout ImageImpl::getImageSubresourceLayout(const vk::ImageSubresource& subresource) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getImageSubresourceLayout(vkImage_, subresource, getDispatcher());
}

vk::ResultValueType<vk::ImageDrmFormatModifierPropertiesEXT>::type
  ImageImpl::getDrmFormatModifierPropertiesEXT() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getImageDrmFormatModifierPropertiesEXT(vkImage_, getDispatcher());
}

vk::MemoryRequirements2
  ImageImpl::getImageMemoryRequirements2(const ConstVkNextProxy<vk::ImageMemoryRequirementsInfo2>& next) const {
  vk::ImageMemoryRequirementsInfo2 requirementsInfo;
  requirementsInfo.pNext = next;

  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getImageMemoryRequirements2(requirementsInfo, getDispatcher());
}

vk::MemoryRequirements2KHR
  ImageImpl::getImageMemoryRequirements2KHR(const ConstVkNextProxy<vk::ImageMemoryRequirementsInfo2KHR>& next) const {
  vk::ImageMemoryRequirementsInfo2KHR requirementsInfo;
  requirementsInfo.pNext = next;

  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getImageMemoryRequirements2KHR(requirementsInfo, getDispatcher());
}

std::vector<vk::SparseImageMemoryRequirements2> ImageImpl::getImageSparseMemoryRequirements2(
  const ConstVkNextProxy<vk::ImageSparseMemoryRequirementsInfo2>& next) const {
  vk::ImageSparseMemoryRequirementsInfo2 requirementsInfo;
  requirementsInfo.pNext = next;

  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getImageSparseMemoryRequirements2(requirementsInfo, getDispatcher());
}

std::vector<vk::SparseImageMemoryRequirements2KHR> ImageImpl::getImageSparseMemoryRequirements2KHR(
  const ConstVkNextProxy<vk::ImageSparseMemoryRequirementsInfo2KHR>& next) const {
  vk::ImageSparseMemoryRequirementsInfo2KHR requirementsInfo;
  requirementsInfo.pNext = next;

  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getImageSparseMemoryRequirements2KHR(requirementsInfo, getDispatcher());
}

vk::ResultValueType<void>::type ImageImpl::bindMemory(const vk::DeviceMemory& memory,
                                                      vk::DeviceSize memoryOffset) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.bindImageMemory(vkImage_, memory, memoryOffset, getDispatcher());
}

vk::ResultValueType<void>::type ImageImpl::bindMemory2(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset,
                                                       const ConstVkNextProxy<vk::BindImageMemoryInfo>& next) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);

  vk::BindImageMemoryInfo memoryInfo(vkImage_, memory, memoryOffset);
  memoryInfo.pNext = next;

  return vkDevice.bindImageMemory2(memoryInfo, getDispatcher());
}

vk::ResultValueType<void>::type
  ImageImpl::bindMemory2KHR(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset,
                            const ConstVkNextProxy<vk::BindImageMemoryInfoKHR>& next) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);

  vk::BindImageMemoryInfoKHR memoryInfo(vkImage_, memory, memoryOffset);
  memoryInfo.pNext = next;

  return vkDevice.bindImageMemory2KHR(memoryInfo, getDispatcher());
}

const std::shared_ptr<ImageViewImpl>&
  ImageImpl::createImageView(const vk::ImageViewCreateFlags& flags, vk::ImageViewType viewType, vk::Format format,
                             const vk::ComponentMapping& components, const vk::ImageSubresourceRange& subresourceRange,
                             const ConstVkNextProxy<vk::ImageViewCreateInfo>& next,
                             const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<ImageViewImpl>::createObject(*this, flags, viewType, format, components,
                                                            subresourceRange, next, allocator);
}

void ImageImpl::destroyImageView(size_t id) {
  VulkanObjectComposite<ImageViewImpl>::destroyObject(id);
}

VulkanInstanceImpl& ImageImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& ImageImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& ImageImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& ImageImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void ImageImpl::destroy() const {
  logicalDevice_.destroyImage(id());
}

ImageImpl::operator const vk::Image&() const {
  return vkImage_;
}

void ImageImpl::free() {
  VulkanObjectComposite<ImageViewImpl>::destroyAllObjects();

  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkImage_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());

  VulkanObject::free();
}

}