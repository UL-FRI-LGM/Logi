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
#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

/**
 * @brief Forward declaration for owner reference.
 */
class VulkanInstanceImpl;
class LogicalDeviceImpl;
class LogicalDevice;

class PhysicalDeviceImpl : public VulkanObject<PhysicalDeviceImpl>, public VulkanObjectComposite<LogicalDeviceImpl> {
 public:
  /**
   * @brief Vulkan physical device.
   *
   * @param instance            Instance handle
   * @param	vk_physical_device  Vulkan physical device handle.
   */
  explicit PhysicalDeviceImpl(VulkanInstanceImpl& instance, const vk::PhysicalDevice& vk_physical_device);

  // region Vulkan Declarations

  typename vk::ResultValueType<std::vector<vk::ExtensionProperties>>::type
    enumerateDeviceExtensionProperties(const std::string* layer = nullptr) const;

  typename vk::ResultValueType<std::vector<vk::LayerProperties>>::type enumerateDeviceLayerProperties() const;

  vk::ExternalBufferProperties
    getExternalBufferProperties(const vk::PhysicalDeviceExternalBufferInfo& external_buffer_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalBufferProperties(const vk::PhysicalDeviceExternalBufferInfo& external_buffer_info) const;

  vk::ExternalBufferPropertiesKHR
    getExternalBufferPropertiesKHR(const vk::PhysicalDeviceExternalBufferInfoKHR& external_buffer_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalBufferPropertiesKHR(const vk::PhysicalDeviceExternalBufferInfoKHR& external_buffer_info) const;

  vk::ExternalFenceProperties
    getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& external_fence_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& external_fence_info) const;

  vk::ExternalFencePropertiesKHR
    getExternalFencePropertiesKHR(const vk::PhysicalDeviceExternalFenceInfoKHR& external_fence_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalFencePropertiesKHR(const vk::PhysicalDeviceExternalFenceInfoKHR& external_fence_info) const;

  vk::ExternalSemaphoreProperties
    getExternalSemaphoreProperties(const vk::PhysicalDeviceExternalSemaphoreInfo& external_semaphore_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalSemaphoreProperties(const vk::PhysicalDeviceExternalSemaphoreInfo& external_semaphore_info) const;

  vk::ExternalSemaphorePropertiesKHR
    getExternalSemaphorePropertiesKHR(const vk::PhysicalDeviceExternalSemaphoreInfoKHR& external_semaphore_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalSemaphorePropertiesKHR(const vk::PhysicalDeviceExternalSemaphoreInfoKHR& external_semaphore_info) const;

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

  vk::ImageFormatProperties2 getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& format_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& format_info) const;

  vk::ImageFormatProperties2KHR
    getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2& format_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2KHR& format_info) const;

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

  std::vector<vk::SparseImageFormatProperties2> getSparseImageFormatProperties2(
    const vk::PhysicalDeviceSparseImageFormatInfo2& p_sparse_image_format_properties) const;

  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>> getSparseImageFormatProperties2(
    const vk::PhysicalDeviceSparseImageFormatInfo2& p_sparse_image_format_properties) const;

  std::vector<vk::SparseImageFormatProperties2KHR> getSparseImageFormatProperties2KHR(
    const vk::PhysicalDeviceSparseImageFormatInfo2KHR& p_sparse_image_format_properties) const;

  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>> getSparseImageFormatProperties2KHR(
    const vk::PhysicalDeviceSparseImageFormatInfo2KHR& p_sparse_image_format_properties) const;

  vk::ResultValueType<std::vector<vk::Rect2D>>::type getPresentRectanglesKHR(vk::SurfaceKHR surface) const;

  vk::ResultValueType<vk::SurfaceCapabilitiesKHR>::type getSurfaceCapabilitiesKHR(vk::SurfaceKHR surface) const;

  template <typename X, typename Y, typename... Z>
  typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type
    getSurfaceCapabilities2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surface_info) const;

  vk::ResultValueType<vk::SurfaceCapabilities2KHR>::type
    getSurfaceCapabilities2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surface_info) const;

  vk::ResultValueType<std::vector<vk::SurfaceFormatKHR>>::type getSurfaceFormatsKHR(vk::SurfaceKHR surface) const;

  template <typename X, typename Y, typename... Z>
  typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type
    getSurfaceFormats2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surface_info) const;

  vk::ResultValueType<std::vector<vk::SurfaceFormat2KHR>>::type
    getSurfaceFormats2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surface_info) const;

  vk::ResultValueType<std::vector<vk::PresentModeKHR>>::type getSurfacePresentModesKHR(vk::SurfaceKHR surface) const;

  vk::ResultValueType<vk::Bool32>::type getSurfaceSupportKHR(uint32_t queue_index, vk::SurfaceKHR surface) const;

  vk::ExternalImageFormatPropertiesNV
    getExternalImageFormatPropertiesNV(vk::Format format, vk::ImageType type, vk::ImageTiling tiling,
                                       const vk::ImageUsageFlags& usage, const vk::ImageCreateFlags& flags,
                                       const vk::ExternalMemoryHandleTypeFlagsNV& external_handle_type) const;

  vk::DeviceGeneratedCommandsLimitsNVX
    getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const;

  // endregion

  // region Logi Declarations

  LogicalDevice createLogicalDevice(const vk::DeviceCreateInfo& create_info,
                                    const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyLogicalDevice(size_t id);

  VulkanInstanceImpl& getInstance() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  operator vk::PhysicalDevice() const;

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
  vk::PhysicalDevice vk_physical_device_;
};

// region Template Definitions

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getExternalBufferProperties(
  const vk::PhysicalDeviceExternalBufferInfo& external_buffer_info) const {
  return vk_physical_device_.getExternalBufferProperties<X, Y, Z...>(external_buffer_info, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getExternalBufferPropertiesKHR(
  const vk::PhysicalDeviceExternalBufferInfoKHR& external_buffer_info) const {
  return vk_physical_device_.getExternalBufferPropertiesKHR<X, Y, Z...>(external_buffer_info, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...>
  PhysicalDeviceImpl::getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& external_fence_info) const {
  return vk_physical_device_.getExternalFenceProperties<X, Y, Z...>(external_fence_info, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getExternalFencePropertiesKHR(
  const vk::PhysicalDeviceExternalFenceInfoKHR& external_fence_info) const {
  return vk_physical_device_.getExternalFencePropertiesKHR<X, Y, Z...>(external_fence_info, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getExternalSemaphoreProperties(
  const vk::PhysicalDeviceExternalSemaphoreInfo& external_semaphore_info) const {
  return vk_physical_device_.getExternalSemaphoreProperties<X, Y, Z...>(external_semaphore_info, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getExternalSemaphorePropertiesKHR(
  const vk::PhysicalDeviceExternalSemaphoreInfoKHR& external_semaphore_info) const {
  return vk_physical_device_.getExternalSemaphorePropertiesKHR<X, Y, Z...>(external_semaphore_info, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getFeatures2() const {
  return vk_physical_device_.getFeatures2<X, Y, Z...>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getFeatures2KHR() const {
  return vk_physical_device_.getFeatures2KHR<X, Y, Z...>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getFormatProperties2(vk::Format format) const {
  return vk_physical_device_.getFormatProperties2<X, Y, Z...>(format, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getFormatProperties2KHR(vk::Format format) const {
  return vk_physical_device_.getFormatProperties2KHR<X, Y, Z...>(format, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...>
  PhysicalDeviceImpl::getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& format_info) const {
  return vk_physical_device_.getImageFormatProperties2<X, Y, Z...>(format_info, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...>
  PhysicalDeviceImpl::getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2KHR& format_info) const {
  return vk_physical_device_.getImageFormatProperties2KHR<X, Y, Z...>(format_info, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getMemoryProperties2() const {
  return vk_physical_device_.getMemoryProperties2<X, Y, Z...>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getMemoryProperties2KHR() const {
  return vk_physical_device_.getMemoryProperties2KHR<X, Y, Z...>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getProperties2() const {
  return vk_physical_device_.getProperties2<X, Y, Z...>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDeviceImpl::getProperties2KHR() const {
  return vk_physical_device_.getProperties2KHR<X, Y, Z...>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
std::vector<vk::StructureChain<X, Y, Z...>> PhysicalDeviceImpl::getQueueFamilyProperties2() const {
  return vk_physical_device_.getQueueFamilyProperties2<vk::StructureChain<X, Y, Z...>>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
std::vector<vk::StructureChain<X, Y, Z...>> PhysicalDeviceImpl::getQueueFamilyProperties2KHR() const {
  return vk_physical_device_.getQueueFamilyProperties2KHR<vk::StructureChain<X, Y, Z...>>(getDispatcher());
}

template <typename X, typename Y, typename... Z>
std::vector<vk::StructureChain<X, Y, Z...>> PhysicalDeviceImpl::getSparseImageFormatProperties2(
  const vk::PhysicalDeviceSparseImageFormatInfo2& p_sparse_image_format_properties) const {
  return vk_physical_device_.getSparseImageFormatProperties2<vk::StructureChain<X, Y, Z...>>(
    p_sparse_image_format_properties, getDispatcher());
}

template <typename X, typename Y, typename... Z>
std::vector<vk::StructureChain<X, Y, Z...>> PhysicalDeviceImpl::getSparseImageFormatProperties2KHR(
  const vk::PhysicalDeviceSparseImageFormatInfo2KHR& p_sparse_image_format_properties) const {
  return vk_physical_device_.getSparseImageFormatProperties2KHR<vk::StructureChain<X, Y, Z...>>(
    p_sparse_image_format_properties, getDispatcher());
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...>
  PhysicalDeviceImpl::getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const {
  return vk_physical_device_.getGeneratedCommandsPropertiesNVX<X, Y, Z...>(features, getDispatcher());
}

template <typename X, typename Y, typename... Z>
typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type
  PhysicalDeviceImpl::getSurfaceCapabilities2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surface_info) const {
  return vk_physical_device_.getSurfaceCapabilities2KHR<X, Y, Z...>(surface_info, getDispatcher());
}

template <typename X, typename Y, typename... Z>
typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type
  PhysicalDeviceImpl::getSurfaceFormats2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surface_info) const {
  return vk_physical_device_.getSurfaceFormats2KHR<X, Y, Z...>(surface_info, getDispatcher());
}

// endregion

} // namespace logi

#endif // LOGI_PHYSICAL_DEVICE_IMPL_HPP
