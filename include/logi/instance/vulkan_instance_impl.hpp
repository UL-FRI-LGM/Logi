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

#ifndef LOGI_BASE_VULKAN_INSTANCE_IMPL_HPP
#define LOGI_BASE_VULKAN_INSTANCE_IMPL_HPP

#include <functional>
#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class DebugReportCallbackEXT;
class DebugReportCallbackEXTImpl;
class DebugUtilsMessengerEXT;
class DebugUtilsMessengerEXTImpl;
class PhysicalDevice;
class PhysicalDeviceImpl;
class SurfaceKHR;
class SurfaceKHRImpl;

/**
 * @brief	Vulkan instance handle.
 */
class VulkanInstanceImpl : public VulkanObject<VulkanInstanceImpl>,
                           public VulkanObjectComposite<DebugReportCallbackEXTImpl>,
                           public VulkanObjectComposite<DebugUtilsMessengerEXTImpl>,
                           public VulkanObjectComposite<PhysicalDeviceImpl>,
                           public VulkanObjectComposite<SurfaceKHRImpl> {
 public:
  explicit VulkanInstanceImpl(const vk::InstanceCreateInfo& createInfo,
                              PFN_vkCreateInstance pfnCreateInstance = &vkCreateInstance,
                              PFN_vkGetInstanceProcAddr pfnGetProcAddr = &vkGetInstanceProcAddr,
                              const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Sub-Handles

  std::vector<std::shared_ptr<PhysicalDeviceImpl>> enumeratePhysicalDevices() const;

  const std::shared_ptr<DebugReportCallbackEXTImpl>&
    createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator);

  void destroyDebugReportCallbackEXT(size_t id);

  const std::shared_ptr<DebugUtilsMessengerEXTImpl>&
    createDebugUtilsMessengerEXT(const vk::DebugUtilsMessengerCreateInfoEXT& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator);

  void destroyDebugUtilsMessengerEXT(size_t id);

  const std::shared_ptr<SurfaceKHRImpl>& registerSurfaceKHR(const vk::SurfaceKHR& vkSurface,
                                                            const std::optional<vk::AllocationCallbacks>& allocator);

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  const std::shared_ptr<SurfaceKHRImpl>&
    createAndroidSurfaceKHR(const vk::AndroidSurfaceCreateInfoKHR& createInfo,
                            const std::optional<vk::AllocationCallbacks>& allocator = {});
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

  const std::shared_ptr<SurfaceKHRImpl>&
    createDisplayPlaneSurfaceKHR(const vk::DisplaySurfaceCreateInfoKHR& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {});

#ifdef VK_USE_PLATFORM_IOS_MVK
  const std::shared_ptr<SurfaceKHRImpl>&
    createIOSSurfaceMVK(const vk::IOSSurfaceCreateInfoMVK& createInfo,
                        const std::optional<vk::AllocationCallbacks>& allocator = {});
#endif /*VK_USE_PLATFORM_IOS_MVK*/

#ifdef VK_USE_PLATFORM_FUCHSIA
  const std::shared_ptr<SurfaceKHRImpl>&
    createImagePipeSurfaceFUCHSIA(const vk::ImagePipeSurfaceCreateInfoFUCHSIA& createInfo,
                                  const std::optional<vk::AllocationCallbacks>& allocator = {});
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#ifdef VK_USE_PLATFORM_MACOS_MVK
  const std::shared_ptr<SurfaceKHRImpl>&
    createMacOSSurfaceMVK(const vk::MacOSSurfaceCreateInfoMVK& createInfo,
                          const std::optional<vk::AllocationCallbacks>& allocator = {});
#endif /*VK_USE_PLATFORM_MACOS_MVK*/

#ifdef VK_USE_PLATFORM_METAL_EXT
  const std::shared_ptr<SurfaceKHRImpl>&
    createMetalSurfaceEXT(const vk::MetalSurfaceCreateInfoEXT& createInfo,
                          const std::optional<vk::AllocationCallbacks>& allocator = {});
#endif /*VK_USE_PLATFORM_METAL_EXT*/

#ifdef VK_USE_PLATFORM_GGP
  const std::shared_ptr<SurfaceKHRImpl>&
    createStreamDescriptorSurfaceGGP(const vk::StreamDescriptorSurfaceCreateInfoGGP& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator = {});
#endif /*VK_USE_PLATFORM_GGP*/

#ifdef VK_USE_PLATFORM_VI_NN
  const std::shared_ptr<SurfaceKHRImpl>&
    createViSurfaceNN(const vk::ViSurfaceCreateInfoNN& createInfo,
                      const std::optional<vk::AllocationCallbacks>& allocator = {});
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  const std::shared_ptr<SurfaceKHRImpl>&
    createWaylandSurfaceKHR(const vk::WaylandSurfaceCreateInfoKHR& createInfo,
                            const std::optional<vk::AllocationCallbacks>& allocator = {});
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
  const std::shared_ptr<SurfaceKHRImpl>&
    createWin32SurfaceKHR(const vk::Win32SurfaceCreateInfoKHR& createInfo,
                          const std::optional<vk::AllocationCallbacks>& allocator = {});
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
  const std::shared_ptr<SurfaceKHRImpl>&
    createXcbSurfaceKHR(const vk::XcbSurfaceCreateInfoKHR& createInfo,
                        const std::optional<vk::AllocationCallbacks>& allocator = {});
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
  const std::shared_ptr<SurfaceKHRImpl>&
    createXlibSurfaceKHR(const vk::XlibSurfaceCreateInfoKHR& createInfo,
                         const std::optional<vk::AllocationCallbacks>& allocator = {});
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

  void destroySurfaceKHR(size_t id);

  // endregion

  // region Vulkan Commands

  void debugReportMessageEXT(const vk::DebugReportFlagsEXT& flags, vk::DebugReportObjectTypeEXT objectType,
                             uint64_t object, size_t location, int32_t messageCode, const std::string& layerPrefix,
                             const std::string& message) const;

  void submitDebugUtilsMessageEXT(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                  const vk::DebugUtilsMessageTypeFlagsEXT& messageTypes,
                                  const vk::DebugUtilsMessengerCallbackDataEXT& callbackData) const;

  PFN_vkVoidFunction getInstanceProcAddr(const std::string& name) const;

  // endregion

  // region Logi

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  operator vk::Instance() const;

  void destroy();

 protected:
  void free() override;

  // endregion

 private:
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::Instance vkInstance_;
  vk::DispatchLoaderDynamic dispatcher_;
};

} // namespace logi

#endif // LOGI_BASE_VULKAN_INSTANCE_IMPL_HPP
