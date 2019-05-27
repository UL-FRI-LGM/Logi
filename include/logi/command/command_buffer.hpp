/**
 * Project Logi source code
 * Copyright (C) 2019 Lana Besevic
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

#ifndef LOGI_COMMAND_COMMAND_BUFFER_HPP
#define LOGI_COMMAND_COMMAND_BUFFER_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/handle.hpp"
#include "logi/command/command_buffer_impl.hpp"

namespace logi {

class CommandPoolImpl;
class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;
class CommandPool;

class CommandBuffer : public Handle<CommandBufferImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  vk::ResultValueType<void>::type begin(const vk::CommandBufferBeginInfo& beginInfo) const;

  void beginQuery(vk::QueryPool queryPool, uint32_t query, const vk::QueryControlFlags& flags) const;

  void beginRenderPass(const vk::RenderPassBeginInfo& renderPassBegin, vk::SubpassContents contents) const;

  void bindDescriptorSets(vk::PipelineBindPoint pipelineBindPoint, vk::PipelineLayout layout, uint32_t firstSet,
                          vk::ArrayProxy<const vk::DescriptorSet> descriptorSets,
                          vk::ArrayProxy<const uint32_t> dynamicOffsets) const;

  void bindIndexBuffer(vk::Buffer buffer, vk::DeviceSize offset, vk::IndexType indexType) const;

  void bindPipeline(vk::PipelineBindPoint pipelineBindPoint, vk::Pipeline pipeline) const;

  void bindVertexBuffers(uint32_t firstBinding, vk::ArrayProxy<const vk::Buffer> buffers,
                         vk::ArrayProxy<const vk::DeviceSize> offsets) const;

  void blitImage(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Image dstImage, vk::ImageLayout dstImageLayout,
                 vk::ArrayProxy<const vk::ImageBlit> regions, vk::Filter filter) const;

  void clearAttachments(vk::ArrayProxy<const vk::ClearAttachment> attachments,
                        vk::ArrayProxy<const vk::ClearRect> rects) const;

  void clearColorImage(vk::Image image, vk::ImageLayout imageLayout, const vk::ClearColorValue& color,
                       vk::ArrayProxy<const vk::ImageSubresourceRange> ranges) const;

  void clearDepthStencilImage(vk::Image image, vk::ImageLayout imageLayout,
                              const vk::ClearDepthStencilValue& depthStencil,
                              vk::ArrayProxy<const vk::ImageSubresourceRange> ranges) const;

  void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::ArrayProxy<const vk::BufferCopy> regions) const;

  void copyBufferToImage(vk::Buffer srcBuffer, vk::Image dstImage, vk::ImageLayout dstImageLayout,
                         vk::ArrayProxy<const vk::BufferImageCopy> regions) const;

  void copyImage(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Image dstImage, vk::ImageLayout dstImageLayout,
                 vk::ArrayProxy<const vk::ImageCopy> regions) const;

  void copyImageToBuffer(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Buffer dstBuffer,
                         vk::ArrayProxy<const vk::BufferImageCopy> regions) const;

  void copyQueryPoolResults(vk::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, vk::Buffer dstBuffer,
                            vk::DeviceSize dstOffset, vk::DeviceSize stride, const vk::QueryResultFlags& flags) const;

  void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const;

  void dispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX,
                    uint32_t groupCountY, uint32_t groupCountZ) const;

  void dispatchIndirect(vk::Buffer buffer, vk::DeviceSize offset) const;

  void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const;

  void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
                   uint32_t firstInstance) const;

  void drawIndexedIndirect(vk::Buffer buffer, vk::DeviceSize offset, uint32_t drawCount, uint32_t stride) const;

  void drawIndirect(vk::Buffer buffer, vk::DeviceSize offset, uint32_t drawCount, uint32_t stride) const;

  void endQuery(vk::QueryPool queryPool, uint32_t query) const;

  void endRenderPass() const;

  void executeCommands(vk::ArrayProxy<const vk::CommandBuffer> commandBuffers) const;

  void fillBuffer(vk::Buffer dstBuffer, vk::DeviceSize dstOffset, vk::DeviceSize size, uint32_t data) const;

  void nextSubpass(vk::SubpassContents contents) const;

  void pipelineBarrier(const vk::PipelineStageFlags& srcStageMask, vk::PipelineStageFlags dstStageMask,
                       const vk::DependencyFlags& dependencyFlags,
                       vk::ArrayProxy<const vk::MemoryBarrier> memoryBarriers,
                       vk::ArrayProxy<const vk::BufferMemoryBarrier> bufferMemoryBarriers,
                       vk::ArrayProxy<const vk::ImageMemoryBarrier> imageMemoryBarriers) const;

  template <typename T>
  void pushConstants(vk::PipelineLayout layout, vk::ShaderStageFlags stageFlags, uint32_t offset,
                     vk::ArrayProxy<const T> values) const {
    object_->pushConstants(layout, stageFlags, offset, values, getDispatcher());
  }

  void resetEvent(vk::Event event, const vk::PipelineStageFlags& stageMask) const;

  void resetQueryPool(vk::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const;

  void resolveImage(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Image dstImage,
                    vk::ImageLayout dstImageLayout, vk::ArrayProxy<const vk::ImageResolve> regions) const;

  void setBlendConstants(const float blendConstants[4]) const;

  void setDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const;

  void setDepthBounds(float minDepthBounds, float maxDepthBounds) const;

  void setDeviceMask(uint32_t deviceMask) const;

  void setEvent(vk::Event event, vk::PipelineStageFlags stageMask) const;

  void setLineWidth(float lineWidth) const;

  void setScissor(uint32_t firstScissor, vk::ArrayProxy<const vk::Rect2D> scissors) const;

  void setStencilCompareMask(vk::StencilFaceFlags faceMask, uint32_t compareMask) const;

  void setStencilReference(vk::StencilFaceFlags faceMask, uint32_t reference) const;

  void setStencilWriteMask(vk::StencilFaceFlags faceMask, uint32_t writeMask) const;

  void setViewport(uint32_t firstViewport, vk::ArrayProxy<const vk::Viewport> viewports) const;

  template <typename T>
  void updateBuffer(vk::Buffer dstBuffer, vk::DeviceSize dstOffset, vk::ArrayProxy<const T> data) const {
    object_->updateBuffer(dstBuffer, dstOffset, data, getDispatcher());
  }

  void waitEvents(vk::ArrayProxy<const vk::Event> events, const vk::PipelineStageFlags& srcStageMask,
                  const vk::PipelineStageFlags& dstStageMask, vk::ArrayProxy<const vk::MemoryBarrier> memoryBarriers,
                  vk::ArrayProxy<const vk::BufferMemoryBarrier> bufferMemoryBarriers,
                  vk::ArrayProxy<const vk::ImageMemoryBarrier> imageMemoryBarriers) const;

  void writeTimestamp(vk::PipelineStageFlagBits pipelineStage, vk::QueryPool queryPool, uint32_t query) const;

  vk::ResultValueType<void>::type end() const;

  void beginRenderPass2KHR(const vk::RenderPassBeginInfo& renderPassBegin,
                           const vk::SubpassBeginInfoKHR& subpassBeginInfo) const;

  void dispatchBaseKHR(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX,
                       uint32_t groupCountY, uint32_t groupCountZ) const;

  void drawIndexedIndirectCountKHR(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                   vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;

  void drawIndirectCountKHR(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                            vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;

  void endRenderPass2KHR(const vk::SubpassEndInfoKHR& subpassEndInfo) const;

  void nextSubpass2KHR(const vk::SubpassBeginInfoKHR& subpassBeginInfo,
                       const vk::SubpassEndInfoKHR& subpassEndInfo) const;

  void pushDescriptorSetKHR(vk::PipelineBindPoint pipelineBindPoint, vk::PipelineLayout layout, uint32_t set,
                            vk::ArrayProxy<const vk::WriteDescriptorSet> descriptorWrites) const;

  void pushDescriptorSetWithTemplateKHR(vk::DescriptorUpdateTemplate descriptorUpdateTemplate,
                                        vk::PipelineLayout layout, uint32_t set, const void* pData) const;

  void setDeviceMaskKHR(uint32_t deviceMask) const;

  void beginConditionalRenderingEXT(const vk::ConditionalRenderingBeginInfoEXT& conditionalRenderingBegin) const;

  void beginDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& labelInfo) const;

  void beginQueryIndexedEXT(vk::QueryPool queryPool, uint32_t query, const vk::QueryControlFlags& flags,
                            uint32_t index) const;

  void beginTransformFeedbackEXT(uint32_t firstCounterBuffer, vk::ArrayProxy<const vk::Buffer> counterBuffers,
                                 vk::ArrayProxy<const vk::DeviceSize> counterBufferOffsets) const;

  void bindTransformFeedbackBuffersEXT(uint32_t firstBinding, vk::ArrayProxy<const vk::Buffer> buffers,
                                       vk::ArrayProxy<const vk::DeviceSize> offsets,
                                       vk::ArrayProxy<const vk::DeviceSize> sizes) const;

  void debugMarkerBeginEXT(const vk::DebugMarkerMarkerInfoEXT& markerInfo) const;

  void debugMarkerEndEXT() const;

  void debugMarkerInsertEXT(const vk::DebugMarkerMarkerInfoEXT& markerInfo) const;

  void drawIndirectByteCountEXT(uint32_t instanceCount, uint32_t firstInstance, vk::Buffer counterBuffer,
                                vk::DeviceSize counterBufferOffset, uint32_t counterOffset,
                                uint32_t vertexStride) const;

  void endConditionalRenderingEXT() const;

  void endDebugUtilsLabelEXT() const;

  void endQueryIndexedEXT(vk::QueryPool queryPool, uint32_t query, uint32_t index) const;

  void endTransformFeedbackEXT(uint32_t firstCounterBuffer, vk::ArrayProxy<const vk::Buffer> counterBuffers,
                               vk::ArrayProxy<const vk::DeviceSize> counterBufferOffsets) const;

  void insertDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& labelInfo) const;

  void setDiscardRectangleEXT(uint32_t firstDiscardRectangle, vk::ArrayProxy<const vk::Rect2D> discardRectangles) const;

  void setSampleLocationsEXT(const vk::SampleLocationsInfoEXT& sampleLocationsInfo) const;

  void bindShadingRateImageNV(vk::ImageView imageView, vk::ImageLayout imageLayout) const;

  void buildAccelerationStructureNV(const vk::AccelerationStructureInfoNV& info, vk::Buffer instanceData,
                                    vk::DeviceSize instanceOffset, vk::Bool32 update, vk::AccelerationStructureNV dst,
                                    vk::AccelerationStructureNV src, vk::Buffer scratch,
                                    vk::DeviceSize scratchOffset) const;

  void copyAccelerationStructureNV(vk::AccelerationStructureNV dst, vk::AccelerationStructureNV src,
                                   vk::CopyAccelerationStructureModeNV mode) const;

  void drawMeshTasksIndirectCountNV(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                    vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;

  void drawMeshTasksIndirectNV(vk::Buffer buffer, vk::DeviceSize offset, uint32_t drawCount, uint32_t stride) const;

  void drawMeshTasksNV(uint32_t taskCount, uint32_t firstTask) const;

  void setCheckpointNV(const void* pCheckpointMarker) const;

  void setCoarseSampleOrderNV(vk::CoarseSampleOrderTypeNV sampleOrderType,
                              vk::ArrayProxy<const vk::CoarseSampleOrderCustomNV> customSampleOrders) const;

  void setExclusiveScissorNV(uint32_t firstExclusiveScissor, vk::ArrayProxy<const vk::Rect2D> exclusiveScissors) const;

  void setViewportShadingRatePaletteNV(uint32_t firstViewport,
                                       vk::ArrayProxy<const vk::ShadingRatePaletteNV> shadingRatePalettes) const;

  void setViewportWScalingNV(uint32_t firstViewport,
                             vk::ArrayProxy<const vk::ViewportWScalingNV> viewportWScalings) const;

  void traceRaysNV(vk::Buffer raygenShaderBindingTableBuffer, vk::DeviceSize raygenShaderBindingOffset,
                   vk::Buffer missShaderBindingTableBuffer, vk::DeviceSize missShaderBindingOffset,
                   vk::DeviceSize missShaderBindingStride, vk::Buffer hitShaderBindingTableBuffer,
                   vk::DeviceSize hitShaderBindingOffset, vk::DeviceSize hitShaderBindingStride,
                   vk::Buffer callableShaderBindingTableBuffer, vk::DeviceSize callableShaderBindingOffset,
                   vk::DeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) const;

  void writeAccelerationStructuresPropertiesNV(vk::ArrayProxy<const vk::AccelerationStructureNV> accelerationStructures,
                                               vk::QueryType queryType, vk::QueryPool queryPool,
                                               uint32_t firstQuery) const;

  void processCommandsNVX(const vk::CmdProcessCommandsInfoNVX& processCommandsInfo) const;

  void reserveSpaceForCommandsNVX(const vk::CmdReserveSpaceForCommandsInfoNVX& reserveSpaceInfo) const;

  void drawIndexedIndirectCountAMD(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                   vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;

  void drawIndirectCountAMD(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                            vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;

  void writeBufferMarkerAMD(vk::PipelineStageFlagBits pipelineStage, vk::Buffer dstBuffer, vk::DeviceSize dstOffset,
                            uint32_t marker) const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  CommandPool getCommandPool() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::CommandBuffer() const;

  // endregion
};

} // namespace logi

#endif // LOGI_COMMAND_COMMAND_BUFFER_HPP
