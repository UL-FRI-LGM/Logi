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

#include "logi/command/command_buffer_impl.hpp"
#include "logi/command/command_pool_impl.hpp"

namespace logi {

CommandBufferImpl::CommandBufferImpl(CommandPoolImpl& commandPool, const vk::CommandBuffer& vkCommandBuffer)
  : commandPool_(commandPool), vkCommandBuffer_(vkCommandBuffer) {}

// region Vulkan Definitions

vk::ResultValueType<void>::type CommandBufferImpl::begin(const vk::CommandBufferBeginInfo& beginInfo) const {
  return vkCommandBuffer_.begin(beginInfo, getDispatcher());
}

void CommandBufferImpl::beginQuery(vk::QueryPool queryPool, uint32_t query, const vk::QueryControlFlags& flags) const {
  vkCommandBuffer_.beginQuery(queryPool, query, flags, getDispatcher());
}

void CommandBufferImpl::beginRenderPass(const vk::RenderPassBeginInfo& renderPassBegin,
                                        vk::SubpassContents contents) const {
  vkCommandBuffer_.beginRenderPass(renderPassBegin, contents, getDispatcher());
}

void CommandBufferImpl::bindDescriptorSets(vk::PipelineBindPoint pipelineBindPoint, vk::PipelineLayout layout,
                                           uint32_t firstSet, vk::ArrayProxy<const vk::DescriptorSet> descriptorSets,
                                           vk::ArrayProxy<const uint32_t> dynamicOffsets) const {
  vkCommandBuffer_.bindDescriptorSets(pipelineBindPoint, layout, firstSet, descriptorSets, dynamicOffsets,
                                      getDispatcher());
}

void CommandBufferImpl::bindIndexBuffer(vk::Buffer buffer, vk::DeviceSize offset, vk::IndexType indexType) const {
  vkCommandBuffer_.bindIndexBuffer(buffer, offset, indexType, getDispatcher());
}

void CommandBufferImpl::bindPipeline(vk::PipelineBindPoint pipelineBindPoint, vk::Pipeline pipeline) const {
  vkCommandBuffer_.bindPipeline(pipelineBindPoint, pipeline, getDispatcher());
}

void CommandBufferImpl::bindVertexBuffers(uint32_t firstBinding, vk::ArrayProxy<const vk::Buffer> buffers,
                                          vk::ArrayProxy<const vk::DeviceSize> offsets) const {
  vkCommandBuffer_.bindVertexBuffers(firstBinding, buffers, offsets, getDispatcher());
}

void CommandBufferImpl::blitImage(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Image dstImage,
                                  vk::ImageLayout dstImageLayout, vk::ArrayProxy<const vk::ImageBlit> regions,
                                  vk::Filter filter) const {
  vkCommandBuffer_.blitImage(srcImage, srcImageLayout, dstImage, dstImageLayout, regions, filter, getDispatcher());
}

void CommandBufferImpl::clearAttachments(vk::ArrayProxy<const vk::ClearAttachment> attachments,
                                         vk::ArrayProxy<const vk::ClearRect> rects) const {
  vkCommandBuffer_.clearAttachments(attachments, rects, getDispatcher());
}

void CommandBufferImpl::clearColorImage(vk::Image image, vk::ImageLayout imageLayout, const vk::ClearColorValue& color,
                                        vk::ArrayProxy<const vk::ImageSubresourceRange> ranges) const {
  vkCommandBuffer_.clearColorImage(image, imageLayout, color, ranges, getDispatcher());
}

void CommandBufferImpl::clearDepthStencilImage(vk::Image image, vk::ImageLayout imageLayout,
                                               const vk::ClearDepthStencilValue& depthStencil,
                                               vk::ArrayProxy<const vk::ImageSubresourceRange> ranges) const {
  vkCommandBuffer_.clearDepthStencilImage(image, imageLayout, depthStencil, ranges, getDispatcher());
}

void CommandBufferImpl::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer,
                                   vk::ArrayProxy<const vk::BufferCopy> regions) const {
  vkCommandBuffer_.copyBuffer(srcBuffer, dstBuffer, regions, getDispatcher());
}

void CommandBufferImpl::copyBufferToImage(vk::Buffer srcBuffer, vk::Image dstImage, vk::ImageLayout dstImageLayout,
                                          vk::ArrayProxy<const vk::BufferImageCopy> regions) const {
  vkCommandBuffer_.copyBufferToImage(srcBuffer, dstImage, dstImageLayout, regions, getDispatcher());
}

void CommandBufferImpl::copyImage(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Image dstImage,
                                  vk::ImageLayout dstImageLayout, vk::ArrayProxy<const vk::ImageCopy> regions) const {
  vkCommandBuffer_.copyImage(srcImage, srcImageLayout, dstImage, dstImageLayout, regions, getDispatcher());
}

void CommandBufferImpl::copyImageToBuffer(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Buffer dstBuffer,
                                          vk::ArrayProxy<const vk::BufferImageCopy> regions) const {
  vkCommandBuffer_.copyImageToBuffer(srcImage, srcImageLayout, dstBuffer, regions, getDispatcher());
}

void CommandBufferImpl::copyQueryPoolResults(vk::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount,
                                             vk::Buffer dstBuffer, vk::DeviceSize dstOffset, vk::DeviceSize stride,
                                             const vk::QueryResultFlags& flags) const {
  vkCommandBuffer_.copyQueryPoolResults(queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags,
                                        getDispatcher());
}

void CommandBufferImpl::dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const {
  vkCommandBuffer_.dispatch(groupCountX, groupCountY, groupCountZ, getDispatcher());
}

void CommandBufferImpl::dispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ,
                                     uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const {
  vkCommandBuffer_.dispatchBase(baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ,
                                getDispatcher());
}

void CommandBufferImpl::dispatchIndirect(vk::Buffer buffer, vk::DeviceSize offset) const {
  vkCommandBuffer_.dispatchIndirect(buffer, offset, getDispatcher());
}

void CommandBufferImpl::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex,
                             uint32_t firstInstance) const {
  vkCommandBuffer_.draw(vertexCount, instanceCount, firstVertex, firstInstance, getDispatcher());
}

void CommandBufferImpl::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                                    int32_t vertexOffset, uint32_t firstInstance) const {
  vkCommandBuffer_.drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance, getDispatcher());
}

void CommandBufferImpl::drawIndexedIndirect(vk::Buffer buffer, vk::DeviceSize offset, uint32_t drawCount,
                                            uint32_t stride) const {
  vkCommandBuffer_.drawIndexedIndirect(buffer, offset, drawCount, stride, getDispatcher());
}

void CommandBufferImpl::drawIndirect(vk::Buffer buffer, vk::DeviceSize offset, uint32_t drawCount,
                                     uint32_t stride) const {
  vkCommandBuffer_.drawIndirect(buffer, offset, drawCount, stride, getDispatcher());
}

void CommandBufferImpl::endQuery(vk::QueryPool queryPool, uint32_t query) const {
  vkCommandBuffer_.endQuery(queryPool, query, getDispatcher());
}

void CommandBufferImpl::endRenderPass() const {
  vkCommandBuffer_.endRenderPass(getDispatcher());
}

void CommandBufferImpl::executeCommands(vk::ArrayProxy<const vk::CommandBuffer> commandBuffers) const {
  vkCommandBuffer_.executeCommands(commandBuffers, getDispatcher());
}

void CommandBufferImpl::fillBuffer(vk::Buffer dstBuffer, vk::DeviceSize dstOffset, vk::DeviceSize size,
                                   uint32_t data) const {
  vkCommandBuffer_.fillBuffer(dstBuffer, dstOffset, size, data, getDispatcher());
}

void CommandBufferImpl::nextSubpass(vk::SubpassContents contents) const {
  vkCommandBuffer_.nextSubpass(contents, getDispatcher());
}

void CommandBufferImpl::pipelineBarrier(const vk::PipelineStageFlags& srcStageMask, vk::PipelineStageFlags dstStageMask,
                                        const vk::DependencyFlags& dependencyFlags,
                                        vk::ArrayProxy<const vk::MemoryBarrier> memoryBarriers,
                                        vk::ArrayProxy<const vk::BufferMemoryBarrier> bufferMemoryBarriers,
                                        vk::ArrayProxy<const vk::ImageMemoryBarrier> imageMemoryBarriers) const {
  vkCommandBuffer_.pipelineBarrier(srcStageMask, dstStageMask, dependencyFlags, memoryBarriers, bufferMemoryBarriers,
                                   imageMemoryBarriers, getDispatcher());
}

vk::ResultValueType<void>::type CommandBufferImpl::reset(const vk::CommandBufferResetFlags& flags) const {
  return vkCommandBuffer_.reset(flags, getDispatcher());
}

void CommandBufferImpl::resetEvent(vk::Event event, const vk::PipelineStageFlags& stageMask) const {
  vkCommandBuffer_.resetEvent(event, stageMask, getDispatcher());
}

void CommandBufferImpl::resetQueryPool(vk::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const {
  vkCommandBuffer_.resetQueryPool(queryPool, firstQuery, queryCount, getDispatcher());
}

void CommandBufferImpl::resolveImage(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Image dstImage,
                                     vk::ImageLayout dstImageLayout,
                                     vk::ArrayProxy<const vk::ImageResolve> regions) const {
  vkCommandBuffer_.resolveImage(srcImage, srcImageLayout, dstImage, dstImageLayout, regions, getDispatcher());
}

void CommandBufferImpl::setBlendConstants(const float* blendConstants) const {
  vkCommandBuffer_.setBlendConstants(blendConstants, getDispatcher());
}

void CommandBufferImpl::setDepthBias(float depthBiasConstantFactor, float depthBiasClamp,
                                     float depthBiasSlopeFactor) const {
  vkCommandBuffer_.setDepthBias(depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, getDispatcher());
}

void CommandBufferImpl::setDepthBounds(float minDepthBounds, float maxDepthBounds) const {
  vkCommandBuffer_.setDepthBounds(minDepthBounds, maxDepthBounds, getDispatcher());
}

void CommandBufferImpl::setDeviceMask(uint32_t deviceMask) const {
  vkCommandBuffer_.setDeviceMask(deviceMask, getDispatcher());
}

void CommandBufferImpl::setEvent(vk::Event event, vk::PipelineStageFlags stageMask) const {
  vkCommandBuffer_.setEvent(event, stageMask, getDispatcher());
}

void CommandBufferImpl::setLineWidth(float lineWidth) const {
  vkCommandBuffer_.setLineWidth(lineWidth, getDispatcher());
}

void CommandBufferImpl::setScissor(uint32_t firstScissor, vk::ArrayProxy<const vk::Rect2D> scissors) const {
  vkCommandBuffer_.setScissor(firstScissor, scissors, getDispatcher());
}

void CommandBufferImpl::setStencilCompareMask(vk::StencilFaceFlags faceMask, uint32_t compareMask) const {
  vkCommandBuffer_.setStencilCompareMask(faceMask, compareMask, getDispatcher());
}

void CommandBufferImpl::setStencilReference(vk::StencilFaceFlags faceMask, uint32_t reference) const {
  vkCommandBuffer_.setStencilReference(faceMask, reference, getDispatcher());
}

void CommandBufferImpl::setStencilWriteMask(vk::StencilFaceFlags faceMask, uint32_t writeMask) const {
  vkCommandBuffer_.setStencilWriteMask(faceMask, writeMask, getDispatcher());
}

void CommandBufferImpl::setViewport(uint32_t firstViewport, vk::ArrayProxy<const vk::Viewport> viewports) const {
  vkCommandBuffer_.setViewport(firstViewport, viewports, getDispatcher());
}

void CommandBufferImpl::waitEvents(vk::ArrayProxy<const vk::Event> events, const vk::PipelineStageFlags& srcStageMask,
                                   const vk::PipelineStageFlags& dstStageMask,
                                   vk::ArrayProxy<const vk::MemoryBarrier> memoryBarriers,
                                   vk::ArrayProxy<const vk::BufferMemoryBarrier> bufferMemoryBarriers,
                                   vk::ArrayProxy<const vk::ImageMemoryBarrier> imageMemoryBarriers) const {
  vkCommandBuffer_.waitEvents(events, srcStageMask, dstStageMask, memoryBarriers, bufferMemoryBarriers,
                              imageMemoryBarriers, getDispatcher());
}

void CommandBufferImpl::writeTimestamp(vk::PipelineStageFlagBits pipelineStage, vk::QueryPool queryPool,
                                       uint32_t query) const {
  vkCommandBuffer_.writeTimestamp(pipelineStage, queryPool, query, getDispatcher());
}

vk::ResultValueType<void>::type CommandBufferImpl::end() const {
  return vkCommandBuffer_.end(getDispatcher());
}

void CommandBufferImpl::beginRenderPass2KHR(const vk::RenderPassBeginInfo& renderPassBegin,
                                            const vk::SubpassBeginInfoKHR& subpassBeginInfo) const {
  vkCommandBuffer_.beginRenderPass2KHR(renderPassBegin, subpassBeginInfo, getDispatcher());
}

void CommandBufferImpl::dispatchBaseKHR(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ,
                                        uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const {
  vkCommandBuffer_.dispatchBaseKHR(baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ,
                                   getDispatcher());
}

void CommandBufferImpl::drawIndexedIndirectCountKHR(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                                    vk::DeviceSize countBufferOffset, uint32_t maxDrawCount,
                                                    uint32_t stride) const {
  vkCommandBuffer_.drawIndexedIndirectCountKHR(buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride,
                                               getDispatcher());
}

void CommandBufferImpl::drawIndirectCountKHR(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                             vk::DeviceSize countBufferOffset, uint32_t maxDrawCount,
                                             uint32_t stride) const {
  vkCommandBuffer_.drawIndirectCountKHR(buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride,
                                        getDispatcher());
}

void CommandBufferImpl::endRenderPass2KHR(const vk::SubpassEndInfoKHR& subpassEndInfo) const {
  vkCommandBuffer_.endRenderPass2KHR(subpassEndInfo, getDispatcher());
}

void CommandBufferImpl::nextSubpass2KHR(const vk::SubpassBeginInfoKHR& subpassBeginInfo,
                                        const vk::SubpassEndInfoKHR& subpassEndInfo) const {
  vkCommandBuffer_.nextSubpass2KHR(subpassBeginInfo, subpassEndInfo, getDispatcher());
}

void CommandBufferImpl::pushDescriptorSetKHR(vk::PipelineBindPoint pipelineBindPoint, vk::PipelineLayout layout,
                                             uint32_t set,
                                             vk::ArrayProxy<const vk::WriteDescriptorSet> descriptorWrites) const {
  vkCommandBuffer_.pushDescriptorSetKHR(pipelineBindPoint, layout, set, descriptorWrites, getDispatcher());
}

void CommandBufferImpl::pushDescriptorSetWithTemplateKHR(vk::DescriptorUpdateTemplate descriptorUpdateTemplate,
                                                         vk::PipelineLayout layout, uint32_t set,
                                                         const void* pData) const {
  vkCommandBuffer_.pushDescriptorSetWithTemplateKHR(descriptorUpdateTemplate, layout, set, pData, getDispatcher());
}

void CommandBufferImpl::setDeviceMaskKHR(uint32_t deviceMask) const {
  vkCommandBuffer_.setDeviceMaskKHR(deviceMask, getDispatcher());
}

void CommandBufferImpl::beginConditionalRenderingEXT(
  const vk::ConditionalRenderingBeginInfoEXT& conditionalRenderingBegin) const {
  vkCommandBuffer_.beginConditionalRenderingEXT(conditionalRenderingBegin, getDispatcher());
}

void CommandBufferImpl::beginDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& labelInfo) const {
  vkCommandBuffer_.beginDebugUtilsLabelEXT(labelInfo, getDispatcher());
}

void CommandBufferImpl::beginQueryIndexedEXT(vk::QueryPool queryPool, uint32_t query,
                                             const vk::QueryControlFlags& flags, uint32_t index) const {
  vkCommandBuffer_.beginQueryIndexedEXT(queryPool, query, flags, index, getDispatcher());
}

void CommandBufferImpl::beginTransformFeedbackEXT(uint32_t firstCounterBuffer,
                                                  vk::ArrayProxy<const vk::Buffer> counterBuffers,
                                                  vk::ArrayProxy<const vk::DeviceSize> counterBufferOffsets) const {
  vkCommandBuffer_.beginTransformFeedbackEXT(firstCounterBuffer, counterBuffers, counterBufferOffsets, getDispatcher());
}

void CommandBufferImpl::bindTransformFeedbackBuffersEXT(uint32_t firstBinding, vk::ArrayProxy<const vk::Buffer> buffers,
                                                        vk::ArrayProxy<const vk::DeviceSize> offsets,
                                                        vk::ArrayProxy<const vk::DeviceSize> sizes) const {
  vkCommandBuffer_.bindTransformFeedbackBuffersEXT(firstBinding, buffers, offsets, sizes, getDispatcher());
}

void CommandBufferImpl::debugMarkerBeginEXT(const vk::DebugMarkerMarkerInfoEXT& markerInfo) const {
  vkCommandBuffer_.debugMarkerBeginEXT(markerInfo, getDispatcher());
}

void CommandBufferImpl::debugMarkerEndEXT() const {
  vkCommandBuffer_.debugMarkerEndEXT(getDispatcher());
}

void CommandBufferImpl::debugMarkerInsertEXT(const vk::DebugMarkerMarkerInfoEXT& markerInfo) const {
  vkCommandBuffer_.debugMarkerInsertEXT(markerInfo, getDispatcher());
}
void CommandBufferImpl::drawIndirectByteCountEXT(uint32_t instanceCount, uint32_t firstInstance,
                                                 vk::Buffer counterBuffer, vk::DeviceSize counterBufferOffset,
                                                 uint32_t counterOffset, uint32_t vertexStride) const {
  vkCommandBuffer_.drawIndirectByteCountEXT(instanceCount, firstInstance, counterBuffer, counterBufferOffset,
                                            counterOffset, vertexStride, getDispatcher());
}

void CommandBufferImpl::endConditionalRenderingEXT() const {
  vkCommandBuffer_.endConditionalRenderingEXT(getDispatcher());
}

void CommandBufferImpl::endDebugUtilsLabelEXT() const {
  vkCommandBuffer_.endDebugUtilsLabelEXT(getDispatcher());
}

void CommandBufferImpl::endQueryIndexedEXT(vk::QueryPool queryPool, uint32_t query, uint32_t index) const {
  vkCommandBuffer_.endQueryIndexedEXT(queryPool, query, index, getDispatcher());
}

void CommandBufferImpl::endTransformFeedbackEXT(uint32_t firstCounterBuffer,
                                                vk::ArrayProxy<const vk::Buffer> counterBuffers,
                                                vk::ArrayProxy<const vk::DeviceSize> counterBufferOffsets) const {
  vkCommandBuffer_.endTransformFeedbackEXT(firstCounterBuffer, counterBuffers, counterBufferOffsets, getDispatcher());
}

void CommandBufferImpl::insertDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& labelInfo) const {
  vkCommandBuffer_.insertDebugUtilsLabelEXT(labelInfo, getDispatcher());
}

void CommandBufferImpl::setDiscardRectangleEXT(uint32_t firstDiscardRectangle,
                                               vk::ArrayProxy<const vk::Rect2D> discardRectangles) const {
  vkCommandBuffer_.setDiscardRectangleEXT(firstDiscardRectangle, discardRectangles, getDispatcher());
}

void CommandBufferImpl::setSampleLocationsEXT(const vk::SampleLocationsInfoEXT& sampleLocationsInfo) const {
  vkCommandBuffer_.setSampleLocationsEXT(sampleLocationsInfo, getDispatcher());
}

void CommandBufferImpl::bindShadingRateImageNV(vk::ImageView imageView, vk::ImageLayout imageLayout) const {
  vkCommandBuffer_.bindShadingRateImageNV(imageView, imageLayout, getDispatcher());
}

void CommandBufferImpl::buildAccelerationStructureNV(const vk::AccelerationStructureInfoNV& info,
                                                     vk::Buffer instanceData, vk::DeviceSize instanceOffset,
                                                     vk::Bool32 update, vk::AccelerationStructureNV dst,
                                                     vk::AccelerationStructureNV src, vk::Buffer scratch,
                                                     vk::DeviceSize scratchOffset) const {
  vkCommandBuffer_.buildAccelerationStructureNV(info, instanceData, instanceOffset, update, dst, src, scratch,
                                                scratchOffset, getDispatcher());
}

void CommandBufferImpl::copyAccelerationStructureNV(vk::AccelerationStructureNV dst, vk::AccelerationStructureNV src,
                                                    vk::CopyAccelerationStructureModeNV mode) const {
  vkCommandBuffer_.copyAccelerationStructureNV(dst, src, mode, getDispatcher());
}

void CommandBufferImpl::drawMeshTasksIndirectCountNV(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                                     vk::DeviceSize countBufferOffset, uint32_t maxDrawCount,
                                                     uint32_t stride) const {
  vkCommandBuffer_.drawMeshTasksIndirectCountNV(buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride,
                                                getDispatcher());
}

void CommandBufferImpl::drawMeshTasksIndirectNV(vk::Buffer buffer, vk::DeviceSize offset, uint32_t drawCount,
                                                uint32_t stride) const {
  vkCommandBuffer_.drawMeshTasksIndirectNV(buffer, offset, drawCount, stride, getDispatcher());
}

void CommandBufferImpl::drawMeshTasksNV(uint32_t taskCount, uint32_t firstTask) const {
  vkCommandBuffer_.drawMeshTasksNV(taskCount, firstTask, getDispatcher());
}

void CommandBufferImpl::setCheckpointNV(const void* pCheckpointMarker) const {
  vkCommandBuffer_.setCheckpointNV(pCheckpointMarker, getDispatcher());
}

void CommandBufferImpl::setCoarseSampleOrderNV(
  vk::CoarseSampleOrderTypeNV sampleOrderType,
  vk::ArrayProxy<const vk::CoarseSampleOrderCustomNV> customSampleOrders) const {
  vkCommandBuffer_.setCoarseSampleOrderNV(sampleOrderType, customSampleOrders, getDispatcher());
}

void CommandBufferImpl::setExclusiveScissorNV(uint32_t firstExclusiveScissor,
                                              vk::ArrayProxy<const vk::Rect2D> exclusiveScissors) const {
  vkCommandBuffer_.setExclusiveScissorNV(firstExclusiveScissor, exclusiveScissors, getDispatcher());
}

void CommandBufferImpl::setViewportShadingRatePaletteNV(
  uint32_t firstViewport, vk::ArrayProxy<const vk::ShadingRatePaletteNV> shadingRatePalettes) const {
  vkCommandBuffer_.setViewportShadingRatePaletteNV(firstViewport, shadingRatePalettes, getDispatcher());
}

void CommandBufferImpl::setViewportWScalingNV(uint32_t firstViewport,
                                              vk::ArrayProxy<const vk::ViewportWScalingNV> viewportWScalings) const {
  vkCommandBuffer_.setViewportWScalingNV(firstViewport, viewportWScalings, getDispatcher());
}

void CommandBufferImpl::traceRaysNV(vk::Buffer raygenShaderBindingTableBuffer, vk::DeviceSize raygenShaderBindingOffset,
                                    vk::Buffer missShaderBindingTableBuffer, vk::DeviceSize missShaderBindingOffset,
                                    vk::DeviceSize missShaderBindingStride, vk::Buffer hitShaderBindingTableBuffer,
                                    vk::DeviceSize hitShaderBindingOffset, vk::DeviceSize hitShaderBindingStride,
                                    vk::Buffer callableShaderBindingTableBuffer,
                                    vk::DeviceSize callableShaderBindingOffset,
                                    vk::DeviceSize callableShaderBindingStride, uint32_t width, uint32_t height,
                                    uint32_t depth) const {
  vkCommandBuffer_.traceRaysNV(raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer,
                               missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer,
                               hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer,
                               callableShaderBindingOffset, callableShaderBindingStride, width, height, depth,
                               getDispatcher());
}

void CommandBufferImpl::writeAccelerationStructuresPropertiesNV(
  vk::ArrayProxy<const vk::AccelerationStructureNV> accelerationStructures, vk::QueryType queryType,
  vk::QueryPool queryPool, uint32_t firstQuery) const {
  vkCommandBuffer_.writeAccelerationStructuresPropertiesNV(accelerationStructures, queryType, queryPool, firstQuery,
                                                           getDispatcher());
}

void CommandBufferImpl::processCommandsNVX(const vk::CmdProcessCommandsInfoNVX& processCommandsInfo) const {
  vkCommandBuffer_.processCommandsNVX(processCommandsInfo, getDispatcher());
}

void CommandBufferImpl::reserveSpaceForCommandsNVX(
  const vk::CmdReserveSpaceForCommandsInfoNVX& reserveSpaceInfo) const {
  vkCommandBuffer_.reserveSpaceForCommandsNVX(reserveSpaceInfo, getDispatcher());
}

void CommandBufferImpl::drawIndexedIndirectCountAMD(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                                    vk::DeviceSize countBufferOffset, uint32_t maxDrawCount,
                                                    uint32_t stride) const {
  vkCommandBuffer_.drawIndexedIndirectCountAMD(buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride,
                                               getDispatcher());
}

void CommandBufferImpl::drawIndirectCountAMD(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                             vk::DeviceSize countBufferOffset, uint32_t maxDrawCount,
                                             uint32_t stride) const {
  vkCommandBuffer_.drawIndirectCountAMD(buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride,
                                        getDispatcher());
}

void CommandBufferImpl::writeBufferMarkerAMD(vk::PipelineStageFlagBits pipelineStage, vk::Buffer dstBuffer,
                                             vk::DeviceSize dstOffset, uint32_t marker) const {
  vkCommandBuffer_.writeBufferMarkerAMD(pipelineStage, dstBuffer, dstOffset, marker, getDispatcher());
}

// endregion

// region Logi Definitions

VulkanInstanceImpl& CommandBufferImpl::getInstance() const {
  return commandPool_.getInstance();
}

PhysicalDeviceImpl& CommandBufferImpl::getPhysicalDevice() const {
  return commandPool_.getPhysicalDevice();
}

LogicalDeviceImpl& CommandBufferImpl::getLogicalDevice() const {
  return commandPool_.getLogicalDevice();
}

CommandPoolImpl& CommandBufferImpl::getCommandPool() const {
  return commandPool_;
}

const vk::DispatchLoaderDynamic& CommandBufferImpl::getDispatcher() const {
  return commandPool_.getDispatcher();
}

void CommandBufferImpl::destroy() const {
  commandPool_.freeCommandBuffers({id()});
}

CommandBufferImpl::operator vk::CommandBuffer() const {
  return vkCommandBuffer_;
}

void CommandBufferImpl::free() {
  vkCommandBuffer_ = nullptr;
  VulkanObject::free();
}

// endregion

}