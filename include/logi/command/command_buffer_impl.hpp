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

#ifndef LOGI_COMMAND_COMMAND_BUFFER_IMPL_HPP
#define LOGI_COMMAND_COMMAND_BUFFER_IMPL_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;
class CommandPoolImpl;

class CommandBufferImpl : public VulkanObject<CommandBufferImpl> {
 public:
  CommandBufferImpl(CommandPoolImpl& commandPool, const vk::CommandBuffer& vkCommandBuffer);

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

  void pipelineBarrier(vk::PipelineStageFlags srcStageMask, vk::PipelineStageFlags dstStageMask,
                       vk::DependencyFlags dependencyFlags, vk::ArrayProxy<const vk::MemoryBarrier> memoryBarriers,
                       vk::ArrayProxy<const vk::BufferMemoryBarrier> bufferMemoryBarriers,
                       vk::ArrayProxy<const vk::ImageMemoryBarrier> imageMemoryBarriers) const;

  template <typename T>
  void pushConstants(vk::PipelineLayout layout, vk::ShaderStageFlags stageFlags, uint32_t offset,
                     vk::ArrayProxy<const T> values) const {
    vkCommandBuffer_.pushConstants(layout, stageFlags, offset, values, getDispatcher());
  }

  void resetEvent(vk::Event event, vk::PipelineStageFlags stageMask) const;

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
    vkCommandBuffer_.updateBuffer(dstBuffer, dstOffset, data, getDispatcher());
  }

  void waitEvents(vk::ArrayProxy<const vk::Event> events, const vk::PipelineStageFlags& srcStageMask,
                  const vk::PipelineStageFlags& dstStageMask, vk::ArrayProxy<const vk::MemoryBarrier> memoryBarriers,
                  vk::ArrayProxy<const vk::BufferMemoryBarrier> bufferMemoryBarriers,
                  vk::ArrayProxy<const vk::ImageMemoryBarrier> imageMemoryBarriers) const;

  void writeTimestamp(vk::PipelineStageFlagBits pipelineStage, vk::QueryPool queryPool, uint32_t query) const;

  vk::ResultValueType<void>::type end() const;

  // endregion

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  CommandPoolImpl& getCommandPool() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::CommandBuffer() const;

 protected:
  void free() override;

  // endregion

 private:
  CommandPoolImpl& commandPool_;
  vk::CommandBuffer vkCommandBuffer_;
};

} // namespace logi

#endif // LOGI_COMMAND_COMMAND_BUFFER_IMPL_HPP
