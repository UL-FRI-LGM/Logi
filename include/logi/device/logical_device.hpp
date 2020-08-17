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

#ifndef LOGI_DEVICE_LOGICAL_DEVICE_HPP
#define LOGI_DEVICE_LOGICAL_DEVICE_HPP

#include "logi/base/handle.hpp"
#include "logi/command/command_pool.hpp"
#include "logi/descriptor/descriptor_pool.hpp"
#include "logi/descriptor/descriptor_set.hpp"
#include "logi/descriptor/descriptor_update_template.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/memory/acceleration_structure_nv.hpp"
#include "logi/memory/buffer.hpp"
#include "logi/memory/device_memory.hpp"
#include "logi/memory/image.hpp"
#include "logi/memory/memory_allocator.hpp"
#include "logi/memory/sampler.hpp"
#include "logi/memory/sampler_ycbcr_conversion.hpp"
#include "logi/nvidia/indirect_commands_layout_nvx.hpp"
#include "logi/nvidia/object_table_nvx.hpp"
#include "logi/program/descriptor_set_layout.hpp"
#include "logi/program/pipeline.hpp"
#include "logi/program/pipeline_cache.hpp"
#include "logi/program/pipeline_layout.hpp"
#include "logi/program/shader_module.hpp"
#include "logi/program/validation_cache_ext.hpp"
#include "logi/query/query_pool.hpp"
#include "logi/queue/queue_family.hpp"
#include "logi/render_pass/framebuffer.hpp"
#include "logi/render_pass/render_pass.hpp"
#include "logi/swapchain/swapchain_khr.hpp"
#include "logi/synchronization/event.hpp"
#include "logi/synchronization/fence.hpp"
#include "logi/synchronization/semaphore.hpp"

namespace logi {

class LogicalDevice : public Handle<LogicalDeviceImpl> {
 public:
  using Handle::Handle;

  // region Sub-Handles

  MemoryAllocator createMemoryAllocator(vk::DeviceSize preferredLargeHeapBlockSize = 0u, uint32_t frameInUseCount = 0u,
                                        const std::vector<vk::DeviceSize>& heapSizeLimits = {},
                                        const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyMemoryAllocator(const MemoryAllocator& memoryAllocator) const;

  DeviceMemory allocateMemory(const vk::MemoryAllocateInfo& createInfo,
                              const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void freeMemory(const DeviceMemory& deviceMemory) const;b

  Buffer createBuffer(const vk::BufferCreateInfo& createInfo,
                      const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyBuffer(const Buffer& buffer) const;

  Image createImage(const vk::ImageCreateInfo& createInfo,
                    const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyImage(const Image& image) const;

  Sampler createSampler(const vk::SamplerCreateInfo& createInfo,
                        const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroySampler(const Sampler& sampler) const;

  SamplerYcbcrConversion
    createSamplerYcbcrConversion(const vk::SamplerYcbcrConversionCreateInfo& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroySamplerYcbcrConversion(const SamplerYcbcrConversion& samplerYcbcrConversion) const;

  ShaderModule createShaderModule(const vk::ShaderModuleCreateInfo& createInfo,
                                  const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyShaderModule(const ShaderModule& shaderModule) const;

  PipelineCache createPipelineCache(const vk::PipelineCacheCreateInfo& createInfo,
                                    const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyPipelineCache(const PipelineCache& pipelineCache) const;

  DescriptorSetLayout createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& createInfo,
                                                const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyDescriptorSetLayout(const DescriptorSetLayout& descriptorSetLayout) const;

  DescriptorPool createDescriptorPool(const vk::DescriptorPoolCreateInfo& createInfo,
                                      const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyDescriptorPool(const DescriptorPool& descriptorPool) const;

  PipelineLayout createPipelineLayout(const vk::PipelineLayoutCreateInfo& createInfo,
                                      const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyPipelineLayout(const PipelineLayout& pipelineLayout) const;

  DescriptorUpdateTemplate
    createDescriptorUpdateTemplate(const vk::DescriptorUpdateTemplateCreateInfo& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyDescriptorUpdateTemplate(const DescriptorUpdateTemplate& descriptorUpdateTemplate) const;

  std::vector<Pipeline> createComputePipelines(const vk::ArrayProxy<const vk::ComputePipelineCreateInfo>& createInfos,
                                               const vk::PipelineCache& cache = nullptr,
                                               const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  Pipeline createComputePipeline(const vk::ComputePipelineCreateInfo& createInfo,
                                 const vk::PipelineCache& cache = nullptr,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  std::vector<Pipeline> createGraphicsPipelines(const vk::ArrayProxy<const vk::GraphicsPipelineCreateInfo>& createInfos,
                                                const vk::PipelineCache& cache = nullptr,
                                                const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  Pipeline createGraphicsPipeline(const vk::GraphicsPipelineCreateInfo& createInfo,
                                  const vk::PipelineCache& cache = nullptr,
                                  const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  std::vector<Pipeline>
    createRayTracingPipelinesNV(const vk::ArrayProxy<const vk::RayTracingPipelineCreateInfoNV>& createInfos,
                                const vk::PipelineCache& cache = nullptr,
                                const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  Pipeline createRayTracingPipelineNV(const vk::RayTracingPipelineCreateInfoNV& createInfo,
                                      const vk::PipelineCache& cache = nullptr,
                                      const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyPipeline(const Pipeline& pipeline) const;

  QueryPool createQueryPool(const vk::QueryPoolCreateInfo& createInfo,
                            const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyQueryPool(const QueryPool& queryPool) const;

  Event createEvent(const vk::EventCreateInfo& createInfo,
                    const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyEvent(const Event& event) const;

  Fence createFence(const vk::FenceCreateInfo& createInfo,
                    const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  Fence registerEventEXT(const vk::DeviceEventInfoEXT& eventInfo,
                         const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  Fence registerDisplayEventEXT(const vk::DisplayKHR& display, const vk::DisplayEventInfoEXT& eventInfo,
                                const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyFence(const Fence& fence) const;

  Semaphore createSemaphore(const vk::SemaphoreCreateInfo& createInfo,
                            const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroySemaphore(const Semaphore& semaphore) const;

  RenderPass createRenderPass(const vk::RenderPassCreateInfo& createInfo,
                              const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  RenderPass createRenderPass2(const vk::RenderPassCreateInfo2& createInfo, 
                               const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  RenderPass createRenderPass(const vk::RenderPassCreateInfo2KHR& createInfo,
                              const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyRenderPass(const RenderPass& renderPass) const;

  Framebuffer createFramebuffer(const vk::FramebufferCreateInfo& createInfo,
                                const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyFramebuffer(const Framebuffer& framebuffer) const;

  SwapchainKHR createSwapchainKHR(const vk::SwapchainCreateInfoKHR& createInfo,
                                  const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  std::vector<SwapchainKHR>
    createSharedSwapchainsKHR(const vk::ArrayProxy<const vk::SwapchainCreateInfoKHR>& createInfos,
                              const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroySwapchainKHR(const SwapchainKHR& swapchain) const;

  ValidationCacheEXT createValidationCacheEXT(const vk::ValidationCacheCreateInfoEXT& createInfo,
                                              const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyValidationCacheEXT(const ValidationCacheEXT& validationCacheExt) const;

  AccelerationStructureNV
    createAccelerationStructureNV(const vk::AccelerationStructureCreateInfoNV& createInfo,
                                  const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyAccelerationStructureNV(const AccelerationStructureNV& accelerationStructure) const;

  IndirectCommandsLayoutNVX createIndirectCommandsLayoutNVXIndirectCommandsLayoutNVX(
    const vk::IndirectCommandsLayoutCreateInfoNVX& createInfo,
    const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyIndirectCommandsLayoutNVX(const IndirectCommandsLayoutNVX& indirectCommandsLayout) const;

  ObjectTableNVX createObjectTableNVX(const vk::ObjectTableCreateInfoNVX& createInfo,
                                      const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  void destroyObjectTableNVX(const ObjectTableNVX& objectTable) const;

  std::vector<QueueFamily> enumerateQueueFamilies() const;

  // endregion

  // region Vulkan Declarations

  void updateDescriptorSets(const vk::ArrayProxy<const vk::WriteDescriptorSet>& descriptorWrites,
                            const vk::ArrayProxy<const vk::CopyDescriptorSet>& descriptorCopies = {}) const;

  vk::ResultValueType<void>::type setDebugUtilsObjectNameEXT(const vk::DebugUtilsObjectNameInfoEXT& nameInfo) const;

  vk::ResultValueType<void>::type setDebugUtilsObjectTagEXT(const vk::DebugUtilsObjectTagInfoEXT& tagInfo) const;

  vk::ResultValueType<void>::type debugMarkerSetObjectNameEXT(const vk::DebugMarkerObjectNameInfoEXT& nameInfo) const;

  vk::ResultValueType<void>::type debugMarkerSetObjectTagEXT(const vk::DebugMarkerObjectTagInfoEXT& tagInfo) const;

  vk::ResultValueType<void>::type waitIdle() const;

  vk::ResultValueType<void>::type displayPowerControlEXT(const vk::DisplayKHR& display,
                                                         const vk::DisplayPowerInfoEXT& powerInfo) const;

  uint32_t getImageViewHandleNVX(const vk::ImageViewHandleInfoNVX& handleInfo) const;

  vk::ResultValueType<void>::type
    flushMappedMemoryRanges(const vk::ArrayProxy<const vk::MappedMemoryRange>& memoryRanges = {}) const;

  vk::ResultValueType<void>::type
    invalidateMappedMemoryRanges(const vk::ArrayProxy<const vk::MappedMemoryRange>& memoryRanges = {}) const;

  vk::ResultValueType<uint64_t>::type
    getCalibratedTimestampsEXT(const vk::ArrayProxy<const vk::CalibratedTimestampInfoEXT>& timestampInfos,
                               const vk::ArrayProxy<uint64_t>& timestamps) const;

  vk::DescriptorSetLayoutSupport
    getDescriptorSetLayoutSupport(const vk::DescriptorSetLayoutCreateInfo& createInfo) const;

  vk::DescriptorSetLayoutSupportKHR
    getDescriptorSetLayoutSupportKHR(const vk::DescriptorSetLayoutCreateInfo& createInfo) const;

  vk::PeerMemoryFeatureFlags getGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex,
                                                        uint32_t remoteDeviceIndex) const;

  vk::PeerMemoryFeatureFlagsKHR getGroupPeerMemoryFeaturesKHR(uint32_t heapIndex, uint32_t localDeviceIndex,
                                                              uint32_t remoteDeviceIndex) const;

  vk::ResultValueType<vk::DeviceGroupPresentCapabilitiesKHR>::type getGroupPresentCapabilitiesKHR() const;

#ifdef VK_USE_PLATFORM_WIN32_KHR
  vk::ResultValueType<vk::DeviceGroupPresentModeFlagsKHR>::type
    getGroupSurfacePresentModes2EXT(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const;

  vk::ResultValueType<vk::DeviceGroupPresentModeFlagsKHR>::type
    getGroupSurfacePresentModes2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const;
#endif

  PFN_vkVoidFunction getProcAddr(const std::string& name) const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::Device&() const;
};

} // namespace logi

#endif // LOGI_DEVICE_LOGICAL_DEVICE_HPP
