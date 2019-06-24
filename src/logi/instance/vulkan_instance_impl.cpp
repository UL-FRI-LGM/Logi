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

#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/base/exception.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/debug_report_callback.hpp"
#include "logi/instance/debug_report_callback_impl.hpp"
#include "logi/instance/debug_utils_messenger_ext.hpp"
#include "logi/instance/debug_utils_messenger_ext_impl.hpp"
#include "logi/surface/surface.hpp"
#include "logi/surface/surface_impl.hpp"

namespace logi {

VulkanInstanceImpl::VulkanInstanceImpl(const vk::InstanceCreateInfo& createInfo, PFN_vkCreateInstance pfnCreateInstance,
                                       PFN_vkGetInstanceProcAddr pfnGetProcAddr,
                                       const std::optional<vk::AllocationCallbacks>& allocator)
  : allocator_(allocator) {
  // Create initial dispatch with the given function pointer.
  vk::DispatchLoaderDynamic initDispatch;
  initDispatch.vkCreateInstance = pfnCreateInstance;

  vkInstance_ = vk::createInstance(createInfo, allocator_ ? &allocator_.value() : nullptr, initDispatch);
  dispatcher_ = vk::DispatchLoaderDynamic(static_cast<VkInstance>(vkInstance_), pfnGetProcAddr);

  // Fetch available physical devices.
  std::vector<vk::PhysicalDevice> devices = vkInstance_.enumeratePhysicalDevices(dispatcher_);

  // Create Vulkan device handles.
  for (vk::PhysicalDevice& device : devices) {
    VulkanObjectComposite<PhysicalDeviceImpl>::createObject(*this, device);
  }
}

// region Sub-Handles

std::vector<std::shared_ptr<PhysicalDeviceImpl>> VulkanInstanceImpl::enumeratePhysicalDevices() const {
  const std::unordered_map<size_t, std::shared_ptr<PhysicalDeviceImpl>>& ownedDevices =
    VulkanObjectComposite<PhysicalDeviceImpl>::getHandles();

  std::vector<std::shared_ptr<PhysicalDeviceImpl>> devices;
  devices.reserve(ownedDevices.size());

  for (const auto& entry : ownedDevices) {
    devices.emplace_back(entry.second);
  }

  return devices;
}

const std::shared_ptr<DebugReportCallbackEXTImpl>&
  VulkanInstanceImpl::createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<DebugReportCallbackEXTImpl>::createObject(*this, createInfo, allocator);
}

void VulkanInstanceImpl::destroyDebugReportCallbackEXT(size_t id) {
  VulkanObjectComposite<DebugReportCallbackEXTImpl>::destroyObject(id);
}

const std::shared_ptr<DebugUtilsMessengerEXTImpl>&
  VulkanInstanceImpl::createDebugUtilsMessengerEXT(const vk::DebugUtilsMessengerCreateInfoEXT& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<DebugUtilsMessengerEXTImpl>::createObject(*this, createInfo, allocator);
}

void VulkanInstanceImpl::destroyDebugUtilsMessengerEXT(size_t id) {
  VulkanObjectComposite<DebugUtilsMessengerEXTImpl>::destroyObject(id);
}

const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::registerSurfaceKHR(const vk::SurfaceKHR& vkSurface,
                                         const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, vkSurface, allocator);
}

#ifdef VK_USE_PLATFORM_ANDROID_KHR
const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::createAndroidSurfaceKHR(const vk::AndroidSurfaceCreateInfoKHR& createInfo,
                                              const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator);
}
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::createDisplayPlaneSurfaceKHR(const vk::DisplaySurfaceCreateInfoKHR& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator);
}

#ifdef VK_USE_PLATFORM_IOS_MVK
const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::createIOSSurfaceMVK(const vk::IOSSurfaceCreateInfoMVK& createInfo,
                                          const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator);
}
#endif /*VK_USE_PLATFORM_IOS_MVK*/

#ifdef VK_USE_PLATFORM_FUCHSIA
const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::createImagePipeSurfaceFUCHSIA(const vk::ImagePipeSurfaceCreateInfoFUCHSIA& createInfo,
                                                    const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator);
}
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#ifdef VK_USE_PLATFORM_MACOS_MVK
const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::createMacOSSurfaceMVK(const vk::MacOSSurfaceCreateInfoMVK& createInfo,
                                            const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator);
}
#endif /*VK_USE_PLATFORM_MACOS_MVK*/

#ifdef VK_USE_PLATFORM_METAL_EXT
const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::createMetalSurfaceEXT(const vk::MetalSurfaceCreateInfoEXT& createInfo,
                                            const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator);
}
#endif /*VK_USE_PLATFORM_METAL_EXT*/

#ifdef VK_USE_PLATFORM_GGP
const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::createStreamDescriptorSurfaceGGP(const vk::StreamDescriptorSurfaceCreateInfoGGP& createInfo,
                                                       const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator);
}
#endif /*VK_USE_PLATFORM_GGP*/

#ifdef VK_USE_PLATFORM_VI_NN
const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::createViSurfaceNN(const vk::ViSurfaceCreateInfoNN& createInfo,
                                        const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator);
}
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::createWaylandSurfaceKHR(const vk::WaylandSurfaceCreateInfoKHR& createInfo,
                                              const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator);
}
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::createWin32SurfaceKHR(const vk::Win32SurfaceCreateInfoKHR& createInfo,
                                            const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator);
}
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::createXcbSurfaceKHR(const vk::XcbSurfaceCreateInfoKHR& createInfo,
                                          const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator);
}
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
const std::shared_ptr<SurfaceKHRImpl>&
  VulkanInstanceImpl::createXlibSurfaceKHR(const vk::XlibSurfaceCreateInfoKHR& createInfo,
                                           const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator);
}
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

void VulkanInstanceImpl::destroySurfaceKHR(size_t id) {
  VulkanObjectComposite<SurfaceKHRImpl>::destroyObject(id);
}

// endregion

// region Vulkan Commands

void VulkanInstanceImpl::debugReportMessageEXT(const vk::DebugReportFlagsEXT& flags,
                                               vk::DebugReportObjectTypeEXT objectType, uint64_t object,
                                               size_t location, int32_t messageCode, const std::string& layerPrefix,
                                               const std::string& message) const {
  vkInstance_.debugReportMessageEXT(flags, objectType, object, location, messageCode, layerPrefix, message,
                                    getDispatcher());
}

void VulkanInstanceImpl::submitDebugUtilsMessageEXT(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    const vk::DebugUtilsMessageTypeFlagsEXT& messageTypes,
                                                    const vk::DebugUtilsMessengerCallbackDataEXT& callbackData) const {
  vkInstance_.submitDebugUtilsMessageEXT(messageSeverity, messageTypes, callbackData, getDispatcher());
}

PFN_vkVoidFunction VulkanInstanceImpl::getProcAddr(const std::string& name) const {
  return vkInstance_.getProcAddr(name, getDispatcher());
}

// endregion Vulkan Commands

const vk::DispatchLoaderDynamic& VulkanInstanceImpl::getDispatcher() const {
  return dispatcher_;
}

VulkanInstanceImpl::operator vk::Instance() const {
  return vkInstance_;
}

void VulkanInstanceImpl::free() {
  VulkanObjectComposite<DebugReportCallbackEXTImpl>::destroyAllObjects();
  VulkanObjectComposite<DebugUtilsMessengerEXTImpl>::destroyAllObjects();
  VulkanObjectComposite<PhysicalDeviceImpl>::destroyAllObjects();
  VulkanObjectComposite<SurfaceKHRImpl>::destroyAllObjects();

  vkInstance_.destroy(allocator_ ? &allocator_.value() : nullptr);
  VulkanObject::free();
}

void VulkanInstanceImpl::destroy() {
  free();
}

} // namespace logi
