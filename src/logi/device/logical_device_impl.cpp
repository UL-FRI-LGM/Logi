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

#include "logi/device/logical_device_impl.hpp"
#include "logi/command/command_pool_impl.hpp"
#include "logi/descriptor/descriptor_pool_impl.hpp"
#include "logi/descriptor/descriptor_update_template_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/memory/acceleration_structure_nv_impl.hpp"
#include "logi/memory/buffer_impl.hpp"
#include "logi/memory/device_memory_impl.hpp"
#include "logi/memory/image_impl.hpp"
#include "logi/memory/memory_allocator_impl.hpp"
#include "logi/memory/sampler_impl.hpp"
#include "logi/memory/sampler_ycbcr_conversion_impl.hpp"
#include "logi/nvidia/indirect_commands_layout_nvx_impl.hpp"
#include "logi/nvidia/object_table_nvx_impl.hpp"
#include "logi/program/descriptor_set_layout_impl.hpp"
#include "logi/program/pipeline_cache_impl.hpp"
#include "logi/program/pipeline_impl.hpp"
#include "logi/program/pipeline_layout_impl.hpp"
#include "logi/program/shader_module_impl.hpp"
#include "logi/program/validation_cache_ext_impl.hpp"
#include "logi/query/query_pool_impl.hpp"
#include "logi/queue/queue_family_impl.hpp"
#include "logi/render_pass/framebuffer_impl.hpp"
#include "logi/render_pass/render_pass_impl.hpp"
#include "logi/swapchain/swapchain_khr_impl.hpp"
#include "logi/synchronization/event_impl.hpp"
#include "logi/synchronization/fence_impl.hpp"
#include "logi/synchronization/semaphore_impl.hpp"

namespace logi {

LogicalDeviceImpl::LogicalDeviceImpl(PhysicalDeviceImpl& physicalDevice, const vk::DeviceCreateInfo& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator)
  : physicalDevice_(physicalDevice), allocator_(allocator) {
  vk::Instance vkInstance = physicalDevice.getInstance();
  vk::PhysicalDevice vkPhysicalDevice = physicalDevice;
  const vk::DispatchLoaderDynamic& instanceDispatcher = physicalDevice.getDispatcher();

  vkDevice_ = vkPhysicalDevice.createDevice(createInfo, allocator_ ? &allocator.value() : nullptr, instanceDispatcher);

  // Initialize device dispatcher.
  dispatcher_ = vk::DispatchLoaderDynamic(static_cast<VkInstance>(vkInstance), instanceDispatcher.vkGetInstanceProcAddr,
                                          static_cast<VkDevice>(vkDevice_), instanceDispatcher.vkGetDeviceProcAddr);

  // Initialize queue families.
  for (uint32_t i = 0u; i < createInfo.queueCreateInfoCount; i++) {
    VulkanObjectComposite<QueueFamilyImpl>::createObject(*this, createInfo.pQueueCreateInfos[i]);
  }
}

const std::shared_ptr<MemoryAllocatorImpl>&
  LogicalDeviceImpl::createMemoryAllocator(vk::DeviceSize preferredLargeHeapBlockSize, uint32_t frameInUseCount,
                                           const std::vector<vk::DeviceSize>& heapSizeLimits,
                                           const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<MemoryAllocatorImpl>::createObject(*this, preferredLargeHeapBlockSize, frameInUseCount,
                                                                  heapSizeLimits, allocator);
}

void LogicalDeviceImpl::destroyMemoryAllocator(size_t id) {
  VulkanObjectComposite<MemoryAllocatorImpl>::destroyObject(id);
}

const std::shared_ptr<DeviceMemoryImpl>&
  LogicalDeviceImpl::allocateMemory(const vk::MemoryAllocateInfo& createInfo,
                                    const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<DeviceMemoryImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::freeMemory(size_t id) {
  VulkanObjectComposite<DeviceMemoryImpl>::destroyObject(id);
}

const std::shared_ptr<BufferImpl>&
  LogicalDeviceImpl::createBuffer(const vk::BufferCreateInfo& createInfo,
                                  const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<BufferImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyBuffer(size_t id) {
  VulkanObjectComposite<BufferImpl>::destroyObject(id);
}

const std::shared_ptr<ImageImpl>&
  LogicalDeviceImpl::createImage(const vk::ImageCreateInfo& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<ImageImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyImage(size_t id) {
  VulkanObjectComposite<ImageImpl>::destroyObject(id);
}

const std::shared_ptr<SamplerImpl>&
  LogicalDeviceImpl::createSampler(const vk::SamplerCreateInfo& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SamplerImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroySampler(size_t id) {
  VulkanObjectComposite<SamplerImpl>::destroyObject(id);
}

const std::shared_ptr<SamplerYcbcrConversionImpl>&
  LogicalDeviceImpl::createSamplerYcbcrConversion(const vk::SamplerYcbcrConversionCreateInfo& createInfo,
                                                  const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SamplerYcbcrConversionImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroySamplerYcbcrConversion(size_t id) {
  VulkanObjectComposite<SamplerYcbcrConversionImpl>::destroyObject(id);
}

const std::shared_ptr<ShaderModuleImpl>&
  LogicalDeviceImpl::createShaderModule(const vk::ShaderModuleCreateInfo& createInfo,
                                        const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<ShaderModuleImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyShaderModule(size_t id) {
  VulkanObjectComposite<ShaderModuleImpl>::destroyObject(id);
}

const std::shared_ptr<PipelineCacheImpl>&
  LogicalDeviceImpl::createPipelineCache(const vk::PipelineCacheCreateInfo& createInfo,
                                         const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<PipelineCacheImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyPipelineCache(size_t id) {
  VulkanObjectComposite<PipelineCacheImpl>::destroyObject(id);
}

const std::shared_ptr<DescriptorSetLayoutImpl>&
  LogicalDeviceImpl::createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& createInfo,
                                               const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<DescriptorSetLayoutImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyDescriptorSetLayout(size_t id) {
  VulkanObjectComposite<DescriptorSetLayoutImpl>::destroyObject(id);
}

const std::shared_ptr<DescriptorPoolImpl>&
  LogicalDeviceImpl::createDescriptorPool(const vk::DescriptorPoolCreateInfo& createInfo,
                                          const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<DescriptorPoolImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyDescriptorPool(size_t id) {
  VulkanObjectComposite<DescriptorPoolImpl>::destroyObject(id);
}

const std::shared_ptr<DescriptorUpdateTemplateImpl>&
  LogicalDeviceImpl::createDescriptorUpdateTemplate(const vk::DescriptorUpdateTemplateCreateInfo& createInfo,
                                                    const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<DescriptorUpdateTemplateImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyDescriptorUpdateTemplate(size_t id) {
  VulkanObjectComposite<DescriptorUpdateTemplateImpl>::destroyObject(id);
}

const std::shared_ptr<PipelineLayoutImpl>&
  LogicalDeviceImpl::createPipelineLayout(const vk::PipelineLayoutCreateInfo& createInfo,
                                          const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<PipelineLayoutImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyPipelineLayout(size_t id) {
  VulkanObjectComposite<PipelineLayoutImpl>::destroyObject(id);
}

std::vector<std::shared_ptr<PipelineImpl>>
  LogicalDeviceImpl::createComputePipelines(const vk::ArrayProxy<const vk::ComputePipelineCreateInfo>& createInfos,
                                            const vk::PipelineCache& cache,
                                            const std::optional<vk::AllocationCallbacks>& allocator) {
  std::vector<vk::Pipeline> vkPipelines =
    vkDevice_.createComputePipelines(cache, createInfos, allocator ? &allocator.value() : nullptr, getDispatcher());

  std::vector<std::shared_ptr<PipelineImpl>> pipelines;
  pipelines.reserve(vkPipelines.size());

  for (const vk::Pipeline& vkPipeline : vkPipelines) {
    pipelines.emplace_back(VulkanObjectComposite<PipelineImpl>::createObject(*this, vkPipeline, allocator));
  }

  return pipelines;
}

std::shared_ptr<PipelineImpl>
  LogicalDeviceImpl::createComputePipeline(const vk::ComputePipelineCreateInfo& createInfo,
                                           const vk::PipelineCache& cache,
                                           const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<PipelineImpl>::createObject(
    *this,
    vkDevice_.createComputePipeline(cache, createInfo, allocator ? &allocator.value() : nullptr, getDispatcher()),
    allocator);
}

std::vector<std::shared_ptr<PipelineImpl>>
  LogicalDeviceImpl::createGraphicsPipelines(const vk::ArrayProxy<const vk::GraphicsPipelineCreateInfo>& createInfos,
                                             const vk::PipelineCache& cache,
                                             const std::optional<vk::AllocationCallbacks>& allocator) {
  std::vector<vk::Pipeline> vkPipelines =
    vkDevice_.createGraphicsPipelines(cache, createInfos, allocator ? &allocator.value() : nullptr, getDispatcher());

  std::vector<std::shared_ptr<PipelineImpl>> pipelines;
  pipelines.reserve(vkPipelines.size());

  for (const vk::Pipeline& vkPipeline : vkPipelines) {
    pipelines.emplace_back(VulkanObjectComposite<PipelineImpl>::createObject(*this, vkPipeline, allocator));
  }

  return pipelines;
}

std::shared_ptr<PipelineImpl>
  LogicalDeviceImpl::createGraphicsPipeline(const vk::GraphicsPipelineCreateInfo& createInfo,
                                            const vk::PipelineCache& cache,
                                            const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<PipelineImpl>::createObject(
    *this,
    vkDevice_.createGraphicsPipeline(cache, createInfo, allocator ? &allocator.value() : nullptr, getDispatcher()),
    allocator);
}

std::vector<std::shared_ptr<PipelineImpl>> LogicalDeviceImpl::createRayTracingPipelinesNV(
  const vk::ArrayProxy<const vk::RayTracingPipelineCreateInfoNV>& createInfos, const vk::PipelineCache& cache,
  const std::optional<vk::AllocationCallbacks>& allocator) {
  std::vector<vk::Pipeline> vkPipelines = vkDevice_.createRayTracingPipelinesNV(
    cache, createInfos, allocator ? &allocator.value() : nullptr, getDispatcher());

  std::vector<std::shared_ptr<PipelineImpl>> pipelines;
  pipelines.reserve(vkPipelines.size());

  for (const vk::Pipeline& vkPipeline : vkPipelines) {
    pipelines.emplace_back(VulkanObjectComposite<PipelineImpl>::createObject(*this, vkPipeline, allocator));
  }

  return pipelines;
}

std::shared_ptr<PipelineImpl>
  LogicalDeviceImpl::createRayTracingPipelineNV(const vk::RayTracingPipelineCreateInfoNV& createInfo,
                                                const vk::PipelineCache& cache,
                                                const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<PipelineImpl>::createObject(
    *this,
    vkDevice_.createRayTracingPipelineNV(cache, createInfo, allocator ? &allocator.value() : nullptr, getDispatcher()),
    allocator);
}

void LogicalDeviceImpl::destroyPipeline(size_t id) {
  VulkanObjectComposite<PipelineImpl>::destroyObject(id);
}

const std::shared_ptr<QueryPoolImpl>&
  LogicalDeviceImpl::createQueryPool(const vk::QueryPoolCreateInfo& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<QueryPoolImpl>::createObject(*this, createInfo, allocator);
}
void LogicalDeviceImpl::destroyQueryPool(size_t id) {
  VulkanObjectComposite<QueryPoolImpl>::destroyObject(id);
}

const std::shared_ptr<EventImpl>&
  LogicalDeviceImpl::createEvent(const vk::EventCreateInfo& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<EventImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyEvent(size_t id) {
  VulkanObjectComposite<EventImpl>::destroyObject(id);
}

const std::shared_ptr<FenceImpl>&
  LogicalDeviceImpl::createFence(const vk::FenceCreateInfo& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<FenceImpl>::createObject(*this, createInfo, allocator);
}

const std::shared_ptr<FenceImpl>&
  LogicalDeviceImpl::registerEventEXT(const vk::DeviceEventInfoEXT& eventInfo,
                                      const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<FenceImpl>::createObject(*this, eventInfo, allocator);
}

const std::shared_ptr<FenceImpl>&
  LogicalDeviceImpl::registerDisplayEventEXT(const vk::DisplayKHR& display, const vk::DisplayEventInfoEXT& eventInfo,
                                             const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<FenceImpl>::createObject(*this, display, eventInfo, allocator);
}

void LogicalDeviceImpl::destroyFence(size_t id) {
  VulkanObjectComposite<FenceImpl>::destroyObject(id);
}

const std::shared_ptr<SemaphoreImpl>&
  LogicalDeviceImpl::createSemaphore(const vk::SemaphoreCreateInfo& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SemaphoreImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroySemaphore(size_t id) {
  VulkanObjectComposite<SemaphoreImpl>::destroyObject(id);
}

const std::shared_ptr<RenderPassImpl>&
  LogicalDeviceImpl::createRenderPass(const vk::RenderPassCreateInfo& createInfo,
                                      const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<RenderPassImpl>::createObject(*this, createInfo, allocator);
}

const std::shared_ptr<RenderPassImpl>&
  LogicalDeviceImpl::createRenderPass(const vk::RenderPassCreateInfo2KHR& createInfo,
                                      const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<RenderPassImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyRenderPass(size_t id) {
  VulkanObjectComposite<RenderPassImpl>::destroyObject(id);
}

const std::shared_ptr<FramebufferImpl>&
  LogicalDeviceImpl::createFramebuffer(const vk::FramebufferCreateInfo& createInfo,
                                       const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<FramebufferImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyFramebuffer(size_t id) {
  VulkanObjectComposite<FramebufferImpl>::destroyObject(id);
}

const std::shared_ptr<SwapchainKHRImpl>&
  LogicalDeviceImpl::createSwapchainKHR(const vk::SwapchainCreateInfoKHR& createInfo,
                                        const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<SwapchainKHRImpl>::createObject(*this, createInfo, allocator);
}

std::vector<std::shared_ptr<SwapchainKHRImpl>>
  LogicalDeviceImpl::createSharedSwapchainsKHR(const vk::ArrayProxy<const vk::SwapchainCreateInfoKHR>& createInfos,
                                               const std::optional<vk::AllocationCallbacks>& allocator) {
  std::vector<vk::SwapchainKHR> vkSwapchains =
    vkDevice_.createSharedSwapchainsKHR(createInfos, allocator_ ? &allocator_.value() : nullptr, getDispatcher());

  std::vector<std::shared_ptr<SwapchainKHRImpl>> logiSwapchains;
  logiSwapchains.reserve(vkSwapchains.size());

  for (const auto& vkSwapchain : vkSwapchains) {
    logiSwapchains.emplace_back(VulkanObjectComposite<SwapchainKHRImpl>::createObject(*this, vkSwapchain, allocator));
  }

  return logiSwapchains;
}

void LogicalDeviceImpl::destroySwapchainKHR(size_t id) {
  return VulkanObjectComposite<SwapchainKHRImpl>::destroyObject(id);
}

const std::shared_ptr<ValidationCacheEXTImpl>&
  LogicalDeviceImpl::createValidationCacheEXT(const vk::ValidationCacheCreateInfoEXT& createInfo,
                                              const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<ValidationCacheEXTImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyValidationCacheEXT(size_t id) {
  return VulkanObjectComposite<ValidationCacheEXTImpl>::destroyObject(id);
}

const std::shared_ptr<AccelerationStructureNVImpl>&
  LogicalDeviceImpl::createAccelerationStructureNV(const vk::AccelerationStructureCreateInfoNV& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<AccelerationStructureNVImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyAccelerationStructureNV(size_t id) {
  VulkanObjectComposite<AccelerationStructureNVImpl>::destroyObject(id);
}

const std::shared_ptr<IndirectCommandsLayoutNVXImpl>&
  LogicalDeviceImpl::createIndirectCommandsLayoutNVXIndirectCommandsLayoutNVX(
    const vk::IndirectCommandsLayoutCreateInfoNVX& createInfo,
    const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<IndirectCommandsLayoutNVXImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyIndirectCommandsLayoutNVX(size_t id) {
  VulkanObjectComposite<IndirectCommandsLayoutNVXImpl>::destroyObject(id);
}

const std::shared_ptr<ObjectTableNVXImpl>&
  LogicalDeviceImpl::createObjectTableNVX(const vk::ObjectTableCreateInfoNVX& createInfo,
                                          const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanObjectComposite<ObjectTableNVXImpl>::createObject(*this, createInfo, allocator);
}

void LogicalDeviceImpl::destroyObjectTableNVX(size_t id) {
  VulkanObjectComposite<ObjectTableNVXImpl>::destroyObject(id);
}

std::vector<std::shared_ptr<QueueFamilyImpl>> LogicalDeviceImpl::enumerateQueueFamilies() const {
  std::unordered_map<size_t, std::shared_ptr<QueueFamilyImpl>> familiesMap =
    VulkanObjectComposite<QueueFamilyImpl>::getHandles();

  std::vector<std::shared_ptr<QueueFamilyImpl>> queueFamilies;
  queueFamilies.reserve(familiesMap.size());

  for (const auto& entry : familiesMap) {
    queueFamilies.emplace_back(entry.second);
  }

  return queueFamilies;
}

void LogicalDeviceImpl::updateDescriptorSets(
  const vk::ArrayProxy<const vk::WriteDescriptorSet>& descriptorWrites,
  const vk::ArrayProxy<const vk::CopyDescriptorSet>& descriptorCopies) const {
  vkDevice_.updateDescriptorSets(descriptorWrites, descriptorCopies, getDispatcher());
}

vk::ResultValueType<void>::type
  LogicalDeviceImpl::setDebugUtilsObjectNameEXT(const vk::DebugUtilsObjectNameInfoEXT& nameInfo) const {
  return vkDevice_.setDebugUtilsObjectNameEXT(nameInfo, getDispatcher());
}

vk::ResultValueType<void>::type
  LogicalDeviceImpl::setDebugUtilsObjectTagEXT(const vk::DebugUtilsObjectTagInfoEXT& tagInfo) const {
  return vkDevice_.setDebugUtilsObjectTagEXT(tagInfo, getDispatcher());
}

vk::ResultValueType<void>::type
  LogicalDeviceImpl::debugMarkerSetObjectNameEXT(const vk::DebugMarkerObjectNameInfoEXT& nameInfo) const {
  return vkDevice_.debugMarkerSetObjectNameEXT(nameInfo, getDispatcher());
}

vk::ResultValueType<void>::type
  LogicalDeviceImpl::debugMarkerSetObjectTagEXT(const vk::DebugMarkerObjectTagInfoEXT& tagInfo) const {
  return vkDevice_.debugMarkerSetObjectTagEXT(tagInfo, getDispatcher());
}

vk::ResultValueType<void>::type LogicalDeviceImpl::waitIdle() const {
  return vkDevice_.waitIdle(getDispatcher());
}

vk::ResultValueType<void>::type
  LogicalDeviceImpl::displayPowerControlEXT(const vk::DisplayKHR& display,
                                            const vk::DisplayPowerInfoEXT& powerInfo) const {
  return vkDevice_.displayPowerControlEXT(display, powerInfo, getDispatcher());
}

uint32_t LogicalDeviceImpl::getImageViewHandleNVX(const vk::ImageViewHandleInfoNVX& handleInfo) const {
  return vkDevice_.getImageViewHandleNVX(handleInfo, getDispatcher());
}

vk::ResultValueType<void>::type
  LogicalDeviceImpl::flushMappedMemoryRanges(const vk::ArrayProxy<const vk::MappedMemoryRange>& memoryRanges) const {
  return vkDevice_.flushMappedMemoryRanges(memoryRanges, getDispatcher());
}

vk::ResultValueType<void>::type LogicalDeviceImpl::invalidateMappedMemoryRanges(
  const vk::ArrayProxy<const vk::MappedMemoryRange>& memoryRanges) const {
  return vkDevice_.invalidateMappedMemoryRanges(memoryRanges, getDispatcher());
}

vk::ResultValueType<uint64_t>::type LogicalDeviceImpl::getCalibratedTimestampsEXT(
  const vk::ArrayProxy<const vk::CalibratedTimestampInfoEXT>& timestampInfos,
  const vk::ArrayProxy<uint64_t>& timestamps) const {
  return vkDevice_.getCalibratedTimestampsEXT(timestampInfos, timestamps, getDispatcher());
}

vk::DescriptorSetLayoutSupport
  LogicalDeviceImpl::getDescriptorSetLayoutSupport(const vk::DescriptorSetLayoutCreateInfo& createInfo) const {
  return vkDevice_.getDescriptorSetLayoutSupport(createInfo, getDispatcher());
}

vk::DescriptorSetLayoutSupportKHR
  LogicalDeviceImpl::getDescriptorSetLayoutSupportKHR(const vk::DescriptorSetLayoutCreateInfo& createInfo) const {
  return vkDevice_.getDescriptorSetLayoutSupportKHR(createInfo, getDispatcher());
}

vk::PeerMemoryFeatureFlags LogicalDeviceImpl::getGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex,
                                                                         uint32_t remoteDeviceIndex) const {
  return vkDevice_.getGroupPeerMemoryFeatures(heapIndex, localDeviceIndex, remoteDeviceIndex, getDispatcher());
}

vk::PeerMemoryFeatureFlagsKHR LogicalDeviceImpl::getGroupPeerMemoryFeaturesKHR(uint32_t heapIndex,
                                                                               uint32_t localDeviceIndex,
                                                                               uint32_t remoteDeviceIndex) const {
  return vkDevice_.getGroupPeerMemoryFeaturesKHR(heapIndex, localDeviceIndex, remoteDeviceIndex, getDispatcher());
}

vk::ResultValueType<vk::DeviceGroupPresentCapabilitiesKHR>::type
  LogicalDeviceImpl::getGroupPresentCapabilitiesKHR() const {
  return vkDevice_.getGroupPresentCapabilitiesKHR(getDispatcher());
}

#ifdef VK_USE_PLATFORM_WIN32_KHR
vk::ResultValueType<vk::DeviceGroupPresentModeFlagsKHR>::type
  LogicalDeviceImpl::getGroupSurfacePresentModes2EXT(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const {
  return vkDevice_.getGroupSurfacePresentModes2EXT(surfaceInfo, getDispatcher();
}

vk::ResultValueType<vk::DeviceGroupPresentModeFlagsKHR>::type
  LogicalDeviceImpl::getGroupSurfacePresentModes2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const {
  return vkDevice_.getGroupSurfacePresentModes2KHR(surfaceInfo, getDispatcher();
}
#endif

PFN_vkVoidFunction LogicalDeviceImpl::getProcAddr(const std::string& name) const {
  return vkDevice_.getProcAddr(name, getDispatcher());
}

VulkanInstanceImpl& LogicalDeviceImpl::getInstance() const {
  return physicalDevice_.getInstance();
}

PhysicalDeviceImpl& LogicalDeviceImpl::getPhysicalDevice() const {
  return physicalDevice_;
}

const vk::DispatchLoaderDynamic& LogicalDeviceImpl::getDispatcher() const {
  return dispatcher_;
}

void LogicalDeviceImpl::destroy() const {
  physicalDevice_.destroyLogicalDevice(id());
}

LogicalDeviceImpl::operator const vk::Device&() const {
  return vkDevice_;
}

void LogicalDeviceImpl::free() {
  VulkanObjectComposite<QueueFamilyImpl>::destroyAllObjects();
  VulkanObjectComposite<BufferImpl>::destroyAllObjects();
  VulkanObjectComposite<ImageImpl>::destroyAllObjects();
  VulkanObjectComposite<AccelerationStructureNVImpl>::destroyAllObjects();
  VulkanObjectComposite<MemoryAllocatorImpl>::destroyAllObjects();
  VulkanObjectComposite<DeviceMemoryImpl>::destroyAllObjects();
  VulkanObjectComposite<SwapchainKHRImpl>::destroyAllObjects();
  VulkanObjectComposite<SamplerImpl>::destroyAllObjects();
  VulkanObjectComposite<QueryPoolImpl>::destroyAllObjects();
  VulkanObjectComposite<EventImpl>::destroyAllObjects();
  VulkanObjectComposite<FenceImpl>::destroyAllObjects();
  VulkanObjectComposite<SemaphoreImpl>::destroyAllObjects();
  VulkanObjectComposite<ShaderModuleImpl>::destroyAllObjects();
  VulkanObjectComposite<PipelineCacheImpl>::destroyAllObjects();
  VulkanObjectComposite<DescriptorSetLayoutImpl>::destroyAllObjects();
  VulkanObjectComposite<DescriptorPoolImpl>::destroyAllObjects();
  VulkanObjectComposite<DescriptorUpdateTemplateImpl>::destroyAllObjects();
  VulkanObjectComposite<PipelineLayoutImpl>::destroyAllObjects();
  VulkanObjectComposite<PipelineImpl>::destroyAllObjects();
  VulkanObjectComposite<RenderPassImpl>::destroyAllObjects();
  VulkanObjectComposite<FramebufferImpl>::destroyAllObjects();
  VulkanObjectComposite<ValidationCacheEXTImpl>::destroyAllObjects();
  VulkanObjectComposite<IndirectCommandsLayoutNVXImpl>::destroyAllObjects();
  VulkanObjectComposite<ObjectTableNVXImpl>::destroyAllObjects();
  vkDevice_.destroy(allocator_ ? &allocator_.value() : nullptr, dispatcher_);
  vkDevice_ = nullptr;
  VulkanObject::free();
}

} // namespace logi
