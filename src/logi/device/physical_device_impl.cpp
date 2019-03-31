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

PhysicalDeviceImpl::PhysicalDeviceImpl(VulkanInstanceImpl& instance, const vk::PhysicalDevice& vk_physical_device)
  : instance_(instance), vk_physical_device_(vk_physical_device) {}

typename vk::ResultValueType<std::vector<vk::ExtensionProperties>>::type
  PhysicalDeviceImpl::enumerateDeviceExtensionProperties(const std::string* layer_name) const {
  return vk_physical_device_.enumerateDeviceExtensionProperties(layer_name, getDispatcher());
}

typename vk::ResultValueType<std::vector<vk::LayerProperties>>::type
  PhysicalDeviceImpl::enumerateDeviceLayerProperties() const {
  return vk_physical_device_.enumerateDeviceLayerProperties(getDispatcher());
}

vk::ExternalBufferProperties PhysicalDeviceImpl::getExternalBufferProperties(
  const vk::PhysicalDeviceExternalBufferInfo& external_buffer_info) const {
  return vk_physical_device_.getExternalBufferProperties(external_buffer_info, getDispatcher());
}

vk::ExternalBufferPropertiesKHR PhysicalDeviceImpl::getExternalBufferPropertiesKHR(
  const vk::PhysicalDeviceExternalBufferInfoKHR& external_buffer_info) const {
  return vk_physical_device_.getExternalBufferPropertiesKHR(external_buffer_info, getDispatcher());
}

vk::ExternalFenceProperties
  PhysicalDeviceImpl::getExternalFenceProperties(const vk::PhysicalDeviceExternalFenceInfo& external_fence_info) const {
  return vk_physical_device_.getExternalFenceProperties(external_fence_info, getDispatcher());
}

vk::ExternalFencePropertiesKHR PhysicalDeviceImpl::getExternalFencePropertiesKHR(
  const vk::PhysicalDeviceExternalFenceInfoKHR& external_fence_info) const {
  return vk_physical_device_.getExternalFencePropertiesKHR(external_fence_info, getDispatcher());
}

vk::ExternalSemaphoreProperties PhysicalDeviceImpl::getExternalSemaphoreProperties(
  const vk::PhysicalDeviceExternalSemaphoreInfo& external_semaphore_info) const {
  return vk_physical_device_.getExternalSemaphoreProperties(external_semaphore_info, getDispatcher());
}

vk::ExternalSemaphorePropertiesKHR PhysicalDeviceImpl::getExternalSemaphorePropertiesKHR(
  const vk::PhysicalDeviceExternalSemaphoreInfoKHR& external_semaphore_info) const {
  return vk_physical_device_.getExternalSemaphorePropertiesKHR(external_semaphore_info, getDispatcher());
}

vk::PhysicalDeviceFeatures PhysicalDeviceImpl::getFeatures() const {
  return vk_physical_device_.getFeatures(getDispatcher());
}

vk::PhysicalDeviceFeatures2 PhysicalDeviceImpl::getFeatures2() const {
  return vk_physical_device_.getFeatures2(getDispatcher());
}

vk::PhysicalDeviceFeatures2KHR PhysicalDeviceImpl::getFeatures2KHR() const {
  return vk_physical_device_.getFeatures2KHR(getDispatcher());
}

vk::FormatProperties PhysicalDeviceImpl::getFormatProperties(vk::Format format) const {
  return vk_physical_device_.getFormatProperties(format, getDispatcher());
}

vk::FormatProperties2 PhysicalDeviceImpl::getFormatProperties2(vk::Format format) const {
  return vk_physical_device_.getFormatProperties2(format, getDispatcher());
}

vk::FormatProperties2KHR PhysicalDeviceImpl::getFormatProperties2KHR(vk::Format format) const {
  return vk_physical_device_.getFormatProperties2KHR(format, getDispatcher());
}

vk::ImageFormatProperties PhysicalDeviceImpl::getImageFormatProperties(vk::Format format, vk::ImageType type,
                                                                       vk::ImageTiling tiling,
                                                                       const vk::ImageUsageFlags& usage,
                                                                       const vk::ImageCreateFlags& flags) const {
  return vk_physical_device_.getImageFormatProperties(format, type, tiling, usage, flags, getDispatcher());
}

vk::ImageFormatProperties2
  PhysicalDeviceImpl::getImageFormatProperties2(const vk::PhysicalDeviceImageFormatInfo2& format_info) const {
  return vk_physical_device_.getImageFormatProperties2(format_info, getDispatcher());
}

vk::ImageFormatProperties2KHR
  PhysicalDeviceImpl::getImageFormatProperties2KHR(const vk::PhysicalDeviceImageFormatInfo2& format_info) const {
  return vk_physical_device_.getImageFormatProperties2KHR(format_info, getDispatcher());
}

vk::PhysicalDeviceMemoryProperties PhysicalDeviceImpl::getMemoryProperties() const {
  return vk_physical_device_.getMemoryProperties(getDispatcher());
}

vk::PhysicalDeviceMemoryProperties2 PhysicalDeviceImpl::getMemoryProperties2() const {
  return vk_physical_device_.getMemoryProperties2(getDispatcher());
}

vk::PhysicalDeviceMemoryProperties2KHR PhysicalDeviceImpl::getMemoryProperties2KHR() const {
  return vk_physical_device_.getMemoryProperties2KHR(getDispatcher());
}

vk::PhysicalDeviceProperties PhysicalDeviceImpl::getProperties() const {
  return vk_physical_device_.getProperties(getDispatcher());
}

vk::PhysicalDeviceProperties2 PhysicalDeviceImpl::getProperties2() const {
  return vk_physical_device_.getProperties2(getDispatcher());
}

vk::PhysicalDeviceProperties2KHR PhysicalDeviceImpl::getProperties2KHR() const {
  return vk_physical_device_.getProperties2KHR(getDispatcher());
}

std::vector<vk::QueueFamilyProperties> PhysicalDeviceImpl::getQueueFamilyProperties() const {
  return vk_physical_device_.getQueueFamilyProperties(getDispatcher());
}

std::vector<vk::QueueFamilyProperties2> PhysicalDeviceImpl::getQueueFamilyProperties2() const {
  return vk_physical_device_.getQueueFamilyProperties2(getDispatcher());
}
std::vector<vk::QueueFamilyProperties2KHR> PhysicalDeviceImpl::getQueueFamilyProperties2KHR() const {
  return vk_physical_device_.getQueueFamilyProperties2KHR(getDispatcher());
}

std::vector<vk::SparseImageFormatProperties>
  PhysicalDeviceImpl::getSparseImageFormatProperties(vk::Format format, vk::ImageType type,
                                                     vk::SampleCountFlagBits samples, const vk::ImageUsageFlags& usage,
                                                     vk::ImageTiling tiling) const {
  return vk_physical_device_.getSparseImageFormatProperties(format, type, samples, usage, tiling, getDispatcher());
}

std::vector<vk::SparseImageFormatProperties2> PhysicalDeviceImpl::getSparseImageFormatProperties2(
  const vk::PhysicalDeviceSparseImageFormatInfo2& p_sparse_image_format_properties) const {
  return vk_physical_device_.getSparseImageFormatProperties2(p_sparse_image_format_properties, getDispatcher());
}

std::vector<vk::SparseImageFormatProperties2KHR> PhysicalDeviceImpl::getSparseImageFormatProperties2KHR(
  const vk::PhysicalDeviceSparseImageFormatInfo2KHR& p_sparse_image_format_properties) const {
  return vk_physical_device_.getSparseImageFormatProperties2KHR(p_sparse_image_format_properties, getDispatcher());
}

vk::ExternalImageFormatPropertiesNV PhysicalDeviceImpl::getExternalImageFormatPropertiesNV(
  vk::Format format, vk::ImageType type, vk::ImageTiling tiling, const vk::ImageUsageFlags& usage,
  const vk::ImageCreateFlags& flags, const vk::ExternalMemoryHandleTypeFlagsNV& external_handle_type) const {
  return vk_physical_device_.getExternalImageFormatPropertiesNV(format, type, tiling, usage, flags,
                                                                external_handle_type, getDispatcher());
}

vk::DeviceGeneratedCommandsLimitsNVX
  PhysicalDeviceImpl::getGeneratedCommandsPropertiesNVX(vk::DeviceGeneratedCommandsFeaturesNVX& features) const {
  return vk_physical_device_.getGeneratedCommandsPropertiesNVX(features, getDispatcher());
}

LogicalDevice PhysicalDeviceImpl::createLogicalDevice(const vk::DeviceCreateInfo& create_info,
                                                      const std::optional<vk::AllocationCallbacks>& allocator) {
  return LogicalDevice(VulkanObjectComposite<LogicalDeviceImpl>::createObject(*this, create_info, allocator));
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
  return vk_physical_device_;
}

void PhysicalDeviceImpl::free() {
  // VulkanObjectComposite<LogicalDeviceImpl>::destroyAllObjects();
  VulkanObject::free();
}

} // namespace logi