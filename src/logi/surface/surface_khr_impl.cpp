/**
 * Project Logi source code
 * Copyright (C) 2019 Lana Besevic
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

#include "logi/surface/surface_khr_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"

namespace logi {

SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::SurfaceKHR& vkSurface,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator), vkSurface_(vkSurface) {}

#ifdef VK_USE_PLATFORM_ANDROID_KHR
SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::AndroidSurfaceCreateInfoKHR& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkSurface_ =
    vkInstance.createAndroidSurfaceKHR(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::DisplaySurfaceCreateInfoKHR& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkSurface_ =
    vkInstance.createDisplayPlaneSurfaceKHR(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

#ifdef VK_USE_PLATFORM_IOS_MVK
SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::IOSSurfaceCreateInfoMVK& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkSurface_ = vkInstance.createIOSSurfaceMVK(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}
#endif /*VK_USE_PLATFORM_IOS_MVK*/

#ifdef VK_USE_PLATFORM_FUCHSIA
SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::ImagePipeSurfaceCreateInfoFUCHSIA& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkSurface_ =
    vkInstance.createImagePipeSurfaceFUCHSIA(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#ifdef VK_USE_PLATFORM_MACOS_MVK
SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::MacOSSurfaceCreateInfoMVK& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkSurface_ =
    vkInstance.createMacOSSurfaceMVK(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}
#endif /*VK_USE_PLATFORM_MACOS_MVK*/

#ifdef VK_USE_PLATFORM_METAL_EXT
SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::MetalSurfaceCreateInfoEXT& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkSurface_ =
    vkInstance.createMetalSurfaceEXT(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}
#endif /*VK_USE_PLATFORM_METAL_EXT*/

#ifdef VK_USE_PLATFORM_GGP
SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::StreamDescriptorSurfaceCreateInfoGGP& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkSurface_ = vkInstance.createStreamDescriptorSurfaceGGP(createInfo, allocator_ ? &allocator_.value() : nullptr,
                                                           getDispatcher());
}
#endif /*VK_USE_PLATFORM_GGP*/

#ifdef VK_USE_PLATFORM_VI_NN
SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::ViSurfaceCreateInfoNN& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkSurface_ = vkInstance.createViSurfaceNN(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::WaylandSurfaceCreateInfoKHR& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkSurface_ =
    vkInstance.createWaylandSurfaceKHR(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::Win32SurfaceCreateInfoKHR& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkSurface_ =
    vkInstance.createWin32SurfaceKHR(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::XcbSurfaceCreateInfoKHR& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkSurface_ = vkInstance.createXcbSurfaceKHR(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
SurfaceKHRImpl::SurfaceKHRImpl(VulkanInstanceImpl& instance, const vk::XlibSurfaceCreateInfoKHR& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkSurface_ = vkInstance.createXlibSurfaceKHR(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

VulkanInstanceImpl& SurfaceKHRImpl::getInstance() const {
  return instance_;
}

const vk::DispatchLoaderDynamic& SurfaceKHRImpl::getDispatcher() const {
  return instance_.getDispatcher();
}

void SurfaceKHRImpl::destroy() const {
  instance_.destroySurfaceKHR(id());
}

SurfaceKHRImpl::operator vk::SurfaceKHR() const {
  return vkSurface_;
}

void SurfaceKHRImpl::free() {
  vk::Instance vkInstance = instance_;
  vkInstance.destroySurfaceKHR(vkSurface_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  vkSurface_ = nullptr;
  VulkanObject::free();
}

} // namespace logi