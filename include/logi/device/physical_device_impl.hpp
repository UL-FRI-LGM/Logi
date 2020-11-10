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

#ifndef LOGI_BASE_PHYSICAL_DEVICE_IMPL_HPP
#define LOGI_BASE_PHYSICAL_DEVICE_IMPL_HPP

#include <optional>
#include <string>
#include "logi/base/common.hpp"
#include "logi/base/vulkan_object.hpp"

namespace logi {

/**
 * @brief Forward declaration for owner reference.
 */
class VulkanInstanceImpl;
class LogicalDeviceImpl;
class LogicalDevice;

class PhysicalDeviceImpl : public VulkanObject,
                           public std::enable_shared_from_this<PhysicalDeviceImpl>,
                           public VulkanObjectComposite<LogicalDeviceImpl> {
 public:
  /**
   * @brief Vulkan physical device.
   *
   * @param instance            Instance handle
   * @param	vkPhysicalDevice  Vulkan physical device handle.
   */
  explicit PhysicalDeviceImpl(VulkanInstanceImpl& instance, const vk::PhysicalDevice& vkPhysicalDevice);

  // region Vulkan Declarations

  typename vk::ResultValueType<std::vector<vk::ExtensionProperties>>::type
    enumerateDeviceExtensionProperties(const std::string* layerName = nullptr) const;

  typename vk::ResultValueType<std::vector<vk::LayerProperties>>::type enumerateDeviceLayerProperties() const;

  vk::ExternalBufferProperties
    getExternalBufferProperties(const vk::PhysicalDeviceExternalBufferInfo& externalBufferInfo) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalBufferProperties(const vk::PhysicalDeviceExternalBufferInfo& externalBufferInfo) const;

  vk::ExternalBufferPropertiesKHR
    getExternalBufferPropertiesKHR(const vk::PhysicalDeviceExternalBufferInfoKHR& externalBufferInfo) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalBufferPropertiesKHR(const vk::PhysicalDeviceExternalBufferInfoKHR& externalBufferInfo) const;

  vk::ExternalFenceProperties
    getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& externalFenceInfo) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& externalFenceInfo) const;

  vk::ExternalFencePropertiesKHR
    getExternalFencePropertiesKHR(const vk::PhysicalDeviceExternalFenceInfoKHR& externalFenceInfo) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalFencePropertiesKHR(const vk::PhysicalDeviceExternalFenceInfoKHR& externalFenceInfo) const;

  vk::ExternalSemaphoreProperties
    getExternalSemaphoreProperties(const vk::PhysicalDeviceExternalSemaphoreInfo& externalSemaphoreInfo) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalSemaphoreProperties(const vk::PhysicalDeviceExternalSemaphoreInfo& externalSemaphoreInfo) const;

  vk::ExternalSemaphorePropertiesKHR
    getExternalSemaphorePropertiesKHR(const vk::PhysicalDeviceExternalSemaphoreInfoKHR& externalSemaphoreInfo) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalSemaphorePropertiesKHR(const vk::PhysicalDeviceExternalSemaphoreInfoKHR& externalSemaphoreInfo) const;

  vk::PhysicalDeviceFeatures getFeatures() const;

  vk::PhysicalDeviceFeatures2 getFeatures2() const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getFeatures2() const;

  vk::PhysicalDeviceFeatures2KHR getFeatures2KHR() const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getFeatures2KHR() const;

  vk::FormatProperties getFormatProperties(vk::Format format) const;

  vk::FormatProperties2 getFormatProperties2(vk::Format format) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getFormatProperties2(vk::Format format) const;

  vk::FormatProperties2KHR getFormatProperties2KHR(vk::Format format) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getFormatProperties2KHR(vk::Format format) const;

  vk::ImageFormatProperties getImageFormatProperties(vk::Format format, vk::ImageType type, vk::ImageTiling tiling,
                                                     const vk::ImageUsageFlags& usage,
                                                     const vk::ImageCreateFlags& flags) const;

  vk::ImageFormatProperties2 getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& formatInfo) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& formatInfo) const;

  vk::ImageFormatProperties2KHR
    getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2& formatInfo) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2KHR& formatInfo) const;

  vk::PhysicalDeviceMemoryProperties getMemoryProperties() const;

  vk::PhysicalDeviceMemoryProperties2 getMemoryProperties2() const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getMemoryProperties2() const;

  vk::PhysicalDeviceMemoryProperties2KHR getMemoryProperties2KHR() const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getMemoryProperties2KHR() const;

  vk::PhysicalDeviceProperties getProperties() const;

  vk::PhysicalDeviceProperties2 getProperties2() const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getProperties2() const;

  vk::PhysicalDeviceProperties2KHR getProperties2KHR() const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getProperties2KHR() const;

  std::vector<vk::QueueFamilyProperties> getQueueFamilyProperties() const;

  std::vector<vk::QueueFamilyProperties2> getQueueFamilyProperties2() const;

  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>> getQueueFamilyProperties2() const;

  std::vector<vk::QueueFamilyProperties2KHR> getQueueFamilyProperties2KHR() const;

  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>> getQueueFamilyProperties2KHR() const;

  std::vector<vk::SparseImageFormatProperties> getSparseImageFormatProperties(vk::Format format, vk::ImageType type,
                                                                              vk::SampleCountFlagBits samples,
                                                                              const vk::ImageUsageFlags& usage,
                                                                              vk::ImageTiling tiling) const;

  std::vector<vk::SparseImageFormatProperties2>
    getSparseImageFormatProperties2(const vk::PhysicalDeviceSparseImageFormatInfo2& pSparseImageFormatProperties) const;

  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>>
    getSparseImageFormatProperties2(const vk::PhysicalDeviceSparseImageFormatInfo2& pSparseImageFormatProperties) const;

  std::vector<vk::SparseImageFormatProperties2KHR> getSparseImageFormatProperties2KHR(
    const vk::PhysicalDeviceSparseImageFormatInfo2KHR& pSparseImageFormatProperties) const;

  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>> getSparseImageFormatProperties2KHR(
    const vk::PhysicalDeviceSparseImageFormatInfo2KHR& pSparseImageFormatProperties) const;

  vk::ResultValueType<vk::SurfaceCapabilitiesKHR>::type getSurfaceCapabilitiesKHR(vk::SurfaceKHR surface) const;

  template <typename X, typename Y, typename... Z>
  typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type
    getSurfaceCapabilities2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const;

  vk::ResultValueType<vk::SurfaceCapabilities2KHR>::type
    getSurfaceCapabilities2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const;

  vk::ResultValueType<std::vector<vk::SurfaceFormatKHR>>::type getSurfaceFormatsKHR(vk::SurfaceKHR surface) const;

  template <typename X, typename Y, typename... Z>
  typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type
    getSurfaceFormats2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const;

  vk::ResultValueType<std::vector<vk::SurfaceFormat2KHR>>::type
    getSurfaceFormats2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const;

  vk::ResultValueType<std::vector<vk::PresentModeKHR>>::type getSurfacePresentModesKHR(vk::SurfaceKHR surface) const;

  vk::ResultValueType<vk::Bool32>::type getSurfaceSupportKHR(uint32_t queueFamilyIndex, vk::SurfaceKHR surface) const;

  vk::ExternalImageFormatPropertiesNV
    getExternalImageFormatPropertiesNV(vk::Format format, vk::ImageType type, vk::ImageTiling tiling,
                                       const vk::ImageUsageFlags& usage, const vk::ImageCreateFlags& flags,
                                       const vk::ExternalMemoryHandleTypeFlagsNV& externalHandleType) const;

  // vk::DeviceGeneratedCommandsLimitsNVX
  //   getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const;

  // template <typename X, typename Y, typename... Z>
  // vk::StructureChain<X, Y, Z...>
  //   getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const;

  vk::ResultValueType<std::vector<vk::TimeDomainEXT>>::type getCalibrateableTimeDomainsEXT() const;

  vk::ResultValueType<std::vector<vk::CooperativeMatrixPropertiesNV>>::type getCooperativeMatrixPropertiesNV() const;

  vk::ResultValueType<std::vector<vk::DisplayPlanePropertiesKHR>>::type getDisplayPlanePropertiesKHR() const;

  vk::ResultValueType<std::vector<vk::DisplayPlaneProperties2KHR>>::type getDisplayPlaneProperties2KHR() const;

  vk::ResultValueType<std::vector<vk::DisplayPropertiesKHR>>::type getDisplayPropertiesKHR() const;

  vk::ResultValueType<std::vector<vk::DisplayProperties2KHR>>::type getDisplayProperties2KHR() const;

  vk::MultisamplePropertiesEXT getMultisamplePropertiesEXT(vk::SampleCountFlagBits samples) const;

  vk::ResultValueType<std::vector<vk::Rect2D>>::type getPresentRectanglesKHR(const vk::SurfaceKHR& surface) const;

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  vk::Bool32 getWaylandPresentationSupportKHR(uint32_t queueFamilyIndex, struct wl_display& display) const;
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
  vk::Bool32 getWin32PresentationSupportKHR(uint32_t queueFamilyIndex) const;
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
  vk::Bool32 getXcbPresentationSupportKHR(uint32_t queueFamilyIndex, xcb_connection_t& connection,
                                          xcb_visualid_t visual_id) const;
#endif

#ifdef VK_USE_PLATFORM_XLIB_KHR
  vk::Bool32 getXlibPresentationSupportKHR(uint32_t queueFamilyIndex, Display& dpy, VisualID visualID) const;
#endif

  // endregion

  // region Logi Declarations

  const std::shared_ptr<LogicalDeviceImpl>&
    createLogicalDevice(const vk::DeviceCreateInfo& createInfo,
                        const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyLogicalDevice(size_t id);

  VulkanInstanceImpl& getInstance() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  operator const vk::PhysicalDevice&() const;

 protected:
  /**
   * @brief Destroys all LogicalDevice handles.
   */
  void free() override;

  // endregion

 private:
  /**
   * Vulkan instance handle.
   */
  VulkanInstanceImpl& instance_;

  /**
   * Vulkan physical device handle.
   */
  vk::PhysicalDevice vkPhysicalDevice_;
};

// region Template Definitions

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getExternalBufferProperties(
  const vk::PhysicalDeviceExternalBufferInfo& externalBufferInfo) const {
  return vkPhysicalDevice_.getExternalBufferProperties<X, Y, Z...>(externalBufferInfo, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getExternalBufferPropertiesKHR(
  const vk::PhysicalDeviceExternalBufferInfoKHR& externalBufferInfo) const {
  return vkPhysicalDevice_.getExternalBufferPropertiesKHR<X, Y, Z...>(externalBufferInfo, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...>
  PhysicalDeviceImpl::getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& externalFenceInfo) const {
  return vkPhysicalDevice_.getExternalFenceProperties<X, Y, Z...>(externalFenceInfo, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getExternalFencePropertiesKHR(
  const vk::PhysicalDeviceExternalFenceInfoKHR& externalFenceInfo) const {
  return vkPhysicalDevice_.getExternalFencePropertiesKHR<X, Y, Z...>(externalFenceInfo, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getExternalSemaphoreProperties(
  const vk::PhysicalDeviceExternalSemaphoreInfo& externalSemaphoreInfo) const {
  return vkPhysicalDevice_.getExternalSemaphoreProperties<X, Y, Z...>(externalSemaphoreInfo, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getExternalSemaphorePropertiesKHR(
  const vk::PhysicalDeviceExternalSemaphoreInfoKHR& externalSemaphoreInfo) const {
  return vkPhysicalDevice_.getExternalSemaphorePropertiesKHR<X, Y, Z...>(externalSemaphoreInfo, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getFeatures2() const {
  return vkPhysicalDevice_.getFeatures2<X, Y, Z...>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getFeatures2KHR() const {
  return vkPhysicalDevice_.getFeatures2KHR<X, Y, Z...>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getFormatProperties2(vk::Format format) const {
  return vkPhysicalDevice_.getFormatProperties2<X, Y, Z...>(format, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getFormatProperties2KHR(vk::Format format) const {
  return vkPhysicalDevice_.getFormatProperties2KHR<X, Y, Z...>(format, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...>
  PhysicalDeviceImpl::getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& formatInfo) const {
  return vkPhysicalDevice_.getImageFormatProperties2<X, Y, Z...>(formatInfo, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...>
  PhysicalDeviceImpl::getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2KHR& formatInfo) const {
  return vkPhysicalDevice_.getImageFormatProperties2KHR<X, Y, Z...>(formatInfo, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getMemoryProperties2() const {
  return vkPhysicalDevice_.getMemoryProperties2<X, Y, Z...>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getMemoryProperties2KHR() const {
  return vkPhysicalDevice_.getMemoryProperties2KHR<X, Y, Z...>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getProperties2() const {
  return vkPhysicalDevice_.getProperties2<X, Y, Z...>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getProperties2KHR() const {
  return vkPhysicalDevice_.getProperties2KHR<X, Y, Z...>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
std::vector<vk::StructureChain<X, Y, Z...>> PhysicalDeviceImpl::getQueueFamilyProperties2() const {
  return vkPhysicalDevice_.getQueueFamilyProperties2<vk::StructureChain<X, Y, Z...>>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
std::vector<vk::StructureChain<X, Y, Z...>> PhysicalDeviceImpl::getQueueFamilyProperties2KHR() const {
  return vkPhysicalDevice_.getQueueFamilyProperties2KHR<vk::StructureChain<X, Y, Z...>>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
std::vector<vk::StructureChain<X, Y, Z...>> PhysicalDeviceImpl::getSparseImageFormatProperties2(
  const vk::PhysicalDeviceSparseImageFormatInfo2& pSparseImageFormatProperties) const {
  return vkPhysicalDevice_.getSparseImageFormatProperties2<vk::StructureChain<X, Y, Z...>>(pSparseImageFormatProperties,
                                                                                           getDispatcher());
}

template <typename X, typename Y, typename... Z>
std::vector<vk::StructureChain<X, Y, Z...>> PhysicalDeviceImpl::getSparseImageFormatProperties2KHR(
  const vk::PhysicalDeviceSparseImageFormatInfo2KHR& pSparseImageFormatProperties) const {
  return vkPhysicalDevice_.getSparseImageFormatProperties2KHR<vk::StructureChain<X, Y, Z...>>(
    pSparseImageFormatProperties, getDispatcher());
}

// template <typename X, typename Y, typename... Z>
// vk::StructureChain<X, Y, Z...>
//   PhysicalDeviceImpl::getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const {
//   return vkPhysicalDevice_.getGeneratedCommandsPropertiesNVX<X, Y, Z...>(features, getDispatcher());
// }

template <typename X, typename Y, typename... Z>
typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type
  PhysicalDeviceImpl::getSurfaceCapabilities2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const {
  return vkPhysicalDevice_.getSurfaceCapabilities2KHR<X, Y, Z...>(surfaceInfo, getDispatcher());
}

template <typename X, typename Y, typename... Z>
typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type
  PhysicalDeviceImpl::getSurfaceFormats2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const {
  return vkPhysicalDevice_.getSurfaceFormats2KHR<X, Y, Z...>(surfaceInfo, getDispatcher());
}

// endregion

} // namespace logi

#endif // LOGI_PHYSICAL_DEVICE_IMPL_HPP
