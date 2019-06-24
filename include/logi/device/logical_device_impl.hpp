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

#ifndef LOGI_DEVICE_LOGICAL_DEVICE_IMPL_HPP
#define LOGI_DEVICE_LOGICAL_DEVICE_IMPL_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class QueueFamilyImpl;
class SwapchainKHRImpl;
class ShaderModuleImpl;
class PipelineCacheImpl;
class EventImpl;
class FenceImpl;
class SemaphoreImpl;
class QueryPoolImpl;
class CommandPoolImpl;
class DescriptorSetLayoutImpl;
class DescriptorPoolImpl;
class PipelineLayoutImpl;
class MemoryAllocatorImpl;
class SamplerImpl;
class PipelineImpl;
class RenderPassImpl;
class FramebufferImpl;
class ValidationCacheEXTImpl;
class AccelerationStructureNVImpl;
class IndirectCommandsLayoutNVXImpl;
class ObjectTableNVXImpl;
class DescriptorUpdateTemplateImpl;
class BufferImpl;
class ImageImpl;

class LogicalDeviceImpl : public VulkanObject<LogicalDeviceImpl>,
                          public VulkanObjectComposite<QueueFamilyImpl>,
                          public VulkanObjectComposite<MemoryAllocatorImpl>,
                          public VulkanObjectComposite<BufferImpl>,
                          public VulkanObjectComposite<ImageImpl>,
                          public VulkanObjectComposite<SwapchainKHRImpl>,
                          public VulkanObjectComposite<SamplerImpl>,
                          public VulkanObjectComposite<CommandPoolImpl>,
                          public VulkanObjectComposite<QueryPoolImpl>,
                          public VulkanObjectComposite<EventImpl>,
                          public VulkanObjectComposite<FenceImpl>,
                          public VulkanObjectComposite<SemaphoreImpl>,
                          public VulkanObjectComposite<ShaderModuleImpl>,
                          public VulkanObjectComposite<PipelineCacheImpl>,
                          public VulkanObjectComposite<DescriptorSetLayoutImpl>,
                          public VulkanObjectComposite<DescriptorPoolImpl>,
                          public VulkanObjectComposite<DescriptorUpdateTemplateImpl>,
                          public VulkanObjectComposite<PipelineLayoutImpl>,
                          public VulkanObjectComposite<PipelineImpl>,
                          public VulkanObjectComposite<RenderPassImpl>,
                          public VulkanObjectComposite<FramebufferImpl>,
                          public VulkanObjectComposite<ValidationCacheEXTImpl>,
                          public VulkanObjectComposite<AccelerationStructureNVImpl>,
                          public VulkanObjectComposite<IndirectCommandsLayoutNVXImpl>,
                          public VulkanObjectComposite<ObjectTableNVXImpl> {
 public:
  LogicalDeviceImpl(PhysicalDeviceImpl& physicalDevice, const vk::DeviceCreateInfo& createInfo,
                    const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Sub-Handles

  const std::shared_ptr<MemoryAllocatorImpl>&
    createMemoryAllocator(vk::DeviceSize preferredLargeHeapBlockSize = 0u, uint32_t frameInUseCount = 0u,
                          const std::vector<vk::DeviceSize>& heapSizeLimits = {},
                          const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyMemoryAllocator(size_t id);

  const std::shared_ptr<BufferImpl>& createBuffer(const vk::BufferCreateInfo& createInfo,
                                                  const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyBuffer(size_t id);

  const std::shared_ptr<ImageImpl>& createImage(const vk::ImageCreateInfo& createInfo,
                                                const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyImage(size_t id);

  const std::shared_ptr<SamplerImpl>& createSampler(const vk::SamplerCreateInfo& createInfo,
                                                    const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroySampler(size_t id);

  const std::shared_ptr<ShaderModuleImpl>&
    createShaderModule(const vk::ShaderModuleCreateInfo& createInfo,
                       const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyShaderModule(size_t id);

  const std::shared_ptr<PipelineCacheImpl>&
    createPipelineCache(const vk::PipelineCacheCreateInfo& createInfo,
                        const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyPipelineCache(size_t id);

  const std::shared_ptr<DescriptorSetLayoutImpl>&
    createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& createInfo,
                              const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyDescriptorSetLayout(size_t id);

  const std::shared_ptr<DescriptorPoolImpl>&
    createDescriptorPool(const vk::DescriptorPoolCreateInfo& createInfo,
                         const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyDescriptorPool(size_t id);

  const std::shared_ptr<PipelineLayoutImpl>&
    createPipelineLayout(const vk::PipelineLayoutCreateInfo& createInfo,
                         const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyPipelineLayout(size_t id);

  const std::shared_ptr<DescriptorUpdateTemplateImpl>&
    createDescriptorUpdateTemplate(const vk::DescriptorUpdateTemplateCreateInfo& createInfo,
                                   const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyDescriptorUpdateTemplate(size_t id);

  std::vector<std::shared_ptr<PipelineImpl>>
    createComputePipelines(const vk::ArrayProxy<const vk::ComputePipelineCreateInfo>& create_infos,
                           const vk::PipelineCache& cache = nullptr,
                           const std::optional<vk::AllocationCallbacks>& allocator = {});

  std::shared_ptr<PipelineImpl> createComputePipeline(const vk::ComputePipelineCreateInfo& create_infos,
                                                      const vk::PipelineCache& cache = nullptr,
                                                      const std::optional<vk::AllocationCallbacks>& allocator = {});

  std::vector<std::shared_ptr<PipelineImpl>>
    createGraphicsPipelines(const vk::ArrayProxy<const vk::GraphicsPipelineCreateInfo>& create_infos,
                            const vk::PipelineCache& cache = nullptr,
                            const std::optional<vk::AllocationCallbacks>& allocator = {});

  std::shared_ptr<PipelineImpl> createGraphicsPipeline(const vk::GraphicsPipelineCreateInfo& create_info,
                                                       const vk::PipelineCache& cache = nullptr,
                                                       const std::optional<vk::AllocationCallbacks>& allocator = {});

  std::vector<std::shared_ptr<PipelineImpl>>
    createRayTracingPipelinesNV(const vk::ArrayProxy<const vk::RayTracingPipelineCreateInfoNV>& create_infos,
                                const vk::PipelineCache& cache = nullptr,
                                const std::optional<vk::AllocationCallbacks>& allocator = {});

  std::shared_ptr<PipelineImpl>
    createRayTracingPipelinesNV(const vk::RayTracingPipelineCreateInfoNV& create_info,
                                const vk::PipelineCache& cache = nullptr,
                                const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyPipeline(size_t id);

  const std::shared_ptr<CommandPoolImpl>&
    createCommandPool(const vk::CommandPoolCreateInfo& createInfo,
                      const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyCommandPool(size_t id);

  const std::shared_ptr<QueryPoolImpl>& createQueryPool(const vk::QueryPoolCreateInfo& createInfo,
                                                        const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyQueryPool(size_t id);

  const std::shared_ptr<EventImpl>& createEvent(const vk::EventCreateInfo& createInfo,
                                                const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyEvent(size_t id);

  const std::shared_ptr<FenceImpl>& createFence(const vk::FenceCreateInfo& createInfo,
                                                const std::optional<vk::AllocationCallbacks>& allocator = {});

  const std::shared_ptr<FenceImpl>& registerEventEXT(const vk::DeviceEventInfoEXT& eventInfo,
                                                     const std::optional<vk::AllocationCallbacks>& allocator = {});

  const std::shared_ptr<FenceImpl>&
    registerDisplayEventEXT(const vk::DisplayKHR& display, const vk::DisplayEventInfoEXT& eventInfo,
                            const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyFence(size_t id);

  const std::shared_ptr<SemaphoreImpl>& createSemaphore(const vk::SemaphoreCreateInfo& createInfo,
                                                        const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroySemaphore(size_t id);

  const std::shared_ptr<RenderPassImpl>& createRenderPass(const vk::RenderPassCreateInfo& createInfo,
                                                          const std::optional<vk::AllocationCallbacks>& allocator = {});

  const std::shared_ptr<RenderPassImpl>& createRenderPass(const vk::RenderPassCreateInfo2KHR& createInfo,
                                                          const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyRenderPass(size_t id);

  const std::shared_ptr<FramebufferImpl>&
    createFramebuffer(const vk::FramebufferCreateInfo& createInfo,
                      const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyFramebuffer(size_t id);

  const std::shared_ptr<SwapchainKHRImpl>&
    createSwapchainKHR(const vk::SwapchainCreateInfoKHR& createInfo,
                       const std::optional<vk::AllocationCallbacks>& allocator = {});

  std::vector<std::shared_ptr<SwapchainKHRImpl>>
    createSharedSwapchainsKHR(const vk::ArrayProxy<const vk::SwapchainCreateInfoKHR>& createInfos,
                              const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroySwapchainKHR(size_t id);

  const std::shared_ptr<ValidationCacheEXTImpl>&
    createValidationCacheEXT(const vk::ValidationCacheCreateInfoEXT& createInfo,
                             const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyValidationCacheEXT(size_t id);

  const std::shared_ptr<AccelerationStructureNVImpl>&
    createAccelerationStructureNV(const vk::AccelerationStructureCreateInfoNV& create_info,
                                  const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyAccelerationStructureNV(size_t id);

  const std::shared_ptr<IndirectCommandsLayoutNVXImpl>& createIndirectCommandsLayoutNVXIndirectCommandsLayoutNVX(
    const vk::IndirectCommandsLayoutCreateInfoNVX& create_info,
    const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyIndirectCommandsLayoutNVX(size_t id);

  const std::shared_ptr<ObjectTableNVXImpl>&
    createObjectTableNVX(const vk::ObjectTableCreateInfoNVX& create_info,
                         const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyObjectTableNVX(size_t id);

  std::vector<std::shared_ptr<QueueFamilyImpl>> enumerateQueueFamilies() const;

  // endregion

  // region Vulkan Declarations

  void updateDescriptorSets(const vk::ArrayProxy<const vk::WriteDescriptorSet>& descriptorWrites,
                            const vk::ArrayProxy<const vk::CopyDescriptorSet>& descriptorCopies) const;

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

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::Device() const;

 protected:
  void free() override;

  // endregion

 private:
  PhysicalDeviceImpl& physicalDevice_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::Device vkDevice_;
  vk::DispatchLoaderDynamic dispatcher_;
};

} // namespace logi

#endif // LOGI_DEVICE_LOGICAL_DEVICE_IMPL_HPP
