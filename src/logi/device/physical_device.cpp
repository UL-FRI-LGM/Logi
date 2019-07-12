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

#include "logi/device/physical_device.hpp"
#include "logi/device/logical_device.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"

namespace logi {

typename vk::ResultValueType<std::vector<vk::ExtensionProperties>>::type
  PhysicalDevice::enumerateDeviceExtensionProperties(const std::string* layer) const {
  return object_->enumerateDeviceExtensionProperties(layer);
}

typename vk::ResultValueType<std::vector<vk::LayerProperties>>::type
  PhysicalDevice::enumerateDeviceLayerProperties() const {
  return object_->enumerateDeviceLayerProperties();
}

vk::ExternalBufferProperties
  PhysicalDevice::getExternalBufferProperties(const vk::PhysicalDeviceExternalBufferInfo& externalBufferInfo) const {
  return object_->getExternalBufferProperties(externalBufferInfo);
}

vk::ExternalBufferPropertiesKHR PhysicalDevice::getExternalBufferPropertiesKHR(
  const vk::PhysicalDeviceExternalBufferInfoKHR& externalBufferInfo) const {
  return object_->getExternalBufferPropertiesKHR(externalBufferInfo);
}

vk::ExternalFenceProperties
  PhysicalDevice::getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& externalFenceInfo) const {
  return object_->getExternalFenceProperties(externalFenceInfo);
}

vk::ExternalFencePropertiesKHR
  PhysicalDevice::getExternalFencePropertiesKHR(const vk::PhysicalDeviceExternalFenceInfoKHR& externalFenceInfo) const {
  return object_->getExternalFencePropertiesKHR(externalFenceInfo);
}

vk::ExternalSemaphoreProperties PhysicalDevice::getExternalSemaphoreProperties(
  const vk::PhysicalDeviceExternalSemaphoreInfo& externalSemaphoreInfo) const {
  return object_->getExternalSemaphoreProperties(externalSemaphoreInfo);
}

vk::ExternalSemaphorePropertiesKHR PhysicalDevice::getExternalSemaphorePropertiesKHR(
  const vk::PhysicalDeviceExternalSemaphoreInfoKHR& externalSemaphoreInfo) const {
  return object_->getExternalSemaphorePropertiesKHR(externalSemaphoreInfo);
}

vk::PhysicalDeviceFeatures PhysicalDevice::getFeatures() const {
  return object_->getFeatures();
}

vk::PhysicalDeviceFeatures2 PhysicalDevice::getFeatures2() const {
  return object_->getFeatures2();
}

vk::PhysicalDeviceFeatures2KHR PhysicalDevice::getFeatures2KHR() const {
  return object_->getFeatures2KHR();
}

vk::FormatProperties PhysicalDevice::getFormatProperties(vk::Format format) const {
  return object_->getFormatProperties(format);
}

vk::FormatProperties2 PhysicalDevice::getFormatProperties2(vk::Format format) const {
  return object_->getFormatProperties2(format);
}

vk::FormatProperties2KHR PhysicalDevice::getFormatProperties2KHR(vk::Format format) const {
  return object_->getFormatProperties2KHR(format);
}

vk::ImageFormatProperties PhysicalDevice::getImageFormatProperties(vk::Format format, vk::ImageType type,
                                                                   vk::ImageTiling tiling,
                                                                   const vk::ImageUsageFlags& usage,
                                                                   const vk::ImageCreateFlags& flags) const {
  return object_->getImageFormatProperties(format, type, tiling, usage, flags);
}

vk::ImageFormatProperties2
  PhysicalDevice::getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& formatInfo) const {
  return object_->getImageFormatProperties2(formatInfo);
}

vk::ImageFormatProperties2KHR
  PhysicalDevice::getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2& formatInfo) const {
  return object_->getImageFormatProperties2KHR(formatInfo);
}

vk::PhysicalDeviceMemoryProperties PhysicalDevice::getMemoryProperties() const {
  return object_->getMemoryProperties();
}

vk::PhysicalDeviceMemoryProperties2 PhysicalDevice::getMemoryProperties2() const {
  return object_->getMemoryProperties2();
}

vk::PhysicalDeviceMemoryProperties2KHR PhysicalDevice::getMemoryProperties2KHR() const {
  return object_->getMemoryProperties2KHR();
}

vk::PhysicalDeviceProperties PhysicalDevice::getProperties() const {
  return object_->getProperties();
}

vk::PhysicalDeviceProperties2 PhysicalDevice::getProperties2() const {
  return object_->getProperties2();
}

vk::PhysicalDeviceProperties2KHR PhysicalDevice::getProperties2KHR() const {
  return object_->getProperties2KHR();
}

std::vector<vk::QueueFamilyProperties> PhysicalDevice::getQueueFamilyProperties() const {
  return object_->getQueueFamilyProperties();
}

std::vector<vk::QueueFamilyProperties2> PhysicalDevice::getQueueFamilyProperties2() const {
  return object_->getQueueFamilyProperties2();
}

std::vector<vk::QueueFamilyProperties2KHR> PhysicalDevice::getQueueFamilyProperties2KHR() const {
  return object_->getQueueFamilyProperties2KHR();
}

std::vector<vk::SparseImageFormatProperties>
  PhysicalDevice::getSparseImageFormatProperties(vk::Format format, vk::ImageType type, vk::SampleCountFlagBits samples,
                                                 const vk::ImageUsageFlags& usage, vk::ImageTiling tiling) const {
  return object_->getSparseImageFormatProperties(format, type, samples, usage, tiling);
}

std::vector<vk::SparseImageFormatProperties2> PhysicalDevice::getSparseImageFormatProperties2(
  const vk::PhysicalDeviceSparseImageFormatInfo2& sparseImageFormatProperties) const {
  return object_->getSparseImageFormatProperties2(sparseImageFormatProperties);
}

std::vector<vk::SparseImageFormatProperties2KHR> PhysicalDevice::getSparseImageFormatProperties2KHR(
  const vk::PhysicalDeviceSparseImageFormatInfo2KHR& sparseImageFormatProperties) const {
  return object_->getSparseImageFormatProperties2KHR(sparseImageFormatProperties);
}

vk::ResultValueType<vk::SurfaceCapabilitiesKHR>::type
  PhysicalDevice::getSurfaceCapabilitiesKHR(vk::SurfaceKHR surface) const {
  return object_->getSurfaceCapabilitiesKHR(surface);
}

vk::ResultValueType<vk::SurfaceCapabilities2KHR>::type
  PhysicalDevice::getSurfaceCapabilities2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const {
  return object_->getSurfaceCapabilities2KHR(surfaceInfo);
}

vk::ResultValueType<std::vector<vk::SurfaceFormatKHR>>::type
  PhysicalDevice::getSurfaceFormatsKHR(vk::SurfaceKHR surface) const {
  return object_->getSurfaceFormatsKHR(surface);
}

vk::ResultValueType<std::vector<vk::SurfaceFormat2KHR>>::type
  PhysicalDevice::getSurfaceFormats2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const {
  return object_->getSurfaceFormats2KHR(surfaceInfo);
}

vk::ResultValueType<std::vector<vk::PresentModeKHR>>::type
  PhysicalDevice::getSurfacePresentModesKHR(vk::SurfaceKHR surface) const {
  return object_->getSurfacePresentModesKHR(surface);
}

vk::ResultValueType<vk::Bool32>::type PhysicalDevice::getSurfaceSupportKHR(uint32_t queueFamilyIndex,
                                                                           vk::SurfaceKHR surface) const {
  return object_->getSurfaceSupportKHR(queueFamilyIndex, surface);
}

vk::ExternalImageFormatPropertiesNV PhysicalDevice::getExternalImageFormatPropertiesNV(
  vk::Format format, vk::ImageType type, vk::ImageTiling tiling, const vk::ImageUsageFlags& usage,
  const vk::ImageCreateFlags& flags, const vk::ExternalMemoryHandleTypeFlagsNV& externalHandleType) const {
  return object_->getExternalImageFormatPropertiesNV(format, type, tiling, usage, flags, externalHandleType);
}

vk::DeviceGeneratedCommandsLimitsNVX
  PhysicalDevice::getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const {
  return object_->getGeneratedCommandsPropertiesNVX(features);
}

vk::ResultValueType<std::vector<vk::TimeDomainEXT>>::type PhysicalDevice::getCalibrateableTimeDomainsEXT() const {
  return object_->getCalibrateableTimeDomainsEXT();
}

vk::ResultValueType<std::vector<vk::CooperativeMatrixPropertiesNV>>::type
  PhysicalDevice::getCooperativeMatrixPropertiesNV() const {
  return object_->getCooperativeMatrixPropertiesNV();
}

vk::ResultValueType<std::vector<vk::DisplayPlanePropertiesKHR>>::type
  PhysicalDevice::getDisplayPlanePropertiesKHR() const {
  return object_->getDisplayPlanePropertiesKHR();
}

vk::ResultValueType<std::vector<vk::DisplayPlaneProperties2KHR>>::type
  PhysicalDevice::getDisplayPlaneProperties2KHR() const {
  return object_->getDisplayPlaneProperties2KHR();
}

vk::ResultValueType<std::vector<vk::DisplayPropertiesKHR>>::type PhysicalDevice::getDisplayPropertiesKHR() const {
  return object_->getDisplayPropertiesKHR();
}

vk::ResultValueType<std::vector<vk::DisplayProperties2KHR>>::type PhysicalDevice::getDisplayProperties2KHR() const {
  return object_->getDisplayProperties2KHR();
}

vk::MultisamplePropertiesEXT PhysicalDevice::getMultisamplePropertiesEXT(vk::SampleCountFlagBits samples) const {
  return object_->getMultisamplePropertiesEXT(samples);
}

vk::ResultValueType<std::vector<vk::Rect2D>>::type
  PhysicalDevice::getPresentRectanglesKHR(const vk::SurfaceKHR& surface) const {
  return object_->getPresentRectanglesKHR(surface);
}

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
vk::Bool32 PhysicalDevice::getWaylandPresentationSupportKHR(uint32_t queueFamilyIndex,
                                                            struct wl_display& display) const {
  return object_->getWaylandPresentationSupportKHR(queueFamilyIndex, display);
}
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
vk::Bool32 PhysicalDevice::getWin32PresentationSupportKHR(uint32_t queueFamilyIndex) const {
  return object_->getWin32PresentationSupportKHR(queueFamilyIndex);
}

#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
vk::Bool32 PhysicalDevice::getXcbPresentationSupportKHR(uint32_t queueFamilyIndex, xcb_connection_t& connection,
                                                        xcb_visualid_t visual_id) const {
  return object_->getXcbPresentationSupportKHR(queueFamilyIndex, connection, visual_id);
}
#endif

#ifdef VK_USE_PLATFORM_XLIB_KHR
vk::Bool32 PhysicalDevice::getXlibPresentationSupportKHR(uint32_t queueFamilyIndex, Display& dpy,
                                                         VisualID visualID) const {
  return object_->getXlibPresentationSupportKHR(queueFamilyIndex, dpy, visualID);
}
#endif

LogicalDevice PhysicalDevice::createLogicalDevice(const vk::DeviceCreateInfo& createInfo,
                                                  const std::optional<vk::AllocationCallbacks>& allocator) const {
  return LogicalDevice(object_->createLogicalDevice(createInfo, allocator));
}

void PhysicalDevice::destroyLogicalDevice(const LogicalDevice& device) const {
  object_->destroyLogicalDevice(device.id());
}

VulkanInstance PhysicalDevice::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

const vk::DispatchLoaderDynamic& PhysicalDevice::getDispatcher() const {
  return object_->getDispatcher();
}
PhysicalDevice::operator const vk::PhysicalDevice&() const {
  static vk::PhysicalDevice nullHandle(nullptr);
return (object_) ? object_->operator const vk::PhysicalDevice&() : nullHandle;
}

}