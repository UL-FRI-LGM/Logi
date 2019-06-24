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

#ifndef LOGI_MEMORY_IMAGE_IMPL_HPP
#define LOGI_MEMORY_IMAGE_IMPL_HPP

#include <variant>
#include "logi/base/vulkan_object.hpp"
#include "logi/structures/extension.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;
class MemoryAllocatorImpl;
class ImageViewImpl;
class ImageView;
class SwapchainKHRImpl;

class ImageImpl : public VulkanObject<ImageImpl>, public VulkanObjectComposite<ImageViewImpl> {
 public:
  ImageImpl(LogicalDeviceImpl& logicalDevice, const vk::ImageCreateInfo& createInfo,
            const std::optional<vk::AllocationCallbacks>& allocator = {});

  ImageImpl(SwapchainKHRImpl& swapchainKHR, const vk::Image& image);

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

  const std::shared_ptr<ImageViewImpl>& createImageView(const vk::ImageViewCreateInfo& createInfo,
                                                        const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyImageView(size_t id);

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::Image() const;

 protected:
  void free() override;

  // endregion

 private:
  std::variant<std::reference_wrapper<LogicalDeviceImpl>, std::reference_wrapper<SwapchainKHRImpl>> owner_;
  vk::Image vkImage_;
  std::optional<vk::AllocationCallbacks> allocator_;
};

} // namespace logi

#endif // LOGI_MEMORY_IMAGE_IMPL_HPP
