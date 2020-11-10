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

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"
#include "logi/command/command_buffer_impl.hpp"

namespace logi {

class CommandPoolImpl;
class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;
class CommandPool;
class QueueFamily;

class CommandBuffer : public Handle<CommandBufferImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkBeginCommandBuffer.html">vkBeginCommandBuffer</a>
   */
  vk::ResultValueType<void>::type begin(const vk::CommandBufferBeginInfo& beginInfo = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBeginQuery.html">vkCmdBeginQuery</a>
   */
  void beginQuery(vk::QueryPool queryPool, uint32_t query, const vk::QueryControlFlags& flags) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBeginRenderPass.html">vkCmdBeginRenderPass</a>
   */
  void beginRenderPass(const vk::RenderPassBeginInfo& renderPassBegin, vk::SubpassContents contents) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBeginRenderPass2.html">vkCmdBeginRenderPass2</a>
   */
  void beginRenderPass2(const vk::RenderPassBeginInfo& renderPassBegin, vk::SubpassContents contens) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBindDescriptorSets.html">vkCmdBindDescriptorSets</a>
   */
  void bindDescriptorSets(vk::PipelineBindPoint pipelineBindPoint, vk::PipelineLayout layout, uint32_t firstSet,
                          vk::ArrayProxy<const vk::DescriptorSet> descriptorSets,
                          vk::ArrayProxy<const uint32_t> dynamicOffsets = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBindIndexBuffer.html">vkCmdBindIndexBuffer</a>
   */
  void bindIndexBuffer(vk::Buffer buffer, vk::DeviceSize offset, vk::IndexType indexType) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBindPipeline.html">vkCmdBindPipeline</a>
   */
  void bindPipeline(vk::PipelineBindPoint pipelineBindPoint, vk::Pipeline pipeline) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBindVertexBuffers.html">vkCmdBindVertexBuffers</a>
   */
  void bindVertexBuffers(uint32_t firstBinding, vk::ArrayProxy<const vk::Buffer> buffers,
                         vk::ArrayProxy<const vk::DeviceSize> offsets) const;
 
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBlitImage.html">vkCmdBlitImage</a>
   */
  void blitImage(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Image dstImage, vk::ImageLayout dstImageLayout,
                 vk::ArrayProxy<const vk::ImageBlit> regions, vk::Filter filter) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdClearAttachments.html">vkCmdClearAttachments</a>
   */
  void clearAttachments(vk::ArrayProxy<const vk::ClearAttachment> attachments,
                        vk::ArrayProxy<const vk::ClearRect> rects) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdClearColorImage.html">vkCmdClearColorImage</a>
   */
  void clearColorImage(vk::Image image, vk::ImageLayout imageLayout, const vk::ClearColorValue& color,
                       vk::ArrayProxy<const vk::ImageSubresourceRange> ranges) const;
 
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdClearDepthStencilImage.html">vkCmdClearDepthStencilImage</a>
   */
  void clearDepthStencilImage(vk::Image image, vk::ImageLayout imageLayout,
                              const vk::ClearDepthStencilValue& depthStencil,
                              vk::ArrayProxy<const vk::ImageSubresourceRange> ranges) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdCopyBuffer.html">vkCmdCopyBuffer</a>
   */
  void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::ArrayProxy<const vk::BufferCopy> regions) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdCopyBufferToImage.html">vkCmdCopyBufferToImage</a>
   */
  void copyBufferToImage(vk::Buffer srcBuffer, vk::Image dstImage, vk::ImageLayout dstImageLayout,
                         vk::ArrayProxy<const vk::BufferImageCopy> regions) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdCopyImage.html">vkCmdCopyImage</a>
   */
  void copyImage(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Image dstImage, vk::ImageLayout dstImageLayout,
                 vk::ArrayProxy<const vk::ImageCopy> regions) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdCopyImageToBuffer.html">vkCmdCopyImageToBuffer</a>
   */
  void copyImageToBuffer(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Buffer dstBuffer,
                         vk::ArrayProxy<const vk::BufferImageCopy> regions) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdCopyQueryPoolResults.html">vkCmdCopyQueryPoolResults</a>
   */
  void copyQueryPoolResults(vk::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, vk::Buffer dstBuffer,
                            vk::DeviceSize dstOffset, vk::DeviceSize stride, const vk::QueryResultFlags& flags) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDispatch.html">vkCmdDispatch</a>
   */
  void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDispatchBase.html">vkCmdDispatchBase</a>
   */
  void dispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX,
                    uint32_t groupCountY, uint32_t groupCountZ) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDispatchIndirect.html">vkCmdDispatchIndirect</a>
   */
  void dispatchIndirect(vk::Buffer buffer, vk::DeviceSize offset) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDraw.html">vkCmdDraw</a>
   */
  void draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0,
            uint32_t firstInstance = 0) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawIndexed.html">vkCmdDrawIndexed</a>
   */
  void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
                   uint32_t firstInstance) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawIndexedIndirect.html">vkCmdDrawIndexedIndirect</a>
   */
  void drawIndexedIndirect(vk::Buffer buffer, vk::DeviceSize offset, uint32_t drawCount, uint32_t stride) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawIndexedIndirectCount.html">vkCmdDrawIndexedIndirectCount</a>
   */
  void drawIndexedIndirectCount(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawIndirect.html">vkCmdDrawIndirect</a>
   */
  void drawIndirect(vk::Buffer buffer, vk::DeviceSize offset, uint32_t drawCount, uint32_t stride) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawIndirectCount.html">vkCmdDrawIndirectCount</a>
   */
  void drawIndirectCount(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                         vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdEndQuery.html">vkCmdDraw</a>
   */
  void endQuery(vk::QueryPool queryPool, uint32_t query) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdEndRenderPass.html">vkCmdEndRenderPass</a>
   */
  void endRenderPass() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdEndRenderPass2.html">vkCmdEndRenderPass2</a>
   */
  void endRenderPass2(const vk::SubpassEndInfo& subpassEndInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdExecuteCommands.html">vkCmdExecuteCommands</a>
   */
  void executeCommands(vk::ArrayProxy<const vk::CommandBuffer> commandBuffers) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdFillBuffer.html">vkCmdFillBuffer</a>
   */
  void fillBuffer(vk::Buffer dstBuffer, vk::DeviceSize dstOffset, vk::DeviceSize size, uint32_t data) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdNextSubpass.html">vkCmdNextSubpass</a>
   */
  void nextSubpass(vk::SubpassContents contents) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdNextSubpass2.html">vkCmdNextSubpass2</a>
   */
  void nextSubpass2(vk::SubpassBeginInfo subpassBeginInfo, vk::SubpassEndInfo subpassEndInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdPipelineBarrier.html">vkCmdPipelineBarrier</a>
   */
  void pipelineBarrier(const vk::PipelineStageFlags& srcStageMask, vk::PipelineStageFlags dstStageMask,
                       const vk::DependencyFlags& dependencyFlags,
                       vk::ArrayProxy<const vk::MemoryBarrier> memoryBarriers,
                       vk::ArrayProxy<const vk::BufferMemoryBarrier> bufferMemoryBarriers,
                       vk::ArrayProxy<const vk::ImageMemoryBarrier> imageMemoryBarriers) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdPushConstants.html">vkCmdPushConstants</a>
   */
  template <typename T>
  void pushConstants(vk::PipelineLayout layout, vk::ShaderStageFlags stageFlags, uint32_t offset,
                     vk::ArrayProxy<const T> values) const {
    object_->pushConstants(layout, stageFlags, offset, values);
  }

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkResetCommandBuffer.html">vkResetCommandBuffer</a>
   */
  vk::ResultValueType<void>::type reset(const vk::CommandBufferResetFlags& flags = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkResetEvent.html">vkResetEvent</a>
   */
  void resetEvent(vk::Event event, const vk::PipelineStageFlags& stageMask) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkResetQueryPool.html">vkResetQueryPool</a>
   */
  void resetQueryPool(vk::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdResolveImage.html">vkCmdResolveImage</a>
   */
  void resolveImage(vk::Image srcImage, vk::ImageLayout srcImageLayout, vk::Image dstImage,
                    vk::ImageLayout dstImageLayout, vk::ArrayProxy<const vk::ImageResolve> regions) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetBlendConstants.html">vkCmdSetBlendConstants</a>
   */
  void setBlendConstants(const float blendConstants[4]) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetDepthBias.html">vkCmdSetDepthBias</a>
   */
  void setDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetDepthBounds.html">vkCmdSetDepthBounds</a>
   */
  void setDepthBounds(float minDepthBounds, float maxDepthBounds) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetDeviceMask.html">vkCmdSetDeviceMask</a>
   */
  void setDeviceMask(uint32_t deviceMask) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetEvent.html">vkCmdSetEvent</a>
   */
  void setEvent(vk::Event event, vk::PipelineStageFlags stageMask) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetLineWidth.html">vkCmdSetLineWidth</a>
   */
  void setLineWidth(float lineWidth) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetScissor.html">vkCmdSetScissor</a>
   */
  void setScissor(uint32_t firstScissor, vk::ArrayProxy<const vk::Rect2D> scissors) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetStencilCompareMask.html">vkCmdSetStencilCompareMask</a>
   */
  void setStencilCompareMask(vk::StencilFaceFlags faceMask, uint32_t compareMask) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetStencilReference.html">vkCmdSetStencilReference</a>
   */
  void setStencilReference(vk::StencilFaceFlags faceMask, uint32_t reference) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetStencilWriteMask.html">vkCmdSetStencilWriteMask</a>
   */
  void setStencilWriteMask(vk::StencilFaceFlags faceMask, uint32_t writeMask) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetViewport.html">vkCmdSetViewport</a>
   */
  void setViewport(uint32_t firstViewport, vk::ArrayProxy<const vk::Viewport> viewports) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdUpdateBuffer.html">vkCmdUpdateBuffer</a>
   */
  template <typename T>
  void updateBuffer(vk::Buffer dstBuffer, vk::DeviceSize dstOffset, vk::ArrayProxy<const T> data) const {
    object_->updateBuffer(dstBuffer, dstOffset, data);
  }

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdWaitEvents.html">vkCmdWaitEvents</a>
   */
  void waitEvents(vk::ArrayProxy<const vk::Event> events, const vk::PipelineStageFlags& srcStageMask,
                  const vk::PipelineStageFlags& dstStageMask, vk::ArrayProxy<const vk::MemoryBarrier> memoryBarriers,
                  vk::ArrayProxy<const vk::BufferMemoryBarrier> bufferMemoryBarriers,
                  vk::ArrayProxy<const vk::ImageMemoryBarrier> imageMemoryBarriers) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdWriteTimestamp.html">vkCmdWriteTimestamp</a>
   */
  void writeTimestamp(vk::PipelineStageFlagBits pipelineStage, vk::QueryPool queryPool, uint32_t query) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkEndCommandBuffer.html">vkEndCommandBuffer</a>
   */
  vk::ResultValueType<void>::type end() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBeginRenderPass2KHR.html">vkCmdBeginRenderPass2KHR</a>
   */
  void beginRenderPass2KHR(const vk::RenderPassBeginInfo& renderPassBegin,
                           const vk::SubpassBeginInfoKHR& subpassBeginInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDispatchBaseKHR.html">vkCmdDispatchBaseKHR</a>
   */
  void dispatchBaseKHR(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX,
                       uint32_t groupCountY, uint32_t groupCountZ) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawIndexedIndirectCountKHR.html">vkCmdDrawIndexedIndirectCountKHR</a>
   */
  void drawIndexedIndirectCountKHR(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                   vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawIndirectCountKHR.html">vkCmdDrawIndirectCountKHR</a>
   */
  void drawIndirectCountKHR(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                            vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdEndRenderPass2KHR.html">vkCmdEndRenderPass2KHR</a>
   */
  void endRenderPass2KHR(const vk::SubpassEndInfoKHR& subpassEndInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdNextSubpass2KHR.html">vkCmdNextSubpass2KHR</a>
   */
  void nextSubpass2KHR(const vk::SubpassBeginInfoKHR& subpassBeginInfo,
                       const vk::SubpassEndInfoKHR& subpassEndInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdPushDescriptorSetKHR.html">vkCmdPushDescriptorSetKHR</a>
   */
  void pushDescriptorSetKHR(vk::PipelineBindPoint pipelineBindPoint, vk::PipelineLayout layout, uint32_t set,
                            vk::ArrayProxy<const vk::WriteDescriptorSet> descriptorWrites) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdPushDescriptorSetWithTemplateKHR.html">vkCmdPushDescriptorSetWithTemplateKHR</a>
   */
  void pushDescriptorSetWithTemplateKHR(vk::DescriptorUpdateTemplate descriptorUpdateTemplate,
                                        vk::PipelineLayout layout, uint32_t set, const void* pData) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetDeviceMaskKHR.html">vkCmdSetDeviceMaskKHR</a>
   */
  void setDeviceMaskKHR(uint32_t deviceMask) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBeginConditionalRenderingEXT.html">vkCmdBeginConditionalRenderingEXT</a>
   */
  void beginConditionalRenderingEXT(const vk::ConditionalRenderingBeginInfoEXT& conditionalRenderingBegin) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBeginDebugUtilsLabelEXT.html">vkCmdBeginDebugUtilsLabelEXT</a>
   */
  void beginDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& labelInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBeginQueryIndexedEXT.html">vkCmdBeginQueryIndexedEXT</a>
   */
  void beginQueryIndexedEXT(vk::QueryPool queryPool, uint32_t query, const vk::QueryControlFlags& flags,
                            uint32_t index) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBeginTransformFeedbackEXT.html">vkCmdBeginTransformFeedbackEXT</a>
   */
  void beginTransformFeedbackEXT(uint32_t firstCounterBuffer, vk::ArrayProxy<const vk::Buffer> counterBuffers,
                                 vk::ArrayProxy<const vk::DeviceSize> counterBufferOffsets) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBindTransformFeedbackBuffersEXT.html">vkCmdBindTransformFeedbackBuffersEXT</a>
   */
  void bindTransformFeedbackBuffersEXT(uint32_t firstBinding, vk::ArrayProxy<const vk::Buffer> buffers,
                                       vk::ArrayProxy<const vk::DeviceSize> offsets,
                                       vk::ArrayProxy<const vk::DeviceSize> sizes) const;
  
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDebugMarkerBeginEXT.html">vkCmdDebugMarkerBeginEXT</a>
   */
  void debugMarkerBeginEXT(const vk::DebugMarkerMarkerInfoEXT& markerInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDebugMarkerEndEXT.html">vkCmdDebugMarkerEndEXT</a>
   */
  void debugMarkerEndEXT() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDebugMarkerInsertEXT.html">vkCmdDebugMarkerInsertEXT</a>
   */
  void debugMarkerInsertEXT(const vk::DebugMarkerMarkerInfoEXT& markerInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawIndirectByteCountEXT.html">vkCmdDrawIndirectByteCountEXT</a>
   */
  void drawIndirectByteCountEXT(uint32_t instanceCount, uint32_t firstInstance, vk::Buffer counterBuffer,
                                vk::DeviceSize counterBufferOffset, uint32_t counterOffset,
                                uint32_t vertexStride) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdEndConditionalRenderingEXT.html">vkCmdEndConditionalRenderingEXT</a>
   */
  void endConditionalRenderingEXT() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdEndDebugUtilsLabelEXT.html">vkCmdEndDebugUtilsLabelEXT</a>
   */
  void endDebugUtilsLabelEXT() const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdEndQueryIndexedEXT.html">vkCmdEndQueryIndexedEXT</a>
   */
  void endQueryIndexedEXT(vk::QueryPool queryPool, uint32_t query, uint32_t index) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdEndTransformFeedbackEXT.html">vkCmdEndTransformFeedbackEXT</a>
   */
  void endTransformFeedbackEXT(uint32_t firstCounterBuffer, vk::ArrayProxy<const vk::Buffer> counterBuffers,
                               vk::ArrayProxy<const vk::DeviceSize> counterBufferOffsets) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdInsertDebugUtilsLabelEXT.html">vkCmdInsertDebugUtilsLabelEXT</a>
   */
  void insertDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& labelInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetDiscardRectangleEXT.html">vkCmdSetDiscardRectangleEXT</a>
   */
  void setDiscardRectangleEXT(uint32_t firstDiscardRectangle, vk::ArrayProxy<const vk::Rect2D> discardRectangles) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetSampleLocationsEXT.html">vkCmdSetSampleLocationsEXT</a>
   */
  void setSampleLocationsEXT(const vk::SampleLocationsInfoEXT& sampleLocationsInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBindShadingRateImageNV.html">vkCmdBindShadingRateImageNV</a>
   */
  void bindShadingRateImageNV(vk::ImageView imageView, vk::ImageLayout imageLayout) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBuildAccelerationStructureNV.html">vkCmdBuildAccelerationStructureNV</a>
   */
  void buildAccelerationStructureNV(const vk::AccelerationStructureInfoNV& info, vk::Buffer instanceData,
                                    vk::DeviceSize instanceOffset, vk::Bool32 update, vk::AccelerationStructureNV dst,
                                    vk::AccelerationStructureNV src, vk::Buffer scratch,
                                    vk::DeviceSize scratchOffset) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdCopyAccelerationStructureNV.html">vkCmdCopyAccelerationStructureNV</a>
   */
  void copyAccelerationStructureNV(vk::AccelerationStructureNV dst, vk::AccelerationStructureNV src,
                                   vk::CopyAccelerationStructureModeNV mode) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawMeshTasksIndirectCountNV.html">vkCmdDrawMeshTasksIndirectCountNV</a>
   */
  void drawMeshTasksIndirectCountNV(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                    vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawMeshTasksIndirectNV.html">vkCmdDrawMeshTasksIndirectNV</a>
   */
  void drawMeshTasksIndirectNV(vk::Buffer buffer, vk::DeviceSize offset, uint32_t drawCount, uint32_t stride) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawMeshTasksNV.html">vkCmdDrawMeshTasksNV</a>
   */
  void drawMeshTasksNV(uint32_t taskCount, uint32_t firstTask) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetCheckpointNV.html">vkCmdSetCheckpointNV</a>
   */
  void setCheckpointNV(const void* pCheckpointMarker) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetCoarseSampleOrderNV.html">vkCmdSetCoarseSampleOrderNV</a>
   */
  void setCoarseSampleOrderNV(vk::CoarseSampleOrderTypeNV sampleOrderType,
                              vk::ArrayProxy<const vk::CoarseSampleOrderCustomNV> customSampleOrders) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetExclusiveScissorNV.html">vkCmdSetExclusiveScissorNV</a>
   */
  void setExclusiveScissorNV(uint32_t firstExclusiveScissor, vk::ArrayProxy<const vk::Rect2D> exclusiveScissors) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetViewportShadingRatePaletteNV.html">vkCmdSetViewportShadingRatePaletteNV</a>
   */
  void setViewportShadingRatePaletteNV(uint32_t firstViewport,
                                       vk::ArrayProxy<const vk::ShadingRatePaletteNV> shadingRatePalettes) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdSetViewportWScalingNV.html">vkCmdSetViewportWScalingNV</a>
   */
  void setViewportWScalingNV(uint32_t firstViewport,
                             vk::ArrayProxy<const vk::ViewportWScalingNV> viewportWScalings) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdTraceRaysNV.html">vkCmdTraceRaysNV</a>
   */
  void traceRaysNV(vk::Buffer raygenShaderBindingTableBuffer, vk::DeviceSize raygenShaderBindingOffset,
                   vk::Buffer missShaderBindingTableBuffer, vk::DeviceSize missShaderBindingOffset,
                   vk::DeviceSize missShaderBindingStride, vk::Buffer hitShaderBindingTableBuffer,
                   vk::DeviceSize hitShaderBindingOffset, vk::DeviceSize hitShaderBindingStride,
                   vk::Buffer callableShaderBindingTableBuffer, vk::DeviceSize callableShaderBindingOffset,
                   vk::DeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdWriteAccelerationStructuresPropertiesNV.html">vkCmdWriteAccelerationStructuresPropertiesNV</a>
   */
  void writeAccelerationStructuresPropertiesNV(vk::ArrayProxy<const vk::AccelerationStructureNV> accelerationStructures,
                                               vk::QueryType queryType, vk::QueryPool queryPool,
                                               uint32_t firstQuery) const;

  /**
   * @brief Not supported anymore!
   */
  void processCommandsNVX(const vk::CmdProcessCommandsInfoNVX& processCommandsInfo) const;

  /**
   * @brief Not supported anymore!
   */
  void reserveSpaceForCommandsNVX(const vk::CmdReserveSpaceForCommandsInfoNVX& reserveSpaceInfo) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawIndexedIndirectCountAMD.html">vkCmdDrawIndexedIndirectCountAMD</a>
   */
  void drawIndexedIndirectCountAMD(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                                   vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
 
  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdDrawIndirectCountAMD.html">vkCmdDrawIndirectCountAMD</a>
   */
  void drawIndirectCountAMD(vk::Buffer buffer, vk::DeviceSize offset, vk::Buffer countBuffer,
                            vk::DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdWriteBufferMarkerAMD.html">vkCmdWriteBufferMarkerAMD</a>
   */
  void writeBufferMarkerAMD(vk::PipelineStageFlagBits pipelineStage, vk::Buffer dstBuffer, vk::DeviceSize dstOffset,
                            uint32_t marker) const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  QueueFamily getQueueFamily() const;

  CommandPool getCommandPool() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::CommandBuffer&() const;

  // endregion
};

} // namespace logi

#endif // LOGI_COMMAND_COMMAND_BUFFER_HPP
