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
#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/debug_report_callback.hpp"
#include "logi/instance/debug_utils_messenger_ext.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"

namespace logi {

class SurfaceKHR;

class VulkanInstance : public Handle<VulkanInstanceImpl> {
 public:
  using Handle::Handle;

  // region Sub-Handles

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateDebugReportCallbackEXT.html">vkCreateDebugReportCallbackEXT</a>
   */  
  DebugReportCallbackEXT
    createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyDebugReportCallbackEXT.html">vkDestroyDebugReportCallbackEXT</a>
   */
  void destroyDebugReportCallbackEXT(const DebugReportCallbackEXT& callback) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateDebugUtilsMessengerEXT.html">vkCreateDebugUtilsMessengerEXT</a>
   */
  DebugUtilsMessengerEXT
    createDebugUtilsMessengerEXT(const vk::DebugUtilsMessengerCreateInfoEXT& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyDebugUtilsMessengerEXT.html">vkDestroyDebugUtilsMessengerEXT</a>
   */
  void destroyDebugUtilsMessengerEXT(const DebugUtilsMessengerEXT& messenger) const;

  /**
   * @brief Register created surface to Logi objext
   */
  SurfaceKHR registerSurfaceKHR(const vk::SurfaceKHR& vkSurface,
                                const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateAndroidSurfaceKHR.html">vkCreateAndroidSurfaceKHR</a>
   */
  SurfaceKHR createAndroidSurfaceKHR(const vk::AndroidSurfaceCreateInfoKHR& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateDisplayPlaneSurfaceKHR.html">vkCreateDisplayPlaneSurfaceKHR</a>
   */
  SurfaceKHR createDisplayPlaneSurfaceKHR(const vk::DisplaySurfaceCreateInfoKHR& createInfo,
                                          const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

#ifdef VK_USE_PLATFORM_IOS_MVK
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateIOSSurfaceMVK.html">vkCreateIOSSurfaceMVK</a>
   */
  SurfaceKHR createIOSSurfaceMVK(const vk::IOSSurfaceCreateInfoMVK& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_IOS_MVK*/

#ifdef VK_USE_PLATFORM_FUCHSIA
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateImagePipeSurfaceFUCHSIA.html">vkCreateImagePipeSurfaceFUCHSIA</a>
   */
  SurfaceKHR createImagePipeSurfaceFUCHSIA(const vk::ImagePipeSurfaceCreateInfoFUCHSIA& createInfo,
                                           const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#ifdef VK_USE_PLATFORM_MACOS_MVK
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateMacOSSurfaceMVK.html">vkCreateMacOSSurfaceMVK</a>
   */
  SurfaceKHR createMacOSSurfaceMVK(const vk::MacOSSurfaceCreateInfoMVK& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_MACOS_MVK*/

#ifdef VK_USE_PLATFORM_METAL_EXT
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateMetalSurfaceEXT.html">vkCreateMetalSurfaceEXT</a>
   */
  SurfaceKHR createMetalSurfaceEXT(const vk::MetalSurfaceCreateInfoEXT& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_METAL_EXT*/

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateStreamDescriptorSurfaceGGP.html">vkCreateStreamDescriptorSurfaceGGP</a>
   */
#ifdef VK_USE_PLATFORM_GGP
  SurfaceKHR createStreamDescriptorSurfaceGGP(const vk::StreamDescriptorSurfaceCreateInfoGGP& createInfo,
                                              const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_GGP*/

#ifdef VK_USE_PLATFORM_VI_NN
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateViSurfaceNN.html">vkCreateViSurfaceNN</a>
   */
  SurfaceKHR createViSurfaceNN(const vk::ViSurfaceCreateInfoNN& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateWaylandSurfaceKHR.html">vkCreateWaylandSurfaceKHR</a>
   */
  SurfaceKHR createWaylandSurfaceKHR(const vk::WaylandSurfaceCreateInfoKHR& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateWin32SurfaceKHR.html">vkCreateWin32SurfaceKHR</a>
   */
  SurfaceKHR createWin32SurfaceKHR(const vk::Win32SurfaceCreateInfoKHR& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateXcbSurfaceKHR.html">vkCreateXcbSurfaceKHR</a>
   */
  SurfaceKHR createXcbSurfaceKHR(const vk::XcbSurfaceCreateInfoKHR& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateXlibSurfaceKHR.html">vkCreateXlibSurfaceKHR</a>
   */
  SurfaceKHR createXlibSurfaceKHR(const vk::XlibSurfaceCreateInfoKHR& createInfo,
                                  const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroySurfaceKHR.html">vkDestroySurfaceKHR</a>
   */
  void destroySurfaceKHR(const SurfaceKHR& surface) const;
  // endregion

  // region Vulkan Commands

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkEnumeratePhysicalDevices.html">vkEnumeratePhysicalDevices</a>
   */
  std::vector<PhysicalDevice> enumeratePhysicalDevices() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDebugReportMessageEXT.html">vkDebugReportMessageEXT</a>
   */
  void debugReportMessageEXT(const vk::DebugReportFlagsEXT& flags, vk::DebugReportObjectTypeEXT objectType,
                             uint64_t object, size_t location, int32_t messageCode, const std::string& layerPrefix,
                             const std::string& message) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkSubmitDebugUtilsMessageEXT.html">vkSubmitDebugUtilsMessageEXT</a>
   */
  void submitDebugUtilsMessageEXT(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                  const vk::DebugUtilsMessageTypeFlagsEXT& messageTypes,
                                  const vk::DebugUtilsMessengerCallbackDataEXT& callbackData) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetInstanceProcAddr.html">vkGetInstanceProcAddr</a>
   */
  PFN_vkVoidFunction getProcAddr(const std::string& name) const;

  // endregion

  // region Logi Declarations

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  operator const vk::Instance&() const;

  void destroy() const;

  // endregion
};

/**
 * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateInstance.html">vkCreateInstance</a>
 */
VulkanInstance createInstance(const vk::InstanceCreateInfo& createInfo,
                              PFN_vkCreateInstance pfnCreateInstance = &vkCreateInstance,
                              PFN_vkGetInstanceProcAddr pfnGetProcAddr = &vkGetInstanceProcAddr,
                              const std::optional<vk::AllocationCallbacks>& allocator = {});

/**
 * @brief Check if all requested validation layers are present among the available layers
 */
bool checkValidationLayerSupport(const std::vector<const char*>& layers);

} // namespace logi

#endif // LOGI_BASE_VULKAN_INSTANCE_HPP