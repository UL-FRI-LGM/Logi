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

#ifndef LOGI_BASE_VULKAN_INSTANCE_HPP
#define LOGI_BASE_VULKAN_INSTANCE_HPP

#include <optional>
#include <vulkan/vulkan.hpp>
#include "logi/base/handle.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/debug_report_callback.hpp"
#include "logi/instance/debug_utils_messenger_ext.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"

namespace logi {

class VulkanInstance : public Handle<VulkanInstanceImpl> {
 public:
  using Handle::Handle;

  // region Sub-Handles
  DebugReportCallbackEXT
    createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyDebugReportCallbackEXT(const DebugReportCallbackEXT& callback) const;

  DebugUtilsMessengerEXT
    createDebugUtilsMessengerEXT(const vk::DebugUtilsMessengerCreateInfoEXT& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyDebugUtilsMessengerEXT(const DebugUtilsMessengerEXT& messenger) const;

  SurfaceKHR registerSurfaceKHR(const vk::SurfaceKHR& vkSurface,
                                const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  SurfaceKHR createAndroidSurfaceKHR(const vk::AndroidSurfaceCreateInfoKHR& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

  SurfaceKHR createDisplayPlaneSurfaceKHR(const vk::DisplaySurfaceCreateInfoKHR& createInfo,
                                          const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

#ifdef VK_USE_PLATFORM_IOS_MVK
  SurfaceKHR createIOSSurfaceMVK(const vk::IOSSurfaceCreateInfoMVK& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_IOS_MVK*/

#ifdef VK_USE_PLATFORM_FUCHSIA
  SurfaceKHR createImagePipeSurfaceFUCHSIA(const vk::ImagePipeSurfaceCreateInfoFUCHSIA& createInfo,
                                           const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#ifdef VK_USE_PLATFORM_MACOS_MVK
  SurfaceKHR createMacOSSurfaceMVK(const vk::MacOSSurfaceCreateInfoMVK& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_MACOS_MVK*/

#ifdef VK_USE_PLATFORM_METAL_EXT
  SurfaceKHR createMetalSurfaceEXT(const vk::MetalSurfaceCreateInfoEXT& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_METAL_EXT*/

#ifdef VK_USE_PLATFORM_GGP
  SurfaceKHR createStreamDescriptorSurfaceGGP(const vk::StreamDescriptorSurfaceCreateInfoGGP& createInfo,
                                              const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_GGP*/

#ifdef VK_USE_PLATFORM_VI_NN
  SurfaceKHR createViSurfaceNN(const vk::ViSurfaceCreateInfoNN& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  SurfaceKHR createWaylandSurfaceKHR(const vk::WaylandSurfaceCreateInfoKHR& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
  SurfaceKHR createWin32SurfaceKHR(const vk::Win32SurfaceCreateInfoKHR& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
  SurfaceKHR createXcbSurfaceKHR(const vk::XcbSurfaceCreateInfoKHR& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
  SurfaceKHR createXlibSurfaceKHR(const vk::XlibSurfaceCreateInfoKHR& createInfo,
                                  const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

  void destroySurfaceKHR(const SurfaceKHR& surface) const;
  // endregion

  // region Vulkan Commands

  std::vector<PhysicalDevice> enumeratePhysicalDevices() const;

  void debugReportMessageEXT(const vk::DebugReportFlagsEXT& flags, vk::DebugReportObjectTypeEXT objectType,
                             uint64_t object, size_t location, int32_t messageCode, const std::string& layerPrefix,
                             const std::string& message) const;

  void submitDebugUtilsMessageEXT(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                  const vk::DebugUtilsMessageTypeFlagsEXT& messageTypes,
                                  const vk::DebugUtilsMessengerCallbackDataEXT& callbackData) const;

  PFN_vkVoidFunction getProcAddr(const std::string& name) const;

  // endregion

  // region Logi Declarations

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  operator vk::Instance() const;

  void destroy();

  // endregion
};

VulkanInstance createInstance(const vk::InstanceCreateInfo& createInfo,
                              PFN_vkCreateInstance pfnCreateInstance = &vkCreateInstance,
                              PFN_vkGetInstanceProcAddr pfnGetProcAddr = &vkGetInstanceProcAddr,
                              const std::optional<vk::AllocationCallbacks>& allocator = {});

bool checkValidationLayerSupport(const std::vector<const char*>& layers);

} // namespace logi

#endif // LOGI_BASE_VULKAN_INSTANCE_HPP