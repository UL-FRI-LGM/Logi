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

#ifndef LOGI_MEMORY_IMAGE_HPP
#define LOGI_MEMORY_IMAGE_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/handle.hpp"
#include "logi/memory/image_impl.hpp"

namespace logi {

class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;
class ImageView;
class MemoryAllocator;

class Image : public Handle<ImageImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  vk::MemoryRequirements getMemoryRequirements() const;

  std::vector<vk::SparseImageMemoryRequirements> getSparseMemoryRequirements() const;

  vk::SubresourceLayout getImageSubresourceLayout(const vk::ImageSubresource& subresource) const;

  vk::ResultValueType<vk::ImageDrmFormatModifierPropertiesEXT>::type getDrmFormatModifierPropertiesEXT() const;

  vk::MemoryRequirements2
    getImageMemoryRequirements2(const ConstVkNextProxy<vk::ImageMemoryRequirementsInfo2>& next) const;

  vk::MemoryRequirements2KHR
    getImageMemoryRequirements2KHR(const ConstVkNextProxy<vk::ImageMemoryRequirementsInfo2KHR>& next) const;

  std::vector<vk::SparseImageMemoryRequirements2>
    getImageSparseMemoryRequirements2(const ConstVkNextProxy<vk::ImageSparseMemoryRequirementsInfo2>& next) const;

  std::vector<vk::SparseImageMemoryRequirements2KHR>
    getImageSparseMemoryRequirements2KHR(const ConstVkNextProxy<vk::ImageSparseMemoryRequirementsInfo2KHR>& next) const;

  vk::ResultValueType<void>::type bindMemory(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset) const;

  vk::ResultValueType<void>::type bindMemory2(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset,
                                              const ConstVkNextProxy<vk::BindImageMemoryInfo>& next) const;

  vk::ResultValueType<void>::type bindMemory2KHR(const vk::DeviceMemory& memory, vk::DeviceSize memoryOffset,
                                                 const ConstVkNextProxy<vk::BindImageMemoryInfoKHR>& next) const;

  // endregion

  // region Logi Declarations

  ImageView createImageView(const vk::ImageViewCreateFlags& flags, vk::ImageViewType viewType, vk::Format format,
                            const vk::ComponentMapping& components, const vk::ImageSubresourceRange& subresourceRange,
                            const ConstVkNextProxy<vk::ImageViewCreateInfo>& next = {},
                            const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyImageView(const ImageView& image) const;

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::Image&() const;

  // endregion
};

} // namespace logi

#endif // LOGI_MEMORY_IMAGE_HPP
