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

#include <string>
#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

/**
 * @brief Forward declaration for owner reference.
 */
class VulkanInstanceImpl;
class LogicalDevice;
class LogicalDeviceImpl;

class PhysicalDeviceImpl : public VulkanObject<PhysicalDeviceImpl>, public VulkanObjectComposite<LogicalDeviceImpl> {
 public:
  /**
   * @brief Vulkan physical device.
   *
   * @param instance            Instance handle
   * @param	vk_physical_device  Vulkan physical device handle.
   */
  explicit PhysicalDeviceImpl(VulkanInstanceImpl& instance, const vk::PhysicalDevice& vk_physical_device);

  typename vk::ResultValueType<std::vector<vk::ExtensionProperties>>::type
    enumerateDeviceExtensionProperties(const std::string* layer = nullptr) const;

  typename vk::ResultValueType<std::vector<vk::LayerProperties>>::type enumerateDeviceLayerProperties() const;

  vk::ExternalBufferProperties
    getExternalBufferProperties(const vk::PhysicalDeviceExternalBufferInfo& external_buffer_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalBufferProperties(const vk::PhysicalDeviceExternalBufferInfo& external_buffer_info) const {
    return vk_physical_device_.getExternalBufferProperties<X, Y, Z...>(external_buffer_info, getDispatcher());
  }

  vk::ExternalBufferPropertiesKHR
    getExternalBufferPropertiesKHR(const vk::PhysicalDeviceExternalBufferInfoKHR& external_buffer_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalBufferPropertiesKHR(const vk::PhysicalDeviceExternalBufferInfoKHR& external_buffer_info) const {
    return vk_physical_device_.getExternalBufferPropertiesKHR<X, Y, Z...>(external_buffer_info, getDispatcher());
  }

  vk::ExternalFenceProperties
    getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& external_fence_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& external_fence_info) const {
    return vk_physical_device_.getExternalFenceProperties<X, Y, Z...>(external_fence_info, getDispatcher());
  }

  vk::ExternalFencePropertiesKHR
    getExternalFencePropertiesKHR(const vk::PhysicalDeviceExternalFenceInfoKHR& external_fence_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalFencePropertiesKHR(const vk::PhysicalDeviceExternalFenceInfoKHR& external_fence_info) const {
    return vk_physical_device_.getExternalFencePropertiesKHR<X, Y, Z...>(external_fence_info, getDispatcher());
  }

  vk::ExternalSemaphoreProperties
    getExternalSemaphoreProperties(const vk::PhysicalDeviceExternalSemaphoreInfo& external_semaphore_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalSemaphoreProperties(const vk::PhysicalDeviceExternalSemaphoreInfo& external_semaphore_info) const {
    return vk_physical_device_.getExternalSemaphoreProperties<X, Y, Z...>(external_semaphore_info, getDispatcher());
  }

  vk::ExternalSemaphorePropertiesKHR
    getExternalSemaphorePropertiesKHR(const vk::PhysicalDeviceExternalSemaphoreInfoKHR& external_semaphore_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getExternalSemaphorePropertiesKHR(const vk::PhysicalDeviceExternalSemaphoreInfoKHR& external_semaphore_info) const {
    return vk_physical_device_.getExternalSemaphorePropertiesKHR<X, Y, Z...>(external_semaphore_info, getDispatcher());
  }

  vk::PhysicalDeviceFeatures getFeatures() const;

  vk::PhysicalDeviceFeatures2 getFeatures2() const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getFeatures2() const {
    return vk_physical_device_.getFeatures2<X, Y, Z...>(getDispatcher());
  }

  vk::PhysicalDeviceFeatures2KHR getFeatures2KHR() const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getFeatures2KHR() const {
    return vk_physical_device_.getFeatures2KHR<X, Y, Z...>(getDispatcher());
  }

  vk::FormatProperties getFormatProperties(vk::Format format) const;

  vk::FormatProperties2 getFormatProperties2(vk::Format format) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getFormatProperties2(vk::Format format) const {
    return vk_physical_device_.getFormatProperties2<X, Y, Z...>(format, getDispatcher());
  }

  vk::FormatProperties2KHR getFormatProperties2KHR(vk::Format format) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getFormatProperties2KHR(vk::Format format) const {
    return vk_physical_device_.getFormatProperties2KHR<X, Y, Z...>(format, getDispatcher());
  }

  vk::ImageFormatProperties getImageFormatProperties(vk::Format format, vk::ImageType type, vk::ImageTiling tiling,
                                                     const vk::ImageUsageFlags& usage,
                                                     const vk::ImageCreateFlags& flags) const;

  vk::ImageFormatProperties2 getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& format_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& format_info) const {
    return vk_physical_device_.getImageFormatProperties2<X, Y, Z...>(format_info, getDispatcher());
  }

  vk::ImageFormatProperties2KHR
    getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2& format_info) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2KHR& format_info) const {
    return vk_physical_device_.getImageFormatProperties2KHR<X, Y, Z...>(format_info, getDispatcher());
  }

  vk::PhysicalDeviceMemoryProperties getMemoryProperties() const;

  vk::PhysicalDeviceMemoryProperties2 getMemoryProperties2() const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getMemoryProperties2() const {
    return vk_physical_device_.getMemoryProperties2<X, Y, Z...>(getDispatcher());
  }

  vk::PhysicalDeviceMemoryProperties2KHR getMemoryProperties2KHR() const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getMemoryProperties2KHR() const {
    return vk_physical_device_.getMemoryProperties2KHR<X, Y, Z...>(getDispatcher());
  }

  vk::PhysicalDeviceProperties getProperties() const;

  vk::PhysicalDeviceProperties2 getProperties2() const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getProperties2() const {
    return vk_physical_device_.getProperties2<X, Y, Z...>(getDispatcher());
  }

  vk::PhysicalDeviceProperties2KHR getProperties2KHR() const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...> getProperties2KHR() const {
    return vk_physical_device_.getProperties2KHR<X, Y, Z...>(getDispatcher());
  }

  std::vector<vk::QueueFamilyProperties> getQueueFamilyProperties() const;

  std::vector<vk::QueueFamilyProperties2> getQueueFamilyProperties2() const;

  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>> getQueueFamilyProperties2() const {
    return vk_physical_device_.getQueueFamilyProperties2<vk::StructureChain<X, Y, Z...>>(getDispatcher());
  }

  std::vector<vk::QueueFamilyProperties2KHR> getQueueFamilyProperties2KHR() const;

  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>> getQueueFamilyProperties2KHR() const {
    return vk_physical_device_.getQueueFamilyProperties2KHR<vk::StructureChain<X, Y, Z...>>(getDispatcher());
  }

  std::vector<vk::SparseImageFormatProperties> getSparseImageFormatProperties(vk::Format format, vk::ImageType type,
                                                                              vk::SampleCountFlagBits samples,
                                                                              const vk::ImageUsageFlags& usage,
                                                                              vk::ImageTiling tiling) const;

  std::vector<vk::SparseImageFormatProperties2> getSparseImageFormatProperties2(
    const vk::PhysicalDeviceSparseImageFormatInfo2& p_sparse_image_format_properties) const;

  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>> getSparseImageFormatProperties2(
    const vk::PhysicalDeviceSparseImageFormatInfo2& p_sparse_image_format_properties) const {
    return vk_physical_device_.getSparseImageFormatProperties2<vk::StructureChain<X, Y, Z...>>(
      p_sparse_image_format_properties, getDispatcher());
  }

  std::vector<vk::SparseImageFormatProperties2KHR> getSparseImageFormatProperties2KHR(
    const vk::PhysicalDeviceSparseImageFormatInfo2KHR& p_sparse_image_format_properties) const;

  template <typename X, typename Y, typename... Z>
  std::vector<vk::StructureChain<X, Y, Z...>> getSparseImageFormatProperties2KHR(
    const vk::PhysicalDeviceSparseImageFormatInfo2KHR& p_sparse_image_format_properties) const {
    return vk_physical_device_.getSparseImageFormatProperties2KHR<vk::StructureChain<X, Y, Z...>>(
      p_sparse_image_format_properties, getDispatcher());
  }

  vk::ExternalImageFormatPropertiesNV
    getExternalImageFormatPropertiesNV(vk::Format format, vk::ImageType type, vk::ImageTiling tiling,
                                       const vk::ImageUsageFlags& usage, const vk::ImageCreateFlags& flags,
                                       const vk::ExternalMemoryHandleTypeFlagsNV& external_handle_type) const;

  vk::DeviceGeneratedCommandsLimitsNVX
    getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const;

  template <typename X, typename Y, typename... Z>
  vk::StructureChain<X, Y, Z...>
    getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const {
    return vk_physical_device_.getGeneratedCommandsPropertiesNVX<X, Y, Z...>(features, getDispatcher());
  }

  LogicalDevice createLogicalDevice(const vk::DeviceCreateInfo& create_info,
                                    const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyLogicalDevice(size_t id);

  /**
   * @brief   Retrieve reference to the instance implementation.
   *
   * @return  Reference to the instance implementation.
   */
  VulkanInstanceImpl& getInstance() const;

  /**
   * @brief   Retrieve instance dispatcher.
   *
   * @return  Instance dispatcher.
   */
  const vk::DispatchLoaderDynamic& getDispatcher() const;

  /**
   * @brief   Implicit conversion to vk::PhysicalDevice.
   *
   * @return  vk::PhysicalDevice handle.
   */
  operator vk::PhysicalDevice() const;

 protected:
  /**
   * @brief Destroys all LogicalDevice handles.
   */
  void free() override;

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

} // namespace logi

#endif // LOGI_PHYSICAL_DEVICE_IMPL_HPP
