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

#include "logi/device/physical_device_impl.hpp"
#include "logi/device/logical_device.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"

namespace logi {

PhysicalDeviceImpl::PhysicalDeviceImpl(VulkanInstanceImpl& instance, const vk::PhysicalDevice& vkPhysicalDevice)
  : instance_(instance), vkPhysicalDevice_(vkPhysicalDevice) {}

typename vk::ResultValueType<std::vector<vk::ExtensionProperties>>::type
  PhysicalDeviceImpl::enumerateDeviceExtensionProperties(const std::string* layerName) const {
  return vkPhysicalDevice_.enumerateDeviceExtensionProperties(layerName, getDispatcher());
}

typename vk::ResultValueType<std::vector<vk::LayerProperties>>::type
  PhysicalDeviceImpl::enumerateDeviceLayerProperties() const {
  return vkPhysicalDevice_.enumerateDeviceLayerProperties(getDispatcher());
}

vk::ExternalBufferProperties PhysicalDeviceImpl::getExternalBufferProperties(
  const vk::PhysicalDeviceExternalBufferInfo& externalBufferInfo) const {
  return vkPhysicalDevice_.getExternalBufferProperties(externalBufferInfo, getDispatcher());
}

vk::ExternalBufferPropertiesKHR PhysicalDeviceImpl::getExternalBufferPropertiesKHR(
  const vk::PhysicalDeviceExternalBufferInfoKHR& externalBufferInfo) const {
  return vkPhysicalDevice_.getExternalBufferPropertiesKHR(externalBufferInfo, getDispatcher());
}

vk::ExternalFenceProperties
  PhysicalDeviceImpl::getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& externalFenceInfo) const {
  return vkPhysicalDevice_.getExternalFenceProperties(externalFenceInfo, getDispatcher());
}

vk::ExternalFencePropertiesKHR PhysicalDeviceImpl::getExternalFencePropertiesKHR(
  const vk::PhysicalDeviceExternalFenceInfoKHR& externalFenceInfo) const {
  return vkPhysicalDevice_.getExternalFencePropertiesKHR(externalFenceInfo, getDispatcher());
}

vk::ExternalSemaphoreProperties PhysicalDeviceImpl::getExternalSemaphoreProperties(
  const vk::PhysicalDeviceExternalSemaphoreInfo& externalSemaphoreInfo) const {
  return vkPhysicalDevice_.getExternalSemaphoreProperties(externalSemaphoreInfo, getDispatcher());
}

vk::ExternalSemaphorePropertiesKHR PhysicalDeviceImpl::getExternalSemaphorePropertiesKHR(
  const vk::PhysicalDeviceExternalSemaphoreInfoKHR& externalSemaphoreInfo) const {
  return vkPhysicalDevice_.getExternalSemaphorePropertiesKHR(externalSemaphoreInfo, getDispatcher());
}

vk::PhysicalDeviceFeatures PhysicalDeviceImpl::getFeatures() const {
  return vkPhysicalDevice_.getFeatures(getDispatcher());
}

vk::PhysicalDeviceFeatures2 PhysicalDeviceImpl::getFeatures2() const {
  return vkPhysicalDevice_.getFeatures2(getDispatcher());
}

vk::PhysicalDeviceFeatures2KHR PhysicalDeviceImpl::getFeatures2KHR() const {
  return vkPhysicalDevice_.getFeatures2KHR(getDispatcher());
}

vk::FormatProperties PhysicalDeviceImpl::getFormatProperties(vk::Format format) const {
  return vkPhysicalDevice_.getFormatProperties(format, getDispatcher());
}

vk::FormatProperties2 PhysicalDeviceImpl::getFormatProperties2(vk::Format format) const {
  return vkPhysicalDevice_.getFormatProperties2(format, getDispatcher());
}

vk::FormatProperties2KHR PhysicalDeviceImpl::getFormatProperties2KHR(vk::Format format) const {
  return vkPhysicalDevice_.getFormatProperties2KHR(format, getDispatcher());
}

vk::ImageFormatProperties PhysicalDeviceImpl::getImageFormatProperties(vk::Format format, vk::ImageType type,
                                                                       vk::ImageTiling tiling,
                                                                       const vk::ImageUsageFlags& usage,
                                                                       const vk::ImageCreateFlags& flags) const {
  return vkPhysicalDevice_.getImageFormatProperties(format, type, tiling, usage, flags, getDispatcher());
}

vk::ImageFormatProperties2
  PhysicalDeviceImpl::getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& formatInfo) const {
  return vkPhysicalDevice_.getImageFormatProperties2(formatInfo, getDispatcher());
}

vk::ImageFormatProperties2KHR
  PhysicalDeviceImpl::getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2& formatInfo) const {
  return vkPhysicalDevice_.getImageFormatProperties2KHR(formatInfo, getDispatcher());
}

vk::PhysicalDeviceMemoryProperties PhysicalDeviceImpl::getMemoryProperties() const {
  return vkPhysicalDevice_.getMemoryProperties(getDispatcher());
}

vk::PhysicalDeviceMemoryProperties2 PhysicalDeviceImpl::getMemoryProperties2() const {
  return vkPhysicalDevice_.getMemoryProperties2(getDispatcher());
}

vk::PhysicalDeviceMemoryProperties2KHR PhysicalDeviceImpl::getMemoryProperties2KHR() const {
  return vkPhysicalDevice_.getMemoryProperties2KHR(getDispatcher());
}

vk::PhysicalDeviceProperties PhysicalDeviceImpl::getProperties() const {
  return vkPhysicalDevice_.getProperties(getDispatcher());
}

vk::PhysicalDeviceProperties2 PhysicalDeviceImpl::getProperties2() const {
  return vkPhysicalDevice_.getProperties2(getDispatcher());
}

vk::PhysicalDeviceProperties2KHR PhysicalDeviceImpl::getProperties2KHR() const {
  return vkPhysicalDevice_.getProperties2KHR(getDispatcher());
}

std::vector<vk::QueueFamilyProperties> PhysicalDeviceImpl::getQueueFamilyProperties() const {
  return vkPhysicalDevice_.getQueueFamilyProperties(getDispatcher());
}

std::vector<vk::QueueFamilyProperties2> PhysicalDeviceImpl::getQueueFamilyProperties2() const {
  return vkPhysicalDevice_.getQueueFamilyProperties2(getDispatcher());
}
std::vector<vk::QueueFamilyProperties2KHR> PhysicalDeviceImpl::getQueueFamilyProperties2KHR() const {
  return vkPhysicalDevice_.getQueueFamilyProperties2KHR(getDispatcher());
}

std::vector<vk::SparseImageFormatProperties>
  PhysicalDeviceImpl::getSparseImageFormatProperties(vk::Format format, vk::ImageType type,
                                                     vk::SampleCountFlagBits samples, const vk::ImageUsageFlags& usage,
                                                     vk::ImageTiling tiling) const {
  return vkPhysicalDevice_.getSparseImageFormatProperties(format, type, samples, usage, tiling, getDispatcher());
}

std::vector<vk::SparseImageFormatProperties2> PhysicalDeviceImpl::getSparseImageFormatProperties2(
  const vk::PhysicalDeviceSparseImageFormatInfo2& pSparseImageFormatProperties) const {
  return vkPhysicalDevice_.getSparseImageFormatProperties2(pSparseImageFormatProperties, getDispatcher());
}

std::vector<vk::SparseImageFormatProperties2KHR> PhysicalDeviceImpl::getSparseImageFormatProperties2KHR(
  const vk::PhysicalDeviceSparseImageFormatInfo2KHR& pSparseImageFormatProperties) const {
  return vkPhysicalDevice_.getSparseImageFormatProperties2KHR(pSparseImageFormatProperties, getDispatcher());
}

vk::ResultValueType<vk::SurfaceCapabilitiesKHR>::type
  PhysicalDeviceImpl::getSurfaceCapabilitiesKHR(vk::SurfaceKHR surface) const {
  return vkPhysicalDevice_.getSurfaceCapabilitiesKHR(surface, getDispatcher());
}

vk::ResultValueType<std::vector<vk::SurfaceFormatKHR>>::type
  PhysicalDeviceImpl::getSurfaceFormatsKHR(vk::SurfaceKHR surface) const {
  return vkPhysicalDevice_.getSurfaceFormatsKHR(surface, getDispatcher());
}

vk::ResultValueType<vk::SurfaceCapabilities2KHR>::type
  PhysicalDeviceImpl::getSurfaceCapabilities2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const {
  return vkPhysicalDevice_.getSurfaceCapabilities2KHR(surfaceInfo, getDispatcher());
}

vk::ResultValueType<std::vector<vk::SurfaceFormat2KHR>>::type
  PhysicalDeviceImpl::getSurfaceFormats2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const {
  return vkPhysicalDevice_.getSurfaceFormats2KHR(surfaceInfo, getDispatcher());
}

vk::ResultValueType<std::vector<vk::PresentModeKHR>>::type
  PhysicalDeviceImpl::getSurfacePresentModesKHR(vk::SurfaceKHR surface) const {
  return vkPhysicalDevice_.getSurfacePresentModesKHR(surface, getDispatcher());
}

vk::ResultValueType<vk::Bool32>::type PhysicalDeviceImpl::getSurfaceSupportKHR(uint32_t queueFamilyIndex,
                                                                               vk::SurfaceKHR surface) const {
  return vkPhysicalDevice_.getSurfaceSupportKHR(queueFamilyIndex, surface, getDispatcher());
}

vk::ExternalImageFormatPropertiesNV PhysicalDeviceImpl::getExternalImageFormatPropertiesNV(
  vk::Format format, vk::ImageType type, vk::ImageTiling tiling, const vk::ImageUsageFlags& usage,
  const vk::ImageCreateFlags& flags, const vk::ExternalMemoryHandleTypeFlagsNV& externalHandleType) const {
  return vkPhysicalDevice_.getExternalImageFormatPropertiesNV(format, type, tiling, usage, flags, externalHandleType,
                                                              getDispatcher());
}

vk::DeviceGeneratedCommandsLimitsNVX
  PhysicalDeviceImpl::getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const {
  return vkPhysicalDevice_.getGeneratedCommandsPropertiesNVX(features, getDispatcher());
}

vk::ResultValueType<std::vector<vk::TimeDomainEXT>>::type PhysicalDeviceImpl::getCalibrateableTimeDomainsEXT() const {
  return vkPhysicalDevice_.getCalibrateableTimeDomainsEXT(getDispatcher());
}

vk::ResultValueType<std::vector<vk::CooperativeMatrixPropertiesNV>>::type
  PhysicalDeviceImpl::getCooperativeMatrixPropertiesNV() const {
  return vkPhysicalDevice_.getCooperativeMatrixPropertiesNV(getDispatcher());
}

vk::ResultValueType<std::vector<vk::DisplayPlanePropertiesKHR>>::type
  PhysicalDeviceImpl::getDisplayPlanePropertiesKHR() const {
  return vkPhysicalDevice_.getDisplayPlanePropertiesKHR(getDispatcher());
}

vk::ResultValueType<std::vector<vk::DisplayPlaneProperties2KHR>>::type
  PhysicalDeviceImpl::getDisplayPlaneProperties2KHR() const {
  return vkPhysicalDevice_.getDisplayPlaneProperties2KHR(getDispatcher());
}

vk::ResultValueType<std::vector<vk::DisplayPropertiesKHR>>::type PhysicalDeviceImpl::getDisplayPropertiesKHR() const {
  return vkPhysicalDevice_.getDisplayPropertiesKHR(getDispatcher());
}

vk::ResultValueType<std::vector<vk::DisplayProperties2KHR>>::type PhysicalDeviceImpl::getDisplayProperties2KHR() const {
  return vkPhysicalDevice_.getDisplayProperties2KHR(getDispatcher());
}

vk::MultisamplePropertiesEXT PhysicalDeviceImpl::getMultisamplePropertiesEXT(vk::SampleCountFlagBits samples) const {
  return vkPhysicalDevice_.getMultisamplePropertiesEXT(samples, getDispatcher());
}

vk::ResultValueType<std::vector<vk::Rect2D>>::type
  PhysicalDeviceImpl::getPresentRectanglesKHR(const vk::SurfaceKHR& surface) const {
  return vkPhysicalDevice_.getPresentRectanglesKHR(surface, getDispatcher());
}

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
vk::Bool32 PhysicalDeviceImpl::getWaylandPresentationSupportKHR(uint32_t queueFamilyIndex,
                                                                struct wl_display& display) const {
  return vkPhysicalDevice_.getWaylandPresentationSupportKHR(queueFamilyIndex, &display, getDispatcher());
}
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
vk::Bool32 PhysicalDeviceImpl::getWin32PresentationSupportKHR(uint32_t queueFamilyIndex) const {
  return vkPhysicalDevice_.getWin32PresentationSupportKHR(queueFamilyIndex, getDispatcher());
}
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
vk::Bool32 PhysicalDeviceImpl::getXcbPresentationSupportKHR(uint32_t queueFamilyIndex, xcb_connection_t& connection,
                                                            xcb_visualid_t visual_id) const {
  return vkPhysicalDevice_.getXcbPresentationSupportKHR(queueFamilyIndex, &connection, visual_id, getDispatcher());
}
#endif

#ifdef VK_USE_PLATFORM_XLIB_KHR
vk::Bool32 PhysicalDeviceImpl::getXlibPresentationSupportKHR(uint32_t queueFamilyIndex, Display& dpy,
                                                             VisualID visualID) const {
  return vkPhysicalDevice_.getXlibPresentationSupportKHR(queueFamilyIndex, &dpy, visualID, getDispatcher());
}
#endif

const std::shared_ptr<LogicalDeviceImpl>&
  PhysicalDeviceImpl::createLogicalDevice(const vk::DeviceCreateInfo& createInfo,
                                          const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<LogicalDeviceImpl>::createObject(*this, createInfo, allocator);
}

void PhysicalDeviceImpl::destroyLogicalDevice(size_t id) {
  VulkanObjectComposite<LogicalDeviceImpl>::destroyObject(id);
}

VulkanInstanceImpl& PhysicalDeviceImpl::getInstance() const {
  return instance_;
}

const vk::DispatchLoaderDynamic& PhysicalDeviceImpl::getDispatcher() const {
  return instance_.getDispatcher();
}

PhysicalDeviceImpl::operator vk::PhysicalDevice() const {
  return vkPhysicalDevice_;
}

void PhysicalDeviceImpl::free() {
  VulkanObjectComposite<LogicalDeviceImpl>::destroyAllObjects();
  vkPhysicalDevice_ = nullptr;
  VulkanObject::free();
}

} // namespace logi