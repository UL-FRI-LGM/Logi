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
  PhysicalDevice::getExternalBufferProperties(const vk::PhysicalDeviceExternalBufferInfo& external_buffer_info) const {
  return object_->getExternalBufferProperties(external_buffer_info);
}

vk::ExternalBufferPropertiesKHR PhysicalDevice::getExternalBufferPropertiesKHR(
  const vk::PhysicalDeviceExternalBufferInfoKHR& external_buffer_info) const {
  return object_->getExternalBufferPropertiesKHR(external_buffer_info);
}

vk::ExternalFenceProperties
  PhysicalDevice::getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& external_fence_info) const {
  return object_->getExternalFenceProperties(external_fence_info);
}

vk::ExternalFencePropertiesKHR PhysicalDevice::getExternalFencePropertiesKHR(
  const vk::PhysicalDeviceExternalFenceInfoKHR& external_fence_info) const {
  return object_->getExternalFencePropertiesKHR(external_fence_info);
}

vk::ExternalSemaphoreProperties PhysicalDevice::getExternalSemaphoreProperties(
  const vk::PhysicalDeviceExternalSemaphoreInfo& external_semaphore_info) const {
  return object_->getExternalSemaphoreProperties(external_semaphore_info);
}

vk::ExternalSemaphorePropertiesKHR PhysicalDevice::getExternalSemaphorePropertiesKHR(
  const vk::PhysicalDeviceExternalSemaphoreInfoKHR& external_semaphore_info) const {
  return object_->getExternalSemaphorePropertiesKHR(external_semaphore_info);
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
  PhysicalDevice::getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& format_info) const {
  return object_->getImageFormatProperties2(format_info);
}

vk::ImageFormatProperties2KHR
  PhysicalDevice::getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2& format_info) const {
  return object_->getImageFormatProperties2KHR(format_info);
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
  const vk::PhysicalDeviceSparseImageFormatInfo2& sparse_image_format_properties) const {
  return object_->getSparseImageFormatProperties2(sparse_image_format_properties);
}

std::vector<vk::SparseImageFormatProperties2KHR> PhysicalDevice::getSparseImageFormatProperties2KHR(
  const vk::PhysicalDeviceSparseImageFormatInfo2KHR& sparse_image_format_properties) const {
  return object_->getSparseImageFormatProperties2KHR(sparse_image_format_properties);
}

vk::ResultValueType<std::vector<vk::Rect2D>>::type
  PhysicalDevice::getPresentRectanglesKHR(vk::SurfaceKHR surface) const {
  return object_->getPresentRectanglesKHR(surface);
}

vk::ResultValueType<vk::SurfaceCapabilitiesKHR>::type
  PhysicalDevice::getSurfaceCapabilitiesKHR(vk::SurfaceKHR surface) const {
  return object_->getSurfaceCapabilitiesKHR(surface);
}

vk::ResultValueType<vk::SurfaceCapabilities2KHR>::type
  PhysicalDevice::getSurfaceCapabilities2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surface_info) const {
  return object_->getSurfaceCapabilities2KHR(surface_info);
}

vk::ResultValueType<std::vector<vk::SurfaceFormatKHR>>::type
  PhysicalDevice::getSurfaceFormatsKHR(vk::SurfaceKHR surface) const {
  return object_->getSurfaceFormatsKHR(surface);
}

vk::ResultValueType<std::vector<vk::SurfaceFormat2KHR>>::type
  PhysicalDevice::getSurfaceFormats2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surface_info) const {
  return object_->getSurfaceFormats2KHR(surface_info);
}

vk::ResultValueType<std::vector<vk::PresentModeKHR>>::type
  PhysicalDevice::getSurfacePresentModesKHR(vk::SurfaceKHR surface) const {
  return object_->getSurfacePresentModesKHR(surface);
}

vk::ResultValueType<vk::Bool32>::type PhysicalDevice::getSurfaceSupportKHR(uint32_t queue_index,
                                                                           vk::SurfaceKHR surface) const {
  return object_->getSurfaceSupportKHR(queue_index, surface);
}

vk::ExternalImageFormatPropertiesNV PhysicalDevice::getExternalImageFormatPropertiesNV(
  vk::Format format, vk::ImageType type, vk::ImageTiling tiling, const vk::ImageUsageFlags& usage,
  const vk::ImageCreateFlags& flags, const vk::ExternalMemoryHandleTypeFlagsNV& external_handle_type) const {
  return object_->getExternalImageFormatPropertiesNV(format, type, tiling, usage, flags, external_handle_type);
}

vk::DeviceGeneratedCommandsLimitsNVX
  PhysicalDevice::getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const {
  return object_->getGeneratedCommandsPropertiesNVX(features);
}

LogicalDevice PhysicalDevice::createLogicalDevice(const vk::DeviceCreateInfo& create_info,
                                                  const std::optional<vk::AllocationCallbacks>& allocator) {
  return object_->createLogicalDevice(create_info, allocator);
}

void PhysicalDevice::destroyLogicalDevice(const LogicalDevice& device) {
  object_->destroyLogicalDevice(device.id());
}

VulkanInstance PhysicalDevice::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

const vk::DispatchLoaderDynamic& PhysicalDevice::getDispatcher() const {
  return object_->getDispatcher();
}
PhysicalDevice::operator vk::PhysicalDevice() const {
  return object_->operator vk::PhysicalDevice();
}

}