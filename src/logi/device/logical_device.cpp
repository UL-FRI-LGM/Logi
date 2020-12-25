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

#include "logi/device/logical_device.hpp"
#include "logi/command/command_pool_impl.hpp"
#include "logi/descriptor/descriptor_pool_impl.hpp"
#include "logi/descriptor/descriptor_update_template_impl.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/memory/acceleration_structure_nv_impl.hpp"
#include "logi/memory/acceleration_structure_khr_impl.hpp"
#include "logi/memory/buffer_impl.hpp"
#include "logi/memory/device_memory_impl.hpp"
#include "logi/memory/image_impl.hpp"
#include "logi/memory/memory_allocator_impl.hpp"
#include "logi/memory/sampler_impl.hpp"
#include "logi/nvidia/indirect_commands_layout_nv_impl.hpp"
// #include "logi/nvidia/object_table_nvx_impl.hpp"
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

// region Sub-Handles

MemoryAllocator LogicalDevice::createMemoryAllocator(vk::DeviceSize preferredLargeHeapBlockSize,
                                                     uint32_t frameInUseCount,
                                                     const std::vector<vk::DeviceSize>& heapSizeLimits,
                                                     const std::optional<vk::AllocationCallbacks>& allocator) const {
  return MemoryAllocator(
    object_->createMemoryAllocator(preferredLargeHeapBlockSize, frameInUseCount, heapSizeLimits, allocator));
}

void LogicalDevice::destroyMemoryAllocator(const MemoryAllocator& memoryAllocator) const {
  object_->destroyMemoryAllocator(memoryAllocator.id());
}

DeviceMemory LogicalDevice::allocateMemory(const vk::MemoryAllocateInfo& createInfo,
                                           const std::optional<vk::AllocationCallbacks>& allocator) const {
  return DeviceMemory(object_->allocateMemory(createInfo, allocator));
}

void LogicalDevice::freeMemory(const DeviceMemory& deviceMemory) const {
  object_->freeMemory(deviceMemory.id());
}

Buffer LogicalDevice::createBuffer(const vk::BufferCreateInfo& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator) const {
  return Buffer(object_->createBuffer(createInfo, allocator));
}

void LogicalDevice::destroyBuffer(const Buffer& buffer) const {
  object_->destroyBuffer(buffer.id());
}

Image LogicalDevice::createImage(const vk::ImageCreateInfo& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator) const {
  return Image(object_->createImage(createInfo, allocator));
}

void LogicalDevice::destroyImage(const Image& image) const {
  object_->destroyImage(image.id());
}

Sampler LogicalDevice::createSampler(const vk::SamplerCreateInfo& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator) const {
  return Sampler(object_->createSampler(createInfo, allocator));
}

void LogicalDevice::destroySampler(const Sampler& sampler) const {
  object_->destroySampler(sampler.id());
}

SamplerYcbcrConversion
  LogicalDevice::createSamplerYcbcrConversion(const vk::SamplerYcbcrConversionCreateInfo& createInfo,
                                              const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SamplerYcbcrConversion(object_->createSamplerYcbcrConversion(createInfo, allocator));
}
void LogicalDevice::destroySamplerYcbcrConversion(const SamplerYcbcrConversion& samplerYcbcrConversion) const {
  object_->destroySamplerYcbcrConversion(samplerYcbcrConversion.id());
}

ShaderModule LogicalDevice::createShaderModule(const vk::ShaderModuleCreateInfo& createInfo,
                                               const std::optional<vk::AllocationCallbacks>& allocator) const {
  return ShaderModule(object_->createShaderModule(createInfo, allocator));
}

void LogicalDevice::destroyShaderModule(const ShaderModule& shaderModule) const {
  object_->destroyShaderModule(shaderModule.id());
}

PipelineCache LogicalDevice::createPipelineCache(const vk::PipelineCacheCreateInfo& createInfo,
                                                 const std::optional<vk::AllocationCallbacks>& allocator) const {
  return PipelineCache(object_->createPipelineCache(createInfo, allocator));
}

void LogicalDevice::destroyPipelineCache(const PipelineCache& pipelineCache) const {
  object_->destroyPipelineCache(pipelineCache.id());
}

DescriptorSetLayout
  LogicalDevice::createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& createInfo,
                                           const std::optional<vk::AllocationCallbacks>& allocator) const {
  return DescriptorSetLayout(object_->createDescriptorSetLayout(createInfo, allocator));
}

void LogicalDevice::destroyDescriptorSetLayout(const DescriptorSetLayout& descriptorSetLayout) const {
  object_->destroyDescriptorSetLayout(descriptorSetLayout.id());
}

DescriptorPool LogicalDevice::createDescriptorPool(const vk::DescriptorPoolCreateInfo& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) const {
  return DescriptorPool(object_->createDescriptorPool(createInfo, allocator));
}

void LogicalDevice::destroyDescriptorPool(const DescriptorPool& descriptorPool) const {
  object_->destroyDescriptorPool(descriptorPool.id());
}

PipelineLayout LogicalDevice::createPipelineLayout(const vk::PipelineLayoutCreateInfo& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) const {
  return PipelineLayout(object_->createPipelineLayout(createInfo, allocator));
}

void LogicalDevice::destroyPipelineLayout(const PipelineLayout& pipelineLayout) const {
  object_->destroyPipelineLayout(pipelineLayout.id());
}

DescriptorUpdateTemplate
  LogicalDevice::createDescriptorUpdateTemplate(const vk::DescriptorUpdateTemplateCreateInfo& createInfo,
                                                const std::optional<vk::AllocationCallbacks>& allocator) const {
  return DescriptorUpdateTemplate(object_->createDescriptorUpdateTemplate(createInfo, allocator));
}

void LogicalDevice::destroyDescriptorUpdateTemplate(const DescriptorUpdateTemplate& descriptorUpdateTemplate) const {
  object_->destroyDescriptorUpdateTemplate(descriptorUpdateTemplate.id());
}

std::vector<Pipeline>
  LogicalDevice::createComputePipelines(const vk::ArrayProxy<const vk::ComputePipelineCreateInfo>& createInfos,
                                        const vk::PipelineCache& cache,
                                        const std::optional<vk::AllocationCallbacks>& allocator) const {
  std::vector<std::shared_ptr<PipelineImpl>> pipelineImpls =
    object_->createComputePipelines(createInfos, cache, allocator);
  return std::vector<Pipeline>(pipelineImpls.begin(), pipelineImpls.end());
}

Pipeline LogicalDevice::createComputePipeline(const vk::ComputePipelineCreateInfo& createInfo,
                                              const vk::PipelineCache& cache,
                                              const std::optional<vk::AllocationCallbacks>& allocator) const {
  return Pipeline(object_->createComputePipeline(createInfo, cache, allocator));
}

std::vector<Pipeline>
  LogicalDevice::createGraphicsPipelines(const vk::ArrayProxy<const vk::GraphicsPipelineCreateInfo>& createInfos,
                                         const vk::PipelineCache& cache,
                                         const std::optional<vk::AllocationCallbacks>& allocator) const {
  std::vector<std::shared_ptr<PipelineImpl>> pipelineImpls =
    object_->createGraphicsPipelines(createInfos, cache, allocator);
  return std::vector<Pipeline>(pipelineImpls.begin(), pipelineImpls.end());
}

Pipeline LogicalDevice::createGraphicsPipeline(const vk::GraphicsPipelineCreateInfo& createInfo,
                                               const vk::PipelineCache& cache,
                                               const std::optional<vk::AllocationCallbacks>& allocator) const {
  return Pipeline(object_->createGraphicsPipeline(createInfo, cache, allocator));
}

std::vector<Pipeline> LogicalDevice::createRayTracingPipelinesNV(
  const vk::ArrayProxy<const vk::RayTracingPipelineCreateInfoNV>& createInfos, const vk::PipelineCache& cache,
  const std::optional<vk::AllocationCallbacks>& allocator) const {
  std::vector<std::shared_ptr<PipelineImpl>> pipelineImpls =
    object_->createRayTracingPipelinesNV(createInfos, cache, allocator);
  return std::vector<Pipeline>(pipelineImpls.begin(), pipelineImpls.end());
}

Pipeline LogicalDevice::createRayTracingPipelineNV(const vk::RayTracingPipelineCreateInfoNV& createInfo,
                                                   const vk::PipelineCache& cache,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) const {
  return Pipeline(object_->createRayTracingPipelineNV(createInfo, cache, allocator));
}

void LogicalDevice::destroyPipeline(const Pipeline& pipeline) const {
  object_->destroyPipeline(pipeline.id());
}

QueryPool LogicalDevice::createQueryPool(const vk::QueryPoolCreateInfo& createInfo,
                                         const std::optional<vk::AllocationCallbacks>& allocator) const {
  return QueryPool(object_->createQueryPool(createInfo, allocator));
}

void LogicalDevice::destroyQueryPool(const QueryPool& queryPool) const {
  object_->destroyQueryPool(queryPool.id());
}

Event LogicalDevice::createEvent(const vk::EventCreateInfo& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator) const {
  return Event(object_->createEvent(createInfo, allocator));
}

void LogicalDevice::destroyEvent(const Event& event) const {
  object_->destroyEvent(event.id());
}

Fence LogicalDevice::createFence(const vk::FenceCreateInfo& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator) const {
  return Fence(object_->createFence(createInfo, allocator));
}

Fence LogicalDevice::registerEventEXT(const vk::DeviceEventInfoEXT& eventInfo,
                                      const std::optional<vk::AllocationCallbacks>& allocator) const {
  return Fence(object_->registerEventEXT(eventInfo, allocator));
}

Fence LogicalDevice::registerDisplayEventEXT(const vk::DisplayKHR& display, const vk::DisplayEventInfoEXT& eventInfo,
                                             const std::optional<vk::AllocationCallbacks>& allocator) const {
  return Fence(object_->registerDisplayEventEXT(display, eventInfo, allocator));
}

void LogicalDevice::destroyFence(const Fence& fence) const {
  object_->destroyFence(fence.id());
}

Semaphore LogicalDevice::createSemaphore(const vk::SemaphoreCreateInfo& createInfo,
                                         const std::optional<vk::AllocationCallbacks>& allocator) const {
  return Semaphore(object_->createSemaphore(createInfo, allocator));
}

void LogicalDevice::destroySemaphore(const Semaphore& semaphore) const {
  object_->destroySemaphore(semaphore.id());
}

void LogicalDevice::waitSemaphores(const vk::SemaphoreWaitInfo& waitInfo, uint64_t timeout) const {
  object_->waitSemaphores(waitInfo, timeout);
}

RenderPass LogicalDevice::createRenderPass(const vk::RenderPassCreateInfo& createInfo,
                                           const std::optional<vk::AllocationCallbacks>& allocator) const {
  return RenderPass(object_->createRenderPass(createInfo, allocator));
}

RenderPass LogicalDevice::createRenderPass2(const vk::RenderPassCreateInfo2& createInfo, 
                                            const std::optional<vk::AllocationCallbacks>& allocator) const {
  return RenderPass(object_->createRenderPass2(createInfo, allocator));
}

RenderPass LogicalDevice::createRenderPass(const vk::RenderPassCreateInfo2KHR& createInfo,
                                           const std::optional<vk::AllocationCallbacks>& allocator) const {
  return RenderPass(object_->createRenderPass(createInfo, allocator));
}

void LogicalDevice::destroyRenderPass(const RenderPass& renderPass) const {
  object_->destroyRenderPass(renderPass.id());
}

Framebuffer LogicalDevice::createFramebuffer(const vk::FramebufferCreateInfo& createInfo,
                                             const std::optional<vk::AllocationCallbacks>& allocator) const {
  return Framebuffer(object_->createFramebuffer(createInfo, allocator));
}

void LogicalDevice::destroyFramebuffer(const Framebuffer& framebuffer) const {
  object_->destroyFramebuffer(framebuffer.id());
}

SwapchainKHR LogicalDevice::createSwapchainKHR(const vk::SwapchainCreateInfoKHR& createInfo,
                                               const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SwapchainKHR(object_->createSwapchainKHR(createInfo, allocator));
}

std::vector<SwapchainKHR>
  LogicalDevice::createSharedSwapchainsKHR(const vk::ArrayProxy<const vk::SwapchainCreateInfoKHR>& createInfos,
                                           const std::optional<vk::AllocationCallbacks>& allocator) const {
  std::vector<std::shared_ptr<SwapchainKHRImpl>> swapchainImpls =
    object_->createSharedSwapchainsKHR(createInfos, allocator);
  return std::vector<SwapchainKHR>(swapchainImpls.begin(), swapchainImpls.end());
}

void LogicalDevice::destroySwapchainKHR(const SwapchainKHR& swapchain) const {
  object_->destroySwapchainKHR(swapchain.id());
}

AccelerationStructureKHR 
  LogicalDevice::createAccelerationStructureKHR(const vk::AccelerationStructureCreateInfoKHR& createInfo,
                                                const std::optional<vk::AllocationCallbacks>& allocator) const {
  return AccelerationStructureKHR(object_->createAccelerationStructureKHR(createInfo, allocator));
}

void LogicalDevice::destroyAccelerationStructureKHR(const AccelerationStructureKHR& accelerationStructure) const {
  object_->destroyAccelerationStructureKHR(accelerationStructure.id());
}     

vk::Result LogicalDevice::buildAccelerationStructuresKHR(vk::DeferredOperationKHR deferredOperation, 
                                                         const vk::ArrayProxy<const vk::AccelerationStructureBuildGeometryInfoKHR> &infos,
                                                         const vk::ArrayProxy<const vk::AccelerationStructureBuildRangeInfoKHR *const> &pBuildRangeInfos) const {
  return object_->buildAccelerationStructuresKHR(deferredOperation, infos, pBuildRangeInfos); 
}

vk::AccelerationStructureCompatibilityKHR 
    LogicalDevice::getAccelerationStructureCompatibilityKHR(const vk::AccelerationStructureVersionInfoKHR &versionInfo) const {
  return object_->getAccelerationStructureCompatibilityKHR(versionInfo);
}

ValidationCacheEXT
  LogicalDevice::createValidationCacheEXT(const vk::ValidationCacheCreateInfoEXT& createInfo,
                                          const std::optional<vk::AllocationCallbacks>& allocator) const {
  return ValidationCacheEXT(object_->createValidationCacheEXT(createInfo, allocator));
}

void LogicalDevice::destroyValidationCacheEXT(const ValidationCacheEXT& validationCacheExt) const {
  object_->destroyValidationCacheEXT(validationCacheExt.id());
}

AccelerationStructureNV
  LogicalDevice::createAccelerationStructureNV(const vk::AccelerationStructureCreateInfoNV& createInfo,
                                               const std::optional<vk::AllocationCallbacks>& allocator) const {
  return AccelerationStructureNV(object_->createAccelerationStructureNV(createInfo, allocator));
}

void LogicalDevice::destroyAccelerationStructureNV(const AccelerationStructureNV& accelerationStructure) const {
  object_->destroyAccelerationStructureNV(accelerationStructure.id());
}

IndirectCommandsLayoutNV LogicalDevice::createIndirectCommandsLayoutNV(
  const vk::IndirectCommandsLayoutCreateInfoNV& createInfo,
  const std::optional<vk::AllocationCallbacks>& allocator) const {
  return IndirectCommandsLayoutNV(
    object_->createIndirectCommandsLayoutNV(createInfo, allocator));
}

void LogicalDevice::destroyIndirectCommandsLayoutNV(const IndirectCommandsLayoutNV& indirectCommandsLayout) const {
  object_->destroyIndirectCommandsLayoutNV(indirectCommandsLayout.id());
}

// ObjectTableNVX LogicalDevice::createObjectTableNVX(const vk::ObjectTableCreateInfoNVX& createInfo,
//                                                    const std::optional<vk::AllocationCallbacks>& allocator) const {
//   return ObjectTableNVX(object_->createObjectTableNVX(createInfo, allocator));
// }

// void LogicalDevice::destroyObjectTableNVX(const ObjectTableNVX& objectTable) const {
//   object_->destroyObjectTableNVX(objectTable.id());
// }

std::vector<QueueFamily> LogicalDevice::enumerateQueueFamilies() const {
  std::vector<std::shared_ptr<QueueFamilyImpl>> queueFamilyImpls = object_->enumerateQueueFamilies();
  return std::vector<QueueFamily>(queueFamilyImpls.begin(), queueFamilyImpls.end());
}

// endregion

// region Vulkan definitions

void LogicalDevice::updateDescriptorSets(const vk::ArrayProxy<const vk::WriteDescriptorSet>& descriptorWrites,
                                         const vk::ArrayProxy<const vk::CopyDescriptorSet>& descriptorCopies) const {
  object_->updateDescriptorSets(descriptorWrites, descriptorCopies);
}

vk::ResultValueType<void>::type
  LogicalDevice::setDebugUtilsObjectNameEXT(const vk::DebugUtilsObjectNameInfoEXT& nameInfo) const {
  return object_->setDebugUtilsObjectNameEXT(nameInfo);
}

vk::ResultValueType<void>::type
  LogicalDevice::setDebugUtilsObjectTagEXT(const vk::DebugUtilsObjectTagInfoEXT& tagInfo) const {
  return object_->setDebugUtilsObjectTagEXT(tagInfo);
}

vk::ResultValueType<void>::type
  LogicalDevice::debugMarkerSetObjectNameEXT(const vk::DebugMarkerObjectNameInfoEXT& nameInfo) const {
  return object_->debugMarkerSetObjectNameEXT(nameInfo);
}

vk::ResultValueType<void>::type
  LogicalDevice::debugMarkerSetObjectTagEXT(const vk::DebugMarkerObjectTagInfoEXT& tagInfo) const {
  return object_->debugMarkerSetObjectTagEXT(tagInfo);
}

vk::ResultValueType<void>::type LogicalDevice::waitIdle() const {
  return object_->waitIdle();
}

vk::ResultValueType<void>::type LogicalDevice::displayPowerControlEXT(const vk::DisplayKHR& display,
                                                                      const vk::DisplayPowerInfoEXT& powerInfo) const {
  return object_->displayPowerControlEXT(display, powerInfo);
}

uint32_t LogicalDevice::getImageViewHandleNVX(const vk::ImageViewHandleInfoNVX& handleInfo) const {
  return object_->getImageViewHandleNVX(handleInfo);
}

vk::ResultValueType<void>::type
  LogicalDevice::flushMappedMemoryRanges(const vk::ArrayProxy<const vk::MappedMemoryRange>& memoryRanges) const {
  return object_->flushMappedMemoryRanges(memoryRanges);
}

vk::ResultValueType<void>::type
  LogicalDevice::invalidateMappedMemoryRanges(const vk::ArrayProxy<const vk::MappedMemoryRange>& memoryRanges) const {
  return object_->invalidateMappedMemoryRanges(memoryRanges);
}

vk::ResultValueType<uint64_t>::type
  LogicalDevice::getCalibratedTimestampsEXT(const vk::ArrayProxy<const vk::CalibratedTimestampInfoEXT>& timestampInfos,
                                            const vk::ArrayProxy<uint64_t>& timestamps) const {
  return object_->getCalibratedTimestampsEXT(timestampInfos, timestamps);
}

vk::DescriptorSetLayoutSupport
  LogicalDevice::getDescriptorSetLayoutSupport(const vk::DescriptorSetLayoutCreateInfo& createInfo) const {
  return object_->getDescriptorSetLayoutSupport(createInfo);
}

vk::DescriptorSetLayoutSupportKHR
  LogicalDevice::getDescriptorSetLayoutSupportKHR(const vk::DescriptorSetLayoutCreateInfo& createInfo) const {
  return object_->getDescriptorSetLayoutSupportKHR(createInfo);
}

vk::PeerMemoryFeatureFlags LogicalDevice::getGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex,
                                                                     uint32_t remoteDeviceIndex) const {
  return object_->getGroupPeerMemoryFeatures(heapIndex, localDeviceIndex, remoteDeviceIndex);
}

vk::PeerMemoryFeatureFlagsKHR LogicalDevice::getGroupPeerMemoryFeaturesKHR(uint32_t heapIndex,
                                                                           uint32_t localDeviceIndex,
                                                                           uint32_t remoteDeviceIndex) const {
  return object_->getGroupPeerMemoryFeaturesKHR(heapIndex, localDeviceIndex, remoteDeviceIndex);
}

vk::ResultValueType<vk::DeviceGroupPresentCapabilitiesKHR>::type LogicalDevice::getGroupPresentCapabilitiesKHR() const {
  return object_->getGroupPresentCapabilitiesKHR();
}

#ifdef VK_USE_PLATFORM_WIN32_KHR
vk::ResultValueType<vk::DeviceGroupPresentModeFlagsKHR>::type
  LogicalDevice::getGroupSurfacePresentModes2EXT(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const {
  return object->getGroupSurfacePresentModes2EXT(surfaceInfo);
}

vk::ResultValueType<vk::DeviceGroupPresentModeFlagsKHR>::type
  LogicalDevice::getGroupSurfacePresentModes2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const {
  return object->getGroupSurfacePresentModes2KHR(surfaceInfo);
}
#endif

PFN_vkVoidFunction LogicalDevice::getProcAddr(const std::string& name) const {
  return object_->getProcAddr(name);
}

// endregion

// region Logi definitions

VulkanInstance LogicalDevice::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice LogicalDevice::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& LogicalDevice::getDispatcher() const {
  return object_->getDispatcher();
}

void LogicalDevice::destroy() const {
  if (object_) {
    object_->destroy();
  }
}

LogicalDevice::operator const vk::Device&() const {
  static vk::Device nullHandle(nullptr);
  return (object_) ? object_->operator const vk::Device&() : nullHandle;
}

// endregion

}