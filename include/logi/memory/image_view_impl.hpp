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

#ifndef LOGI_MEMORY_IMAGE_VIEW_IMPL_HPP
#define LOGI_MEMORY_IMAGE_VIEW_IMPL_HPP

#include <optional>
#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;
class MemoryAllocatorImpl;
class ImageImpl;

class ImageViewImpl : public VulkanObject<ImageViewImpl> {
 public:
  ImageViewImpl(ImageImpl& image, vk::ImageViewCreateInfo createInfo,
                const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  MemoryAllocatorImpl& getMemoryAllocator() const;

  ImageImpl& getImage() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::ImageView() const;

 protected:
  void free() override;

  // endregion

 private:
  ImageImpl& image_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::ImageView vkImageView_;
};

} // namespace logi

#endif // LOGI_MEMORY_IMAGE_VIEW_IMPL_HPP
