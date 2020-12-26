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
#include "logi/memory/acceleration_structure_khr.hpp"
#include "logi/memory/buffer.hpp"
#include "logi/memory/device_memory.hpp"
#include "logi/memory/image.hpp"
#include "logi/memory/memory_allocator.hpp"
#include "logi/memory/sampler.hpp"
#include "logi/memory/sampler_ycbcr_conversion.hpp"
#include "logi/nvidia/indirect_commands_layout_nv.hpp"
// #include "logi/nvidia/object_table_nvx.hpp"
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
#include "logi/synchronization/deferred_operation_khr.hpp"

namespace logi {

class LogicalDevice : public Handle<LogicalDeviceImpl> {
 public:
  using Handle::Handle;

  // region Sub-Handles

  // TODO: documentation for allocator!
  MemoryAllocator createMemoryAllocator(vk::DeviceSize preferredLargeHeapBlockSize = 0u, uint32_t frameInUseCount = 0u,
                                        const std::vector<vk::DeviceSize>& heapSizeLimits = {},
                                        const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
  // TODO: documentation for allocator!
  void destroyMemoryAllocator(const MemoryAllocator& memoryAllocator) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkAllocateMemory.html">vkAllocateMemory</a>
   */
  DeviceMemory allocateMemory(const vk::MemoryAllocateInfo& createInfo,
                              const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkFreeMemory.html">vkFreeMemory</a>
   */
  void freeMemory(const DeviceMemory& deviceMemory) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateBuffer.html">vkCreateBuffer</a>
   */
  Buffer createBuffer(const vk::BufferCreateInfo& createInfo,
                      const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyBuffer.html">vkDestroyBuffer</a>
   */
  void destroyBuffer(const Buffer& buffer) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateImage.html">vkCreateImage</a>
   */
  Image createImage(const vk::ImageCreateInfo& createInfo,
                    const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyImage.html">vkDestroyImage</a>
   */
  void destroyImage(const Image& image) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateSampler.html">vkCreateSampler</a>
   */
  Sampler createSampler(const vk::SamplerCreateInfo& createInfo,
                        const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroySampler.html">vkDestroySampler</a>
   */
  void destroySampler(const Sampler& sampler) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateSamplerYcbcrConversion.html">vkCreateSamplerYcbcrConversion</a>
   */
  SamplerYcbcrConversion
    createSamplerYcbcrConversion(const vk::SamplerYcbcrConversionCreateInfo& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroySamplerYcbcrConversion.html">vkDestroySamplerYcbcrConversion</a>
   */
  void destroySamplerYcbcrConversion(const SamplerYcbcrConversion& samplerYcbcrConversion) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateShaderModule.html">vkCreateShaderModule</a>
   */
  ShaderModule createShaderModule(const vk::ShaderModuleCreateInfo& createInfo,
                                  const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyShaderModule.html">vkDestroyShaderModule</a>
   */
  void destroyShaderModule(const ShaderModule& shaderModule) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreatePipelineCache.html">vkCreatePipelineCache</a>
   */
  PipelineCache createPipelineCache(const vk::PipelineCacheCreateInfo& createInfo,
                                    const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyPipelineCache.html">vkDestroyPipelineCache</a>
   */
  void destroyPipelineCache(const PipelineCache& pipelineCache) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateDescriptorSetLayout.html">vkCreateDescriptorSetLayout</a>
   */
  DescriptorSetLayout createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& createInfo,
                                                const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyDescriptorSetLayout.html">vkDestroyDescriptorSetLayout</a>
   */
  void destroyDescriptorSetLayout(const DescriptorSetLayout& descriptorSetLayout) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateDescriptorPool.html">vkCreateDescriptorPool</a>
   */
  DescriptorPool createDescriptorPool(const vk::DescriptorPoolCreateInfo& createInfo,
                                      const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyDescriptorPool.html">vkDestroyDescriptorPool</a>
   */
  void destroyDescriptorPool(const DescriptorPool& descriptorPool) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreatePipelineLayout.html">vkCreatePipelineLayout</a>
   */
  PipelineLayout createPipelineLayout(const vk::PipelineLayoutCreateInfo& createInfo,
                                      const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyPipelineLayout.html">vkDestroyPipelineLayout</a>
   */
  void destroyPipelineLayout(const PipelineLayout& pipelineLayout) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateDescriptorUpdateTemplate.html">vkCreateDescriptorUpdateTemplate</a>
   */
  DescriptorUpdateTemplate
    createDescriptorUpdateTemplate(const vk::DescriptorUpdateTemplateCreateInfo& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyDescriptorUpdateTemplate.html">vkDestroyDescriptorUpdateTemplate</a>
   */
  void destroyDescriptorUpdateTemplate(const DescriptorUpdateTemplate& descriptorUpdateTemplate) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateComputePipelines.html">vkCreateComputePipelines</a>
   */
  std::vector<Pipeline> createComputePipelines(const vk::ArrayProxy<const vk::ComputePipelineCreateInfo>& createInfos,
                                               const vk::PipelineCache& cache = nullptr,
                                               const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateComputePipelines.html">vkCreateComputePipelines</a>
   */
  Pipeline createComputePipeline(const vk::ComputePipelineCreateInfo& createInfo,
                                 const vk::PipelineCache& cache = nullptr,
                                 const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateGraphicsPipelines.html">vkCreateGraphicsPipelines</a>
   */
  std::vector<Pipeline> createGraphicsPipelines(const vk::ArrayProxy<const vk::GraphicsPipelineCreateInfo>& createInfos,
                                                const vk::PipelineCache& cache = nullptr,
                                                const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateGraphicsPipelines.html">vkCreateGraphicsPipelines</a>
   */
  Pipeline createGraphicsPipeline(const vk::GraphicsPipelineCreateInfo& createInfo,
                                  const vk::PipelineCache& cache = nullptr,
                                  const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateRayTracingPipelinesKHR.html">vkCreateRayTracingPipelinesKHR</a>
   */
  std::vector<Pipeline> createRayTracingPipelinesKHR(const vk::DeferredOperationKHR deferredOperation, 
                                                     const vk::ArrayProxy<const vk::RayTracingPipelineCreateInfoKHR>& createInfos,
                                                     const vk::PipelineCache& pipelineCache = nullptr,
                                                     const std::optional<const vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateRayTracingPipelineKHR.html">vkCreateRayTracingPipelineKHR</a>
   */
  Pipeline createRayTracingPipelineKHR(const vk::DeferredOperationKHR deferredOperation, 
                                       const vk::RayTracingPipelineCreateInfoKHR& createInfo, 
                                       const vk::PipelineCache& pipelineCache = nullptr,
                                       const std::optional<const vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateRayTracingPipelinesNV.html">vkCreateRayTracingPipelinesNV</a>
   */
  std::vector<Pipeline>
    createRayTracingPipelinesNV(const vk::ArrayProxy<const vk::RayTracingPipelineCreateInfoNV>& createInfos,
                                const vk::PipelineCache& cache = nullptr,
                                const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateRayTracingPipelinesNV.html">vkCreateRayTracingPipelinesNV</a>
   */
  Pipeline createRayTracingPipelineNV(const vk::RayTracingPipelineCreateInfoNV& createInfo,
                                      const vk::PipelineCache& cache = nullptr,
                                      const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyPipeline.html">vkDestroyPipeline</a>
   */
  void destroyPipeline(const Pipeline& pipeline) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateQueryPool.html">vkCreateQueryPool</a>
   */
  QueryPool createQueryPool(const vk::QueryPoolCreateInfo& createInfo,
                            const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyQueryPool.html">vkDestroyQueryPool</a>
   */
  void destroyQueryPool(const QueryPool& queryPool) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyQueryPool.html">vkDestroyQueryPool</a>
   */
  Event createEvent(const vk::EventCreateInfo& createInfo,
                    const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyEvent.html">vkDestroyEvent</a>
   */
  void destroyEvent(const Event& event) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateFence.html">vkCreateFence</a>
   */
  Fence createFence(const vk::FenceCreateInfo& createInfo,
                    const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkRegisterDeviceEventEXT.html">vkRegisterDeviceEventEXT</a>
   */
  Fence registerEventEXT(const vk::DeviceEventInfoEXT& eventInfo,
                         const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkRegisterDisplayEventEXT.html">vkRegisterDisplayEventEXT</a>
   */
  Fence registerDisplayEventEXT(const vk::DisplayKHR& display, const vk::DisplayEventInfoEXT& eventInfo,
                                const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyFence.html">vkDestroyFence</a>
   */
  void destroyFence(const Fence& fence) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateSemaphore.html">vkCreateSemaphore</a>
   */
  Semaphore createSemaphore(const vk::SemaphoreCreateInfo& createInfo,
                            const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroySemaphore.html">vkDestroySemaphore</a>
   */
  void destroySemaphore(const Semaphore& semaphore) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkWaitSemaphores.html">vkWaitSemaphores</a>
   */
  void waitSemaphores(const vk::SemaphoreWaitInfo& waitInfo, uint64_t timeout) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateDeferredOperationKHR.html">vkCreateDeferredOperationKHR</a>
   */
  DeferredOperationKHR createDeferredOperationKHR(const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyDeferredOperationKHR.html">vkDestroyDeferredOperationKHR</a>
   */
  void destroyDeferredOperationKHR(const DeferredOperationKHR& deferredOperationKHR) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateRenderPass.html">vkCreateRenderPass</a>
   */
  RenderPass createRenderPass(const vk::RenderPassCreateInfo& createInfo,
                              const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateRenderPass2.html">vkCreateRenderPass2</a>
   */
  RenderPass createRenderPass2(const vk::RenderPassCreateInfo2& createInfo, 
                               const std::optional<vk::AllocationCallbacks>& allocator = {}) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateRenderPass2KHR.html">vkCreateRenderPass2KHR</a>
   */
  RenderPass createRenderPass(const vk::RenderPassCreateInfo2KHR& createInfo,
                              const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateRenderPass2KHR.html">vkCreateRenderPass2KHR</a>
   */
  void destroyRenderPass(const RenderPass& renderPass) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyRenderPass.html">vkDestroyRenderPass</a>
   */
  Framebuffer createFramebuffer(const vk::FramebufferCreateInfo& createInfo,
                                const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyFramebuffer.html">vkDestroyFramebuffer</a>
   */
  void destroyFramebuffer(const Framebuffer& framebuffer) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateSwapchainKHR.html">vkCreateSwapchainKHR</a>
   */
  SwapchainKHR createSwapchainKHR(const vk::SwapchainCreateInfoKHR& createInfo,
                                  const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateSwapchainKHR.html">vkCreateSwapchainKHR</a>
   */
  std::vector<SwapchainKHR>
    createSharedSwapchainsKHR(const vk::ArrayProxy<const vk::SwapchainCreateInfoKHR>& createInfos,
                              const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroySwapchainKHR.html">vkDestroySwapchainKHR</a>
   */
  void destroySwapchainKHR(const SwapchainKHR& swapchain) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateAccelerationStructureKHR.html">vkCreateAccelerationStructureKHR</a>
   */
  AccelerationStructureKHR 
    createAccelerationStructureKHR(const vk::AccelerationStructureCreateInfoKHR& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyAccelerationStructureKHR.html">vkDestroyAccelerationStructureKHR</a>
   */
  void destroyAccelerationStructureKHR(const AccelerationStructureKHR& accelerationStructure) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkBuildAccelerationStructuresKHR.html">vkBuildAccelerationStructuresKHR</a>
   */
  vk::Result buildAccelerationStructuresKHR(vk::DeferredOperationKHR deferredOperation, 
                                            const vk::ArrayProxy<const vk::AccelerationStructureBuildGeometryInfoKHR> &infos,
                                            const vk::ArrayProxy<const vk::AccelerationStructureBuildRangeInfoKHR *const> &pBuildRangeInfos) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetDeviceAccelerationStructureCompatibilityKHR.html">vkGetDeviceAccelerationStructureCompatibilityKHR</a>
   */
  vk::AccelerationStructureCompatibilityKHR 
      getAccelerationStructureCompatibilityKHR(const vk::AccelerationStructureVersionInfoKHR &versionInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkWriteAccelerationStructuresPropertiesKHR.html">vkWriteAccelerationStructuresPropertiesKHR</a>
   */
  template <typename T>
  vk::ResultValueType<void>::type writeAccelerationStructuresPropertiesKHR(const vk::ArrayProxy<const vk::AccelerationStructureKHR> &accelerationStructures, 
                                                                           vk::QueryType queryType, const vk::ArrayProxy<T> &data, size_t stride) const;                       

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateValidationCacheEXT.html">vkCreateValidationCacheEXT</a>
   */
  ValidationCacheEXT createValidationCacheEXT(const vk::ValidationCacheCreateInfoEXT& createInfo,
                                              const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyValidationCacheEXT.html">vkDestroyValidationCacheEXT</a>
   */
  void destroyValidationCacheEXT(const ValidationCacheEXT& validationCacheExt) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateAccelerationStructureNV.html">vkCreateAccelerationStructureNV</a>
   */
  AccelerationStructureNV
    createAccelerationStructureNV(const vk::AccelerationStructureCreateInfoNV& createInfo,
                                  const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyAccelerationStructureNV.html">vkDestroyAccelerationStructureNV</a>
   */
  void destroyAccelerationStructureNV(const AccelerationStructureNV& accelerationStructure) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateIndirectCommandsLayoutNV.html">vkCreateIndirectCommandsLayoutNV</a>
   */
  IndirectCommandsLayoutNV createIndirectCommandsLayoutNV(
    const vk::IndirectCommandsLayoutCreateInfoNV& createInfo,
    const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDestroyIndirectCommandsLayoutNV.html">vkDestroyIndirectCommandsLayoutNV</a>
   */
  void destroyIndirectCommandsLayoutNV(const IndirectCommandsLayoutNV& indirectCommandsLayout) const;


  // Deprecated
  // ObjectTableNVX createObjectTableNVX(const vk::ObjectTableCreateInfoNV& createInfo,
  //                                     const std::optional<vk::AllocationCallbacks>& allocator = {}) const;

  // void destroyObjectTableNVX(const ObjectTableNVX& objectTable) const;


  /**
   * @brief Enumerate queue families that are created by this logical device
   */
  std::vector<QueueFamily> enumerateQueueFamilies() const;

  // endregion

  // region Vulkan Declarations

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkUpdateDescriptorSets.html">vkUpdateDescriptorSets</a>
   */  
  void updateDescriptorSets(const vk::ArrayProxy<const vk::WriteDescriptorSet>& descriptorWrites,
                            const vk::ArrayProxy<const vk::CopyDescriptorSet>& descriptorCopies = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkSetDebugUtilsObjectNameEXT.html">vkSetDebugUtilsObjectNameEXT</a>
   */
  vk::ResultValueType<void>::type setDebugUtilsObjectNameEXT(const vk::DebugUtilsObjectNameInfoEXT& nameInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkSetDebugUtilsObjectTagEXT.html">vkSetDebugUtilsObjectTagEXT</a>
   */
  vk::ResultValueType<void>::type setDebugUtilsObjectTagEXT(const vk::DebugUtilsObjectTagInfoEXT& tagInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDebugMarkerSetObjectNameEXT.html">vkDebugMarkerSetObjectNameEXT</a>
   */
  vk::ResultValueType<void>::type debugMarkerSetObjectNameEXT(const vk::DebugMarkerObjectNameInfoEXT& nameInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDebugMarkerSetObjectTagEXT.html">vkDebugMarkerSetObjectTagEXT</a>
   */
  vk::ResultValueType<void>::type debugMarkerSetObjectTagEXT(const vk::DebugMarkerObjectTagInfoEXT& tagInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDeviceWaitIdle.html">vkDeviceWaitIdle</a>
   */
  vk::ResultValueType<void>::type waitIdle() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkDisplayPowerControlEXT.html">vkDisplayPowerControlEXT</a>
   */
  vk::ResultValueType<void>::type displayPowerControlEXT(const vk::DisplayKHR& display,
                                                         const vk::DisplayPowerInfoEXT& powerInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetImageViewHandleNVX.html">vkGetImageViewHandleNVX</a>
   */
  uint32_t getImageViewHandleNVX(const vk::ImageViewHandleInfoNVX& handleInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkFlushMappedMemoryRanges.html">vkFlushMappedMemoryRanges</a>
   */
  vk::ResultValueType<void>::type
    flushMappedMemoryRanges(const vk::ArrayProxy<const vk::MappedMemoryRange>& memoryRanges = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkInvalidateMappedMemoryRanges.html">vkInvalidateMappedMemoryRanges</a>
   */
  vk::ResultValueType<void>::type
    invalidateMappedMemoryRanges(const vk::ArrayProxy<const vk::MappedMemoryRange>& memoryRanges = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetCalibratedTimestampsEXT.html">vkGetCalibratedTimestampsEXT</a>
   */
  vk::ResultValueType<uint64_t>::type
    getCalibratedTimestampsEXT(const vk::ArrayProxy<const vk::CalibratedTimestampInfoEXT>& timestampInfos,
                               const vk::ArrayProxy<uint64_t>& timestamps) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetDescriptorSetLayoutSupport.html">vkGetDescriptorSetLayoutSupport</a>
   */
  vk::DescriptorSetLayoutSupport
    getDescriptorSetLayoutSupport(const vk::DescriptorSetLayoutCreateInfo& createInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetDescriptorSetLayoutSupportKHR.html">vkGetDescriptorSetLayoutSupportKHR</a>
   */
  vk::DescriptorSetLayoutSupportKHR
    getDescriptorSetLayoutSupportKHR(const vk::DescriptorSetLayoutCreateInfo& createInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetDeviceGroupPeerMemoryFeatures.html">vkGetDeviceGroupPeerMemoryFeatures</a>
   */
  vk::PeerMemoryFeatureFlags getGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex,
                                                        uint32_t remoteDeviceIndex) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetDeviceGroupPeerMemoryFeaturesKHR.html">vkGetDeviceGroupPeerMemoryFeaturesKHR</a>
   */
  vk::PeerMemoryFeatureFlagsKHR getGroupPeerMemoryFeaturesKHR(uint32_t heapIndex, uint32_t localDeviceIndex,
                                                              uint32_t remoteDeviceIndex) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetDeviceGroupPresentCapabilitiesKHR.html">vkGetDeviceGroupPresentCapabilitiesKHR</a>
   */
  vk::ResultValueType<vk::DeviceGroupPresentCapabilitiesKHR>::type getGroupPresentCapabilitiesKHR() const;

#ifdef VK_USE_PLATFORM_WIN32_KHR
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetDeviceGroupSurfacePresentModes2EXT.html">vkGetDeviceGroupSurfacePresentModes2EXT</a>
   */
  vk::ResultValueType<vk::DeviceGroupPresentModeFlagsKHR>::type
    getGroupSurfacePresentModes2EXT(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetDeviceGroupSurfacePresentModes2KHR.html">vkGetDeviceGroupSurfacePresentModes2KHR</a>
   */
  vk::ResultValueType<vk::DeviceGroupPresentModeFlagsKHR>::type
    getGroupSurfacePresentModes2KHR(const vk::PhysicalDeviceSurfaceInfo2KHR& surfaceInfo) const;
#endif

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetDeviceProcAddr.html">vkGetDeviceProcAddr</a>
   */
  PFN_vkVoidFunction getProcAddr(const std::string& name) const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::Device&() const;
};

template <typename T>
vk::ResultValueType<void>::type
    LogicalDevice::writeAccelerationStructuresPropertiesKHR(const vk::ArrayProxy<const vk::AccelerationStructureKHR> &accelerationStructures,
                                                            vk::QueryType queryType, const vk::ArrayProxy<T> &data, size_t stride) const {
  return object_->writeAccelerationStructuresPropertiesKHR(accelerationStructures, queryType, data, stride);
}

} // namespace logi

#endif // LOGI_DEVICE_LOGICAL_DEVICE_HPP
