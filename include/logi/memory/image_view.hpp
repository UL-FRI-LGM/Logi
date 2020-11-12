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

#ifndef LOGI_MEMORY_IMAGE_VIEW_HPP
#define LOGI_MEMORY_IMAGE_VIEW_HPP

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"
#include "logi/memory/image_view_impl.hpp"
#include "logi/structures/extension.hpp"

namespace logi {

class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;
class MemoryAllocator;
class Image;

class ImageView : public Handle<ImageViewImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetImageViewHandleNVX.html">vkGetImageViewHandleNVX</a>
   */
  uint32_t getHandleNVX(vk::DescriptorType descriptorType, vk::Sampler sampler,
                        const ConstVkNextProxy<vk::ImageViewHandleInfoNVX>& next) const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  Image getImage() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::ImageView&() const;

  // endregion
};

} // namespace logi

#endif // LOGI_MEMORY_IMAGE_VIEW_HPP
