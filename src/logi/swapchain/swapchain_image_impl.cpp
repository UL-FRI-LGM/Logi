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

#include "logi/swapchain/swapchain_image_impl.hpp"
#include "logi/memory/image_view_impl.hpp"
#include "logi/swapchain/swapchain_khr_impl.hpp"

namespace logi {

SwapchainImageImpl::SwapchainImageImpl(SwapchainKHRImpl& swapchain, const vk::Image& vkImage)
  : ImageImpl(swapchain.getLogicalDevice(), vkImage), swapchain_(swapchain) {}

SwapchainKHRImpl& SwapchainImageImpl::getSwapchain() const {
  return swapchain_;
}

void SwapchainImageImpl::destroy() const {
  // Swapchain image is destroyed implicitly.
}

void SwapchainImageImpl::free() {
  VulkanObjectComposite<ImageViewImpl>::destroyAllObjects();
  VulkanObject::free();
}

}