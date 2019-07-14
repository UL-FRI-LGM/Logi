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

#ifndef LOGI_SWAPCHAIN_SWAPCHAIN_IMAGE_HPP
#define LOGI_SWAPCHAIN_SWAPCHAIN_IMAGE_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/handle.hpp"
#include "logi/memory/image.hpp"

namespace logi {

class SwapchainKHR;
class SwapchainImageImpl;

class SwapchainImage : public Image {
 public:
  explicit SwapchainImage() = default;

  explicit SwapchainImage(const std::shared_ptr<SwapchainImageImpl>& swapchainImageImpl);

  explicit SwapchainImage(const Image& image);

  SwapchainKHR getSwapchain() const;
};

} // namespace logi

#endif // LOGI_SWAPCHAIN_SWAPCHAIN_IMAGE_HPP
