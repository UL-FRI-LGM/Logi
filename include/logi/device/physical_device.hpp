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

#ifndef LOGI_BASE_PHYSICAL_DEVICE_HPP
#define LOGI_BASE_PHYSICAL_DEVICE_HPP

#include "logi/base/handle.hpp"
#include "logi/device/logical_device.hpp"
#include "logi/device/physical_device_impl.hpp"

namespace logi {

class VulkanInstance;

class PhysicalDevice : public Handle<PhysicalDeviceImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkEnumerateDeviceExtensionProperties.html">vkEnumerateDeviceExtensionProperties</a>
   */
  typename vk::ResultValueType<std::vector<vk::ExtensionProperties>>::type
    enumerateDeviceExtensionProperties(const std::string* layer = nullptr) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkEnumerateDeviceLayerProperties.html">vkEnumerateDeviceLayerProperties</a>
   */
  typename vk::ResultValueType<std::vector<vk::LayerProperties>>::type enumerateDeviceLayerProperties() const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalBufferProperties.html">vkGetPhysicalDeviceExternalBufferProperties</a>
   */  
  vk::ExternalBufferProperties
    getExternalBufferProperties(const vk::PhysicalDeviceExternalBufferInfo& externalBufferInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalBufferProperties.html">vkGetPhysicalDeviceExternalBufferProperties</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalBufferProperties(const vk::PhysicalDeviceExternalBufferInfo& externalBufferInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalBufferPropertiesKHR.html">vkGetPhysicalDeviceExternalBufferPropertiesKHR</a>
   */
  vk::ExternalBufferPropertiesKHR
    getExternalBufferPropertiesKHR(const vk::PhysicalDeviceExternalBufferInfoKHR& externalBufferInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalBufferPropertiesKHR.html">vkGetPhysicalDeviceExternalBufferPropertiesKHR</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalBufferPropertiesKHR(const vk::PhysicalDeviceExternalBufferInfoKHR& externalBufferInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalFenceProperties.html">vkGetPhysicalDeviceExternalFenceProperties</a>
   */
  vk::ExternalFenceProperties
    getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& externalFenceInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalFenceProperties.html">vkGetPhysicalDeviceExternalFenceProperties</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& externalFenceInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalFencePropertiesKHR.html">vkGetPhysicalDeviceExternalFencePropertiesKHR</a>
   */
  vk::ExternalFencePropertiesKHR
    getExternalFencePropertiesKHR(const vk::PhysicalDeviceExternalFenceInfoKHR& externalFenceInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalFencePropertiesKHR.html">vkGetPhysicalDeviceExternalFencePropertiesKHR</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalFencePropertiesKHR(const vk::PhysicalDeviceExternalFenceInfoKHR& externalFenceInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalSemaphoreProperties.html">vkGetPhysicalDeviceExternalSemaphoreProperties</a>
   */
  vk::ExternalSemaphoreProperties
    getExternalSemaphoreProperties(const vk::PhysicalDeviceExternalSemaphoreInfo& externalSemaphoreInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalSemaphoreProperties.html">vkGetPhysicalDeviceExternalSemaphoreProperties</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalSemaphoreProperties(const vk::PhysicalDeviceExternalSemaphoreInfo& externalSemaphoreInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalSemaphorePropertiesKHR.html">vkGetPhysicalDeviceExternalSemaphorePropertiesKHR</a>
   */
  vk::ExternalSemaphorePropertiesKHR
    getExternalSemaphorePropertiesKHR(const vk::PhysicalDeviceExternalSemaphoreInfoKHR& externalSemaphoreInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalSemaphorePropertiesKHR.html">vkGetPhysicalDeviceExternalSemaphorePropertiesKHR</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalSemaphorePropertiesKHR(const vk::PhysicalDeviceExternalSemaphoreInfoKHR& externalSemaphoreInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceFeatures.html">vkGetPhysicalDeviceFeatures</a>
   */
  vk::PhysicalDeviceFeatures getFeatures() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceFeatures2.html">vkGetPhysicalDeviceFeatures2</a>
   */
  vk::PhysicalDeviceFeatures2 getFeatures2() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceFeatures2.html">vkGetPhysicalDeviceFeatures2</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getFeatures2() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceFeatures2KHR.html">vkGetPhysicalDeviceFeatures2KHR</a>
   */
  vk::PhysicalDeviceFeatures2KHR getFeatures2KHR() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceFeatures2KHR.html">vkGetPhysicalDeviceFeatures2KHR</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getFeatures2KHR() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceFormatProperties.html">vkGetPhysicalDeviceFormatProperties</a>
   */
  vk::FormatProperties getFormatProperties(vk::Format format) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceFormatProperties2.html">vkGetPhysicalDeviceFormatProperties2</a>
   */
  vk::FormatProperties2 getFormatProperties2(vk::Format format) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceFormatProperties2.html">vkGetPhysicalDeviceFormatProperties2</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getFormatProperties2(vk::Format format) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceFormatProperties2KHR.html">vkGetPhysicalDeviceFormatProperties2KHR</a>
   */
  vk::FormatProperties2KHR getFormatProperties2KHR(vk::Format format) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceFormatProperties2KHR.html">vkGetPhysicalDeviceFormatProperties2KHR</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getFormatProperties2KHR(vk::Format format) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceImageFormatProperties.html">vkGetPhysicalDeviceImageFormatProperties</a>
   */
  vk::ImageFormatProperties getImageFormatProperties(vk::Format format, vk::ImageType type, vk::ImageTiling tiling,
                                                     const vk::ImageUsageFlags& usage,
                                                     const vk::ImageCreateFlags& flags) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceImageFormatProperties2.html">vkGetPhysicalDeviceImageFormatProperties2</a>
   */
  vk::ImageFormatProperties2 getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& formatInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceImageFormatProperties2.html">vkGetPhysicalDeviceImageFormatProperties2</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& formatInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceImageFormatProperties2KHR.html">vkGetPhysicalDeviceImageFormatProperties2KHR</a>
   */
  vk::ImageFormatProperties2KHR
    getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2& formatInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceImageFormatProperties2KHR.html">vkGetPhysicalDeviceImageFormatProperties2KHR</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2KHR& formatInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceMemoryProperties.html">vkGetPhysicalDeviceMemoryProperties</a>
   */
  vk::PhysicalDeviceMemoryProperties getMemoryProperties() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceMemoryProperties2.html">vkGetPhysicalDeviceMemoryProperties2</a>
   */
  vk::PhysicalDeviceMemoryProperties2 getMemoryProperties2() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceMemoryProperties2.html">vkGetPhysicalDeviceMemoryProperties2</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getMemoryProperties2() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceMemoryProperties2KHR.html">vkGetPhysicalDeviceMemoryProperties2KHR</a>
   */
  vk::PhysicalDeviceMemoryProperties2KHR getMemoryProperties2KHR() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceMemoryProperties2KHR.html">vkGetPhysicalDeviceMemoryProperties2KHR</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getMemoryProperties2KHR() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceProperties.html">vkGetPhysicalDeviceProperties</a>
   */
  vk::PhysicalDeviceProperties getProperties() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceProperties2.html">vkGetPhysicalDeviceProperties2</a>
   */
  vk::PhysicalDeviceProperties2 getProperties2() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceProperties2.html">vkGetPhysicalDeviceProperties2</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getProperties2() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceProperties2KHR.html">vkGetPhysicalDeviceProperties2KHR</a>
   */
  vk::PhysicalDeviceProperties2KHR getProperties2KHR() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceProperties2KHR.html">vkGetPhysicalDeviceProperties2KHR</a>
   */
  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getProperties2KHR() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceQueueFamilyProperties.html">vkGetPhysicalDeviceQueueFamilyProperties</a>
   */
  std::vector<vk::QueueFamilyProperties> getQueueFamilyProperties() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceQueueFamilyProperties2.html">vkGetPhysicalDeviceQueueFamilyProperties2</a>
   */
  std::vector<vk::QueueFamilyProperties2> getQueueFamilyProperties2() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceQueueFamilyProperties2.html">vkGetPhysicalDeviceQueueFamilyProperties2</a>
   */
  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>> getQueueFamilyProperties2() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceQueueFamilyProperties2KHR.html">vkGetPhysicalDeviceQueueFamilyProperties2KHR</a>
   */
  std::vector<vk::QueueFamilyProperties2KHR> getQueueFamilyProperties2KHR() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceQueueFamilyProperties2KHR.html">vkGetPhysicalDeviceQueueFamilyProperties2KHR</a>
   */
  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>> getQueueFamilyProperties2KHR() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSparseImageFormatProperties.html">vkGetPhysicalDeviceSparseImageFormatProperties</a>
   */
  std::vector<vk::SparseImageFormatProperties> getSparseImageFormatProperties(vk::Format format, vk::ImageType type,
                                                                              vk::SampleCountFlagBits samples,
                                                                              const vk::ImageUsageFlags& usage,
                                                                              vk::ImageTiling tiling) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSparseImageFormatProperties2.html">vkGetPhysicalDeviceSparseImageFormatProperties2</a>
   */
  std::vector<vk::SparseImageFormatProperties2>
    getSparseImageFormatProperties2(const vk::PhysicalDeviceSparseImageFormatInfo2& sparseImageFormatProperties) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSparseImageFormatProperties2.html">vkGetPhysicalDeviceSparseImageFormatProperties2</a>
   */
  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>>
    getSparseImageFormatProperties2(const vk::PhysicalDeviceSparseImageFormatInfo2& sparseImageFormatProperties) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSparseImageFormatProperties2KHR.html">vkGetPhysicalDeviceSparseImageFormatProperties2KHR</a>
   */
  std::vector<vk::SparseImageFormatProperties2KHR> getSparseImageFormatProperties2KHR(
    const vk::PhysicalDeviceSparseImageFormatInfo2KHR& sparseImageFormatProperties) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSparseImageFormatProperties2KHR.html">vkGetPhysicalDeviceSparseImageFormatProperties2KHR</a>
   */
  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>> getSparseImageFormatProperties2KHR(
    const vk::PhysicalDeviceSparseImageFormatInfo2KHR& sparseImageFormatProperties) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSurfaceCapabilitiesKHR.html">vkGetPhysicalDeviceSurfaceCapabilitiesKHR</a>
   */
  vk::ResultValueType<vk::SurfaceCapabilitiesKHR>::type getSurfaceCapabilitiesKHR(vk::SurfaceKHR surface) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSurfaceCapabilities2KHR.html">vkGetPhysicalDeviceSurfaceCapabilities2KHR</a>
   */
  template <typename X, typename Y, typename... Z>
  typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type
    getSurfaceCapabilities2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSurfaceCapabilities2KHR.html">vkGetPhysicalDeviceSurfaceCapabilities2KHR</a>
   */
  vk::ResultValueType<vk::SurfaceCapabilities2KHR>::type
    getSurfaceCapabilities2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSurfaceFormatsKHR.html">vkGetPhysicalDeviceSurfaceFormatsKHR</a>
   */
  vk::ResultValueType<std::vector<vk::SurfaceFormatKHR>>::type getSurfaceFormatsKHR(vk::SurfaceKHR surface) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSurfaceFormats2KHR.html">vkGetPhysicalDeviceSurfaceFormats2KHR</a>
   */
  template <typename X, typename Y, typename... Z>
  typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type
    getSurfaceFormats2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSurfaceFormats2KHR.html">vkGetPhysicalDeviceSurfaceFormats2KHR</a>
   */
  vk::ResultValueType<std::vector<vk::SurfaceFormat2KHR>>::type
    getSurfaceFormats2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSurfacePresentModesKHR.html">vkGetPhysicalDeviceSurfacePresentModesKHR</a>
   */
  vk::ResultValueType<std::vector<vk::PresentModeKHR>>::type getSurfacePresentModesKHR(vk::SurfaceKHR surface) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceSurfaceSupportKHR.html">vkGetPhysicalDeviceSurfaceSupportKHR</a>
   */
  vk::ResultValueType<vk::Bool32>::type getSurfaceSupportKHR(uint32_t queueFamilyIndex, vk::SurfaceKHR surface) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceExternalImageFormatPropertiesNV.html">vkGetPhysicalDeviceExternalImageFormatPropertiesNV</a>
   */
  vk::ExternalImageFormatPropertiesNV
    getExternalImageFormatPropertiesNV(vk::Format format, vk::ImageType type, vk::ImageTiling tiling,
                                       const vk::ImageUsageFlags& usage, const vk::ImageCreateFlags& flags,
                                       const vk::ExternalMemoryHandleTypeFlagsNV& externalHandleType) const;


  // Deprecated
  // vk::DeviceGeneratedCommandsLimitsNVX
  //   getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const;

  // template <typename X, typename Y, typename... Z>
  // vk::StructureChain<X, Y, Z...>
  //   getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const;


  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceCalibrateableTimeDomainsEXT.html">vkGetPhysicalDeviceCalibrateableTimeDomainsEXT</a>
   */
  vk::ResultValueType<std::vector<vk::TimeDomainEXT>>::type getCalibrateableTimeDomainsEXT() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceCooperativeMatrixPropertiesNV.html">vkGetPhysicalDeviceCooperativeMatrixPropertiesNV</a>
   */
  vk::ResultValueType<std::vector<vk::CooperativeMatrixPropertiesNV>>::type getCooperativeMatrixPropertiesNV() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceDisplayPlanePropertiesKHR.html">vkGetPhysicalDeviceDisplayPlanePropertiesKHR</a>
   */
  vk::ResultValueType<std::vector<vk::DisplayPlanePropertiesKHR>>::type getDisplayPlanePropertiesKHR() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceDisplayPlaneProperties2KHR.html">vkGetPhysicalDeviceDisplayPlaneProperties2KHR</a>
   */
  vk::ResultValueType<std::vector<vk::DisplayPlaneProperties2KHR>>::type getDisplayPlaneProperties2KHR() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceDisplayPropertiesKHR.html">vkGetPhysicalDeviceDisplayPropertiesKHR</a>
   */
  vk::ResultValueType<std::vector<vk::DisplayPropertiesKHR>>::type getDisplayPropertiesKHR() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceDisplayProperties2KHR.html">vkGetPhysicalDeviceDisplayProperties2KHR</a>
   */
  vk::ResultValueType<std::vector<vk::DisplayProperties2KHR>>::type getDisplayProperties2KHR() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceMultisamplePropertiesEXT.html">vkGetPhysicalDeviceMultisamplePropertiesEXT</a>
   */
  vk::MultisamplePropertiesEXT getMultisamplePropertiesEXT(vk::SampleCountFlagBits samples) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDevicePresentRectanglesKHR.html">vkGetPhysicalDevicePresentRectanglesKHR</a>
   */
  vk::ResultValueType<std::vector<vk::Rect2D>>::type getPresentRectanglesKHR(const vk::SurfaceKHR& surface) const;

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceWaylandPresentationSupportKHR.html">vkGetPhysicalDeviceWaylandPresentationSupportKHR</a>
   */
  vk::Bool32 getWaylandPresentationSupportKHR(uint32_t queueFamilyIndex, struct wl_display& display) const;
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceWin32PresentationSupportKHR.html">vkGetPhysicalDeviceWin32PresentationSupportKHR</a>
   */
  vk::Bool32 getWin32PresentationSupportKHR(uint32_t queueFamilyIndex) const;
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceXcbPresentationSupportKHR.html">vkGetPhysicalDeviceXcbPresentationSupportKHR</a>
   */
  vk::Bool32 getXcbPresentationSupportKHR(uint32_t queueFamilyIndex, xcb_connection_t& connection,
                                          xcb_visualid_t visual_id) const;
#endif

#ifdef VK_USE_PLATFORM_XLIB_KHR
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceXlibPresentationSupportKHR.html">vkGetPhysicalDeviceXlibPresentationSupportKHR</a>
   */
  vk::Bool32 getXlibPresentationSupportKHR(uint32_t queueFamilyIndex, Display& dpy, VisualID visualID) const;
#endif

  // endregion Vulkan Declarations

  // region Logi Declarations

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateDevice.html">vkCreateDevice</a>
   */
  LogicalDevice createLogicalDevice(const vk::DeviceCreateInfo& createInfo,
                                    const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyDevice.html">vkDestroyDevice</a>
   */
  void destroyLogicalDevice(const LogicalDevice& device) const;

  VulkanInstance getInstance() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  operator const vk::PhysicalDevice&() const;

  // endregion
};

// region Template Definitions

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...>
  PhysicalDevice::getExternalBufferProperties(const vk::PhysicalDeviceExternalBufferInfo& externalBufferInfo) const {
  return object_->getExternalBufferProperties<X, Y, Z...>(externalBufferInfo);
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDevice::getExternalBufferPropertiesKHR(
  const vk::PhysicalDeviceExternalBufferInfoKHR& externalBufferInfo) const {
  return object_->getExternalBufferPropertiesKHR<X, Y, Z...>(externalBufferInfo);
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...>
  PhysicalDevice::getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& externalFenceInfo) const {
  return object_->getExternalFenceProperties<X, Y, Z...>(externalFenceInfo);
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...>
  PhysicalDevice::getExternalFencePropertiesKHR(const vk::PhysicalDeviceExternalFenceInfoKHR& externalFenceInfo) const {
  return object_->getExternalFencePropertiesKHR<X, Y, Z...>(externalFenceInfo);
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDevice::getExternalSemaphoreProperties(
  const vk::PhysicalDeviceExternalSemaphoreInfo& externalSemaphoreInfo) const {
  return object_->getExternalSemaphoreProperties<X, Y, Z...>(externalSemaphoreInfo);
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDevice::getExternalSemaphorePropertiesKHR(
  const vk::PhysicalDeviceExternalSemaphoreInfoKHR& externalSemaphoreInfo) const {
  return object_->getExternalSemaphorePropertiesKHR<X, Y, Z...>(externalSemaphoreInfo);
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDevice::getFeatures2() const {
  return object_->getFeatures2<X, Y, Z...>();
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDevice::getFeatures2KHR() const {
  return object_->getFeatures2KHR<X, Y, Z...>();
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDevice::getFormatProperties2(vk::Format format) const {
  return object_->getFormatProperties2<X, Y, Z...>(format);
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDevice::getFormatProperties2KHR(vk::Format format) const {
  return object_->getFormatProperties2KHR<X, Y, Z...>(format);
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...>
  PhysicalDevice::getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& formatInfo) const {
  return object_->getImageFormatProperties2<X, Y, Z...>(formatInfo);
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...>
  PhysicalDevice::getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2KHR& formatInfo) const {
  return object_->getImageFormatProperties2KHR<X, Y, Z...>(formatInfo);
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDevice::getMemoryProperties2() const {
  return object_->getMemoryProperties2<X, Y, Z...>();
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDevice::getMemoryProperties2KHR() const {
  return object_->getMemoryProperties2KHR<X, Y, Z...>();
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDevice::getProperties2() const {
  return object_->getProperties2<X, Y, Z...>();
}

template <typename X, typename Y, typename... Z>
vk::StructureChain<X, Y, Z...> PhysicalDevice::getProperties2KHR() const {
  return object_->getProperties2KHR<X, Y, Z...>();
}

template <typename X, typename Y, typename... Z>
std::vector<vk::StructureChain<X, Y, Z...>> PhysicalDevice::getQueueFamilyProperties2() const {
  return object_->getQueueFamilyProperties2<X, Y, Z...>();
}

template <typename X, typename Y, typename... Z>
std::vector<vk::StructureChain<X, Y, Z...>> PhysicalDevice::getQueueFamilyProperties2KHR() const {
  return object_->getQueueFamilyProperties2KHR<X, Y, Z...>();
}

template <typename X, typename Y, typename... Z>
std::vector<vk::StructureChain<X, Y, Z...>> PhysicalDevice::getSparseImageFormatProperties2(
  const vk::PhysicalDeviceSparseImageFormatInfo2& sparseImageFormatProperties) const {
  return object_->getSparseImageFormatProperties2<X, Y, Z...>(sparseImageFormatProperties);
}

template <typename X, typename Y, typename... Z>
std::vector<vk::StructureChain<X, Y, Z...>> PhysicalDevice::getSparseImageFormatProperties2KHR(
  const vk::PhysicalDeviceSparseImageFormatInfo2KHR& sparseImageFormatProperties) const {
  return object_->getSparseImageFormatProperties2KHR<X, Y, Z...>(sparseImageFormatProperties);
}

template <typename X, typename Y, typename... Z>
typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type
  PhysicalDevice::getSurfaceCapabilities2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const {
  return object_->getSurfaceCapabilities2KHR(surfaceInfo);
}

template <typename X, typename Y, typename... Z>
typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type
  PhysicalDevice::getSurfaceFormats2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const {
  return object_->getSurfaceFormats2KHR(surfaceInfo);
}

// Deprecated
// template <typename X, typename Y, typename... Z>
// vk::StructureChain<X, Y, Z...>
//   PhysicalDevice::getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const {
//   return object_->getGeneratedCommandsPropertiesNVX<X, Y, Z...>(features);
// }

// endregion

} // namespace logi

#endif // LOGI_BASE_PHYSICAL_DEVICE_HPP
