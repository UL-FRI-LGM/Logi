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

#include "logi/command/command_buffer.hpp"
#include "logi/command/command_pool.hpp"
#include "logi/command/command_pool_impl.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/queue/queue_family.hpp"
#include "logi/queue/queue_family_impl.hpp"

namespace logi {

// region Vulkan Definitions

vk::ResultValueType<void>::type CommandBuffer::begin(const vk::CommandBufferBeginInfo& beginInfo) const {
  return object_->begin(beginInfo);
}

void CommandBuffer::beginQuery(vk::QueryPool queryPool, uint32_t query, const vk::QueryControlFlags& flags) const {
  object_->beginQuery(queryPool, query, flags);
}

void CommandBuffer::beginRenderPass(const vk::RenderPassBeginInfo& renderPassBegin,
                                    vk::SubpassContents contents) const {
  object_->beginRenderPass(renderPassBegin, contents);
}

void CommandBuffer::beginRenderPass2(const vk::RenderPassBeginInfo& renderPassBegin,
                                     vk::SubpassContents contents) const {
  object_->beginRenderPass2(renderPassBegin, contents);
}

void CommandBuffer::bindDescriptorSets(vk::PipelineBindPoint pipelineBindPoint, vk::PipelineLayout layout,
                                       uint32_t firstSet, vk::ArrayProxy<const vk::DescriptorSet> descriptorSets,
                                       vk::ArrayProxy<const uint32_t> dynamicOffsets) const {
  object_->bindDescriptorSets(pipelineBindPoint, layout, firstSet, descriptorSets, dynamicOffsets);
}

void CommandBuffer::bindIndexBuffer(vk::Buffer buffer, vk::DeviceSize offset, vk::IndexType indexType) const {
  object_->bindIndexBuffer(buffer, offset, indexType);
}

void CommandBuffer::bindPipeline(vk::PipelineBindPoint pipelineBindPoint, vk::Pipeline pipeline) const {
  object_->bindPipeline(pipelineBindPoint, pipeline);
}

void CommandBuffer::bindVertexBuffers(uint32_t firstBinding, vk::ArrayProxy<const vk::Buffer> buffers,
                                      vk::ArrayProxy<const vk::DeviceSize> offsets) const {
  object_->bindVertexBuffers(firstBinding, buffers, offsets);
}

void CommandBuffer::blitImage(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Image dstImage,
                              vk::ImageLayout dstImageLayout, vk::ArrayProxy<const vk::ImageBlit> regions,
                              vk::Filter filter) const {
  object_->blitImage(srcImage, srcImageLayout, dstImage, dstImageLayout, regions, filter);
}

void CommandBuffer::clearAttachments(vk::ArrayProxy<const vk::ClearAttachment> attachments,
                                     vk::ArrayProxy<const vk::ClearRect> rects) const {
  object_->clearAttachments(attachments, rects);
}

void CommandBuffer::clearColorImage(vk::Image image, vk::ImageLayout imageLayout, const vk::ClearColorValue& color,
                                    vk::ArrayProxy<const vk::ImageSubresourceRange> ranges) const {
  object_->clearColorImage(image, imageLayout, color, ranges);
}

void CommandBuffer::clearDepthStencilImage(vk::Image image, vk::ImageLayout imageLayout,
                                           const vk::ClearDepthStencilValue& depthStencil,
                                           vk::ArrayProxy<const vk::ImageSubresourceRange> ranges) const {
  object_->clearDepthStencilImage(image, imageLayout, depthStencil, ranges);
}

void CommandBuffer::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer,
                               vk::ArrayProxy<const vk::BufferCopy> regions) const {
  object_->copyBuffer(srcBuffer, dstBuffer, regions);
}

void CommandBuffer::copyBufferToImage(vk::Buffer srcBuffer, vk::Image dstImage, vk::ImageLayout dstImageLayout,
                                      vk::ArrayProxy<const vk::BufferImageCopy> regions) const {
  object_->copyBufferToImage(srcBuffer, dstImage, dstImageLayout, regions);
}

void CommandBuffer::copyImage(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Image dstImage,
                              vk::ImageLayout dstImageLayout, vk::ArrayProxy<const vk::ImageCopy> regions) const {
  object_->copyImage(srcImage, srcImageLayout, dstImage, dstImageLayout, regions);
}

void CommandBuffer::copyImageToBuffer(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Buffer dstBuffer,
                                      vk::ArrayProxy<const vk::BufferImageCopy> regions) const {
  object_->copyImageToBuffer(srcImage, srcImageLayout, dstBuffer, regions);
}

void CommandBuffer::copyQueryPoolResults(vk::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount,
                                         vk::Buffer dstBuffer, vk::DeviceSize dstOffset, vk::DeviceSize stride,
                                         const vk::QueryResultFlags& flags) const {
  object_->copyQueryPoolResults(queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}

void CommandBuffer::dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const {
  object_->dispatch(groupCountX, groupCountY, groupCountZ);
}

void CommandBuffer::dispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX,
                                 uint32_t groupCountY, uint32_t groupCountZ) const {
  object_->dispatchBase(baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void CommandBuffer::dispatchIndirect(vk::Buffer buffer, vk::DeviceSize offset) const {
  object_->dispatchIndirect(buffer, offset);
}

void CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex,
                         uint32_t firstInstance) const {
  object_->draw(vertexCount, instanceCount, firstVertex, firstInstance);
}

void CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
                                uint32_t firstInstance) const {
  object_->drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void CommandBuffer::drawIndexedIndirect(vk::Buffer buffer, vk::DeviceSize offset, uint32_t drawCount,
                                        uint32_t stride) const {
  object_->drawIndexedIndirect(buffer, offset, drawCount, stride);
}

void CommandBuffer::drawIndexedIndirectCount(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                             vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const {
  object_->drawIndexedIndirectCount(buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void CommandBuffer::drawIndirect(vk::Buffer buffer, vk::DeviceSize offset, uint32_t drawCount, uint32_t stride) const {
  object_->drawIndirect(buffer, offset, drawCount, stride);
}

void CommandBuffer::drawIndirectCount(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                      vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const {
  object_->drawIndirectCount(buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void CommandBuffer::endQuery(vk::QueryPool queryPool, uint32_t query) const {
  object_->endQuery(queryPool, query);
}

void CommandBuffer::endRenderPass() const {
  object_->endRenderPass();
}

void CommandBuffer::endRenderPass2(const vk::SubpassEndInfo& subpassEndInfo) const {
  object_->endRenderPass2(subpassEndInfo);
}

void CommandBuffer::executeCommands(vk::ArrayProxy<const vk::CommandBuffer> commandBuffers) const {
  object_->executeCommands(commandBuffers);
}

void CommandBuffer::fillBuffer(vk::Buffer dstBuffer, vk::DeviceSize dstOffset, vk::DeviceSize size,
                               uint32_t data) const {
  object_->fillBuffer(dstBuffer, dstOffset, size, data);
}

void CommandBuffer::nextSubpass(vk::SubpassContents contents) const {
  object_->nextSubpass(contents);
}

void CommandBuffer::nextSubpass2(vk::SubpassBeginInfo subpassBeginInfo, vk::SubpassEndInfo subpassEndInfo) const {
  object_->nextSubpass2(subpassBeginInfo, subpassEndInfo);
}

void CommandBuffer::pipelineBarrier(const vk::PipelineStageFlags& srcStageMask, vk::PipelineStageFlags dstStageMask,
                                    const vk::DependencyFlags& dependencyFlags,
                                    vk::ArrayProxy<const vk::MemoryBarrier> memoryBarriers,
                                    vk::ArrayProxy<const vk::BufferMemoryBarrier> bufferMemoryBarriers,
                                    vk::ArrayProxy<const vk::ImageMemoryBarrier> imageMemoryBarriers) const {
  object_->pipelineBarrier(srcStageMask, dstStageMask, dependencyFlags, memoryBarriers, bufferMemoryBarriers,
                           imageMemoryBarriers);
}

vk::ResultValueType<void>::type CommandBuffer::reset(const vk::CommandBufferResetFlags& flags) const {
  return object_->reset(flags);
}

void CommandBuffer::resetEvent(vk::Event event, const vk::PipelineStageFlags& stageMask) const {
  object_->resetEvent(event, stageMask);
}

void CommandBuffer::resetQueryPool(vk::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const {
  object_->resetQueryPool(queryPool, firstQuery, queryCount);
}

void CommandBuffer::resolveImage(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Image dstImage,
                                 vk::ImageLayout dstImageLayout, vk::ArrayProxy<const vk::ImageResolve> regions) const {
  object_->resolveImage(srcImage, srcImageLayout, dstImage, dstImageLayout, regions);
}

void CommandBuffer::setBlendConstants(const float* blendConstants) const {
  object_->setBlendConstants(blendConstants);
}

void CommandBuffer::setDepthBias(float depthBiasConstantFactor, float depthBiasClamp,
                                 float depthBiasSlopeFactor) const {
  object_->setDepthBias(depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void CommandBuffer::setDepthBounds(float minDepthBounds, float maxDepthBounds) const {
  object_->setDepthBounds(minDepthBounds, maxDepthBounds);
}

void CommandBuffer::setDeviceMask(uint32_t deviceMask) const {
  object_->setDeviceMask(deviceMask);
}

void CommandBuffer::setEvent(vk::Event event, vk::PipelineStageFlags stageMask) const {
  object_->setEvent(event, stageMask);
}

void CommandBuffer::setLineWidth(float lineWidth) const {
  object_->setLineWidth(lineWidth);
}

void CommandBuffer::setScissor(uint32_t firstScissor, vk::ArrayProxy<const vk::Rect2D> scissors) const {
  object_->setScissor(firstScissor, scissors);
}

void CommandBuffer::setStencilCompareMask(vk::StencilFaceFlags faceMask, uint32_t compareMask) const {
  object_->setStencilCompareMask(faceMask, compareMask);
}

void CommandBuffer::setStencilReference(vk::StencilFaceFlags faceMask, uint32_t reference) const {
  object_->setStencilReference(faceMask, reference);
}

void CommandBuffer::setStencilWriteMask(vk::StencilFaceFlags faceMask, uint32_t writeMask) const {
  object_->setStencilWriteMask(faceMask, writeMask);
}

void CommandBuffer::setViewport(uint32_t firstViewport, vk::ArrayProxy<const vk::Viewport> viewports) const {
  object_->setViewport(firstViewport, viewports);
}

void CommandBuffer::waitEvents(vk::ArrayProxy<const vk::Event> events, const vk::PipelineStageFlags& srcStageMask,
                               const vk::PipelineStageFlags& dstStageMask,
                               vk::ArrayProxy<const vk::MemoryBarrier> memoryBarriers,
                               vk::ArrayProxy<const vk::BufferMemoryBarrier> bufferMemoryBarriers,
                               vk::ArrayProxy<const vk::ImageMemoryBarrier> imageMemoryBarriers) const {
  object_->waitEvents(events, srcStageMask, dstStageMask, memoryBarriers, bufferMemoryBarriers, imageMemoryBarriers);
}

void CommandBuffer::writeTimestamp(vk::PipelineStageFlagBits pipelineStage, vk::QueryPool queryPool,
                                   uint32_t query) const {
  object_->writeTimestamp(pipelineStage, queryPool, query);
}

vk::ResultValueType<void>::type CommandBuffer::end() const {
  return object_->end();
}

void CommandBuffer::beginRenderPass2KHR(const vk::RenderPassBeginInfo& renderPassBegin,
                                        const vk::SubpassBeginInfoKHR& subpassBeginInfo) const {
  object_->beginRenderPass2KHR(renderPassBegin, subpassBeginInfo);
}

void CommandBuffer::dispatchBaseKHR(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX,
                                    uint32_t groupCountY, uint32_t groupCountZ) const {
  object_->dispatchBaseKHR(baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void CommandBuffer::drawIndexedIndirectCountKHR(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                                vk::DeviceSize countBufferOffset, uint32_t maxDrawCount,
                                                uint32_t stride) const {
  object_->drawIndexedIndirectCountKHR(buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void CommandBuffer::drawIndirectCountKHR(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                         vk::DeviceSize countBufferOffset, uint32_t maxDrawCount,
                                         uint32_t stride) const {
  object_->drawIndirectCountKHR(buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void CommandBuffer::endRenderPass2KHR(const vk::SubpassEndInfoKHR& subpassEndInfo) const {
  object_->endRenderPass2KHR(subpassEndInfo);
}

void CommandBuffer::nextSubpass2KHR(const vk::SubpassBeginInfoKHR& subpassBeginInfo,
                                    const vk::SubpassEndInfoKHR& subpassEndInfo) const {
  object_->nextSubpass2KHR(subpassBeginInfo, subpassEndInfo);
}

void CommandBuffer::pushDescriptorSetKHR(vk::PipelineBindPoint pipelineBindPoint, vk::PipelineLayout layout,
                                         uint32_t set,
                                         vk::ArrayProxy<const vk::WriteDescriptorSet> descriptorWrites) const {
  object_->pushDescriptorSetKHR(pipelineBindPoint, layout, set, descriptorWrites);
}

void CommandBuffer::pushDescriptorSetWithTemplateKHR(vk::DescriptorUpdateTemplate descriptorUpdateTemplate,
                                                     vk::PipelineLayout layout, uint32_t set, const void* pData) const {
  object_->pushDescriptorSetWithTemplateKHR(descriptorUpdateTemplate, layout, set, pData);
}

void CommandBuffer::setDeviceMaskKHR(uint32_t deviceMask) const {
  object_->setDeviceMaskKHR(deviceMask);
}

void CommandBuffer::buildAccelerationStructuresKHR(const vk::ArrayProxy<const vk::AccelerationStructureBuildGeometryInfoKHR> buildGeometryInfos,
                                                   const vk::ArrayProxy<const vk::AccelerationStructureBuildRangeInfoKHR *const> buildRangeInfos) const {
  object_->buildAccelerationStructuresKHR(buildGeometryInfos, buildRangeInfos);
}

void CommandBuffer::buildAccelerationStructuresIndirectKHR(const vk::ArrayProxy<const vk::AccelerationStructureBuildGeometryInfoKHR> buildGeometryInfos, 
                                                           const vk::ArrayProxy<const vk::DeviceAddress> indirectDeviceAddresses,
                                                           const vk::ArrayProxy<const uint32_t> indirectStrides, 
                                                           const vk::ArrayProxy<const uint32_t *const> maxPrimitiveCounts) const {
  object_->buildAccelerationStructuresIndirectKHR(buildGeometryInfos, indirectDeviceAddresses, indirectStrides, maxPrimitiveCounts);
}

void CommandBuffer::copyAccelerationStructureKHR(const vk::CopyAccelerationStructureInfoKHR& copyAccelerationStructureInfo) const {
  object_->copyAccelerationStructureKHR(copyAccelerationStructureInfo);
}

void CommandBuffer::copyAccelerationStructureToMemoryKHR(const vk::CopyAccelerationStructureToMemoryInfoKHR& copyAccelerationStructureToMemoryInfo) const {
  object_->copyAccelerationStructureToMemoryKHR(copyAccelerationStructureToMemoryInfo);
}              

void CommandBuffer::copyMemoryToAccelerationStructureKHR(const vk::CopyMemoryToAccelerationStructureInfoKHR& copyMemoryToAccelerationStructureInfo) const {
  object_->copyMemoryToAccelerationStructureKHR(copyMemoryToAccelerationStructureInfo);
}                      

void CommandBuffer::writeAccelerationStructuresPropertiesKHR(const vk::ArrayProxy<const vk::AccelerationStructureKHR> accelerationStructures,
                                                             vk::QueryType queryType, vk::QueryPool queryPool, uint32_t firstQuery) const {
  object_->writeAccelerationStructuresPropertiesKHR(accelerationStructures, queryType, queryPool, firstQuery);
}

void CommandBuffer::beginConditionalRenderingEXT(
  const vk::ConditionalRenderingBeginInfoEXT& conditionalRenderingBegin) const {
  object_->beginConditionalRenderingEXT(conditionalRenderingBegin);
}

void CommandBuffer::beginDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& labelInfo) const {
  object_->beginDebugUtilsLabelEXT(labelInfo);
}

void CommandBuffer::beginQueryIndexedEXT(vk::QueryPool queryPool, uint32_t query, const vk::QueryControlFlags& flags,
                                         uint32_t index) const {
  object_->beginQueryIndexedEXT(queryPool, query, flags, index);
}

void CommandBuffer::beginTransformFeedbackEXT(uint32_t firstCounterBuffer,
                                              vk::ArrayProxy<const vk::Buffer> counterBuffers,
                                              vk::ArrayProxy<const vk::DeviceSize> counterBufferOffsets) const {
  object_->beginTransformFeedbackEXT(firstCounterBuffer, counterBuffers, counterBufferOffsets);
}

void CommandBuffer::bindTransformFeedbackBuffersEXT(uint32_t firstBinding, vk::ArrayProxy<const vk::Buffer> buffers,
                                                    vk::ArrayProxy<const vk::DeviceSize> offsets,
                                                    vk::ArrayProxy<const vk::DeviceSize> sizes) const {
  object_->bindTransformFeedbackBuffersEXT(firstBinding, buffers, offsets, sizes);
}

void CommandBuffer::debugMarkerBeginEXT(const vk::DebugMarkerMarkerInfoEXT& markerInfo) const {
  object_->debugMarkerBeginEXT(markerInfo);
}

void CommandBuffer::debugMarkerEndEXT() const {
  object_->debugMarkerEndEXT();
}

void CommandBuffer::debugMarkerInsertEXT(const vk::DebugMarkerMarkerInfoEXT& markerInfo) const {
  object_->debugMarkerInsertEXT(markerInfo);
}

void CommandBuffer::drawIndirectByteCountEXT(uint32_t instanceCount, uint32_t firstInstance, vk::Buffer counterBuffer,
                                             vk::DeviceSize counterBufferOffset, uint32_t counterOffset,
                                             uint32_t vertexStride) const {
  object_->drawIndirectByteCountEXT(instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset,
                                    vertexStride);
}

void CommandBuffer::endConditionalRenderingEXT() const {
  object_->endConditionalRenderingEXT();
}

void CommandBuffer::endDebugUtilsLabelEXT() const {
  object_->endDebugUtilsLabelEXT();
}

void CommandBuffer::endQueryIndexedEXT(vk::QueryPool queryPool, uint32_t query, uint32_t index) const {
  object_->endQueryIndexedEXT(queryPool, query, index);
}

void CommandBuffer::endTransformFeedbackEXT(uint32_t firstCounterBuffer,
                                            vk::ArrayProxy<const vk::Buffer> counterBuffers,
                                            vk::ArrayProxy<const vk::DeviceSize> counterBufferOffsets) const {
  object_->endTransformFeedbackEXT(firstCounterBuffer, counterBuffers, counterBufferOffsets);
}

void CommandBuffer::insertDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& labelInfo) const {
  object_->insertDebugUtilsLabelEXT(labelInfo);
}

void CommandBuffer::setDiscardRectangleEXT(uint32_t firstDiscardRectangle,
                                           vk::ArrayProxy<const vk::Rect2D> discardRectangles) const {
  object_->setDiscardRectangleEXT(firstDiscardRectangle, discardRectangles);
}

void CommandBuffer::setSampleLocationsEXT(const vk::SampleLocationsInfoEXT& sampleLocationsInfo) const {
  object_->setSampleLocationsEXT(sampleLocationsInfo);
}

void CommandBuffer::bindShadingRateImageNV(vk::ImageView imageView, vk::ImageLayout imageLayout) const {
  object_->bindShadingRateImageNV(imageView, imageLayout);
}

void CommandBuffer::buildAccelerationStructureNV(const vk::AccelerationStructureInfoNV& info, vk::Buffer instanceData,
                                                 vk::DeviceSize instanceOffset, vk::Bool32 update,
                                                 vk::AccelerationStructureNV dst, vk::AccelerationStructureNV src,
                                                 vk::Buffer scratch, vk::DeviceSize scratchOffset) const {
  object_->buildAccelerationStructureNV(info, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}

void CommandBuffer::copyAccelerationStructureNV(vk::AccelerationStructureNV dst, vk::AccelerationStructureNV src,
                                                vk::CopyAccelerationStructureModeNV mode) const {
  object_->copyAccelerationStructureNV(dst, src, mode);
}

void CommandBuffer::drawMeshTasksIndirectCountNV(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                                 vk::DeviceSize countBufferOffset, uint32_t maxDrawCount,
                                                 uint32_t stride) const {
  object_->drawMeshTasksIndirectCountNV(buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void CommandBuffer::drawMeshTasksIndirectNV(vk::Buffer buffer, vk::DeviceSize offset, uint32_t drawCount,
                                            uint32_t stride) const {
  object_->drawMeshTasksIndirectNV(buffer, offset, drawCount, stride);
}

void CommandBuffer::drawMeshTasksNV(uint32_t taskCount, uint32_t firstTask) const {
  object_->drawMeshTasksNV(taskCount, firstTask);
}

void CommandBuffer::setCheckpointNV(const void* pCheckpointMarker) const {
  object_->setCheckpointNV(pCheckpointMarker);
}

void CommandBuffer::setCoarseSampleOrderNV(
  vk::CoarseSampleOrderTypeNV sampleOrderType,
  vk::ArrayProxy<const vk::CoarseSampleOrderCustomNV> customSampleOrders) const {
  object_->setCoarseSampleOrderNV(sampleOrderType, customSampleOrders);
}

void CommandBuffer::setExclusiveScissorNV(uint32_t firstExclusiveScissor,
                                          vk::ArrayProxy<const vk::Rect2D> exclusiveScissors) const {
  object_->setExclusiveScissorNV(firstExclusiveScissor, exclusiveScissors);
}

void CommandBuffer::setViewportShadingRatePaletteNV(
  uint32_t firstViewport, vk::ArrayProxy<const vk::ShadingRatePaletteNV> shadingRatePalettes) const {
  object_->setViewportShadingRatePaletteNV(firstViewport, shadingRatePalettes);
}

void CommandBuffer::setViewportWScalingNV(uint32_t firstViewport,
                                          vk::ArrayProxy<const vk::ViewportWScalingNV> viewportWScalings) const {
  object_->setViewportWScalingNV(firstViewport, viewportWScalings);
}

void CommandBuffer::traceRaysNV(vk::Buffer raygenShaderBindingTableBuffer, vk::DeviceSize raygenShaderBindingOffset,
                                vk::Buffer missShaderBindingTableBuffer, vk::DeviceSize missShaderBindingOffset,
                                vk::DeviceSize missShaderBindingStride, vk::Buffer hitShaderBindingTableBuffer,
                                vk::DeviceSize hitShaderBindingOffset, vk::DeviceSize hitShaderBindingStride,
                                vk::Buffer callableShaderBindingTableBuffer, vk::DeviceSize callableShaderBindingOffset,
                                vk::DeviceSize callableShaderBindingStride, uint32_t width, uint32_t height,
                                uint32_t depth) const {
  object_->traceRaysNV(raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer,
                       missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer,
                       hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer,
                       callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}

void CommandBuffer::writeAccelerationStructuresPropertiesNV(
  vk::ArrayProxy<const vk::AccelerationStructureNV> accelerationStructures, vk::QueryType queryType,
  vk::QueryPool queryPool, uint32_t firstQuery) const {
  object_->writeAccelerationStructuresPropertiesNV(accelerationStructures, queryType, queryPool, firstQuery);
}

void CommandBuffer::bindPipelineShaderGroupNV(vk::PipelineBindPoint pipelineBindPoint,
                                              vk::Pipeline pipeline, uint32_t groupIndex) const {
  object_->bindPipelineShaderGroupNV(pipelineBindPoint, pipeline, groupIndex);                                              
} 

void CommandBuffer::preprocessGeneratedCommandsNV(const VkGeneratedCommandsInfoNV& generatedCommandsInfo) const {
  object_->preprocessGeneratedCommandsNV(generatedCommandsInfo);
}

void CommandBuffer::executeGeneratedCommandsNV(vk::Bool32 isPreprocessed, const VkGeneratedCommandsInfoNV& generatedCommandsInfo) const {
  object_->executeGeneratedCommandsNV(isPreprocessed, generatedCommandsInfo);
}


// Deprecated

// void CommandBuffer::processCommandsNVX(const vk::CmdProcessCommandsInfoNVX& processCommandsInfo) const {
//   object_->processCommandsNVX(processCommandsInfo);
// }

// void CommandBuffer::reserveSpaceForCommandsNVX(const vk::CmdReserveSpaceForCommandsInfoNVX& reserveSpaceInfo) const {
//   object_->reserveSpaceForCommandsNVX(reserveSpaceInfo);
// }


void CommandBuffer::drawIndexedIndirectCountAMD(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                                vk::DeviceSize countBufferOffset, uint32_t maxDrawCount,
                                                uint32_t stride) const {
  object_->drawIndexedIndirectCountAMD(buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void CommandBuffer::drawIndirectCountAMD(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                         vk::DeviceSize countBufferOffset, uint32_t maxDrawCount,
                                         uint32_t stride) const {
  object_->drawIndirectCountAMD(buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void CommandBuffer::writeBufferMarkerAMD(vk::PipelineStageFlagBits pipelineStage, vk::Buffer dstBuffer,
                                         vk::DeviceSize dstOffset, uint32_t marker) const {
  object_->writeBufferMarkerAMD(pipelineStage, dstBuffer, dstOffset, marker);
}

// endregion

// region Logi Definitions

VulkanInstance CommandBuffer::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice CommandBuffer::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice CommandBuffer::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

QueueFamily CommandBuffer::getQueueFamily() const {
  return QueueFamily(object_->getQueueFamily().shared_from_this());
}

CommandPool CommandBuffer::getCommandPool() const {
  return CommandPool(object_->getCommandPool().shared_from_this());
}

const vk::DispatchLoaderDynamic& CommandBuffer::getDispatcher() const {
  return object_->getDispatcher();
}

void CommandBuffer::destroy() const {
  if (object_) {
    object_->destroy();
  }
}

CommandBuffer::operator const vk::CommandBuffer&() const {
  static vk::CommandBuffer nullHandle(nullptr);
  return (object_) ? object_->operator const vk::CommandBuffer&() : nullHandle;
}

// endregion

}