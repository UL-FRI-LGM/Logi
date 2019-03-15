#include "logi/commands/CommandBuffer.h"
#include "logi/commands/CommandPool.h"

namespace logi {

CommandBuffer::CommandBuffer(const CommandPool& cmd_pool, const vk::CommandBuffer& vk_cmd_buffer)
  : DestroyableOwnedHandle(cmd_pool, true), vk_cmd_buffer_(vk_cmd_buffer) {}

void CommandBuffer::beginRenderPass(const RenderPassBeginInfo& begin_info, const vk::SubpassContents contents) const {
  checkForNullHandleInvocation("CommandBuffer", "beginRenderPass");

  vk_cmd_buffer_.beginRenderPass(begin_info.build(), contents);
}

void CommandBuffer::endRenderPass() const {
  checkForNullHandleInvocation("CommandBuffer", "endRenderPass");

  vk_cmd_buffer_.endRenderPass();
}

void CommandBuffer::bindPipeline(const Pipeline& pipeline) const {
  checkForNullHandleInvocation("CommandBuffer", "bindPipeline");

  vk_cmd_buffer_.bindPipeline((pipeline.layout().getPipelineType() == PipelineType::eGraphical)
                                ? vk::PipelineBindPoint::eGraphics
                                : vk::PipelineBindPoint::eCompute,
                              pipeline.getVkHandle());
}

void CommandBuffer::bindDescriptorSets(const vk::PipelineBindPoint bind_point, const PipelineLayout& pipeline_layout,
                                       const uint32_t first_set, const std::vector<DescriptorSet>& descriptor_sets,
                                       const std::vector<uint32_t>& dynamic_offsets) const {
  checkForNullHandleInvocation("CommandBuffer", "bindDescriptorSets");

  // Fetch descriptor set handles.
  std::vector<vk::DescriptorSet> vk_descriptor_sets;
  vk_descriptor_sets.reserve(descriptor_sets.size());
  for (const DescriptorSet& set : descriptor_sets) {
    vk_descriptor_sets.emplace_back(set.getVkHandle());
  }

  vk_cmd_buffer_.bindDescriptorSets(bind_point, pipeline_layout.getVkHandle(), first_set,
                                    static_cast<uint32_t>(vk_descriptor_sets.size()), vk_descriptor_sets.data(),
                                    static_cast<uint32_t>(dynamic_offsets.size()), dynamic_offsets.data());
}

void CommandBuffer::bindIndexBuffer(const Buffer& buffer, const uint64_t offset, const vk::IndexType index_type) const {
  checkForNullHandleInvocation("CommandBuffer", "bindIndexBuffer");
  vk_cmd_buffer_.bindIndexBuffer(buffer.getVkHandle(), offset, index_type);
}

void CommandBuffer::bindVertexBuffers(const uint32_t first_binding, const std::vector<Buffer>& buffers,
                                      const std::vector<uint64_t>& offsets) const {
  checkForNullHandleInvocation("CommandBuffer", "bindVertexBuffers");

  std::vector<vk::Buffer> vk_buffers;
  vk_buffers.reserve(buffers.size());

  for (const Buffer& buffer : buffers) {
    vk_buffers.emplace_back(buffer.getVkHandle());
  }

  vk_cmd_buffer_.bindVertexBuffers(first_binding, vk_buffers, offsets);
}

void CommandBuffer::clearColorAttachments(const std::vector<vk::ClearAttachment>& clear_attachments,
                                          const std::vector<vk::ClearRect>& rects) const {
  checkForNullHandleInvocation("CommandBuffer", "clearColorAttachments");
  vk_cmd_buffer_.clearAttachments(clear_attachments, rects);
}

void CommandBuffer::clearColorImage(const Image& image, const vk::ImageLayout layout, const vk::ClearColorValue& color,
                                    const std::vector<vk::ImageSubresourceRange>& ranges) const {
  checkForNullHandleInvocation("CommandBuffer", "clearColorImage");
  vk_cmd_buffer_.clearColorImage(image.getVkHandle(), layout, color, ranges);
}

void CommandBuffer::clearDepthStencilImage(const Image& image, const vk::ImageLayout layout,
                                           const vk::ClearDepthStencilValue& depth_stencil,
                                           const std::vector<vk::ImageSubresourceRange>& ranges) const {
  checkForNullHandleInvocation("CommandBuffer", "clearDepthStencilImage");
  vk_cmd_buffer_.clearDepthStencilImage(image.getVkHandle(), layout, depth_stencil, ranges);
}

void CommandBuffer::copyBuffer(const Buffer& src, const Buffer& dst, const std::vector<vk::BufferCopy>& regions) const {
  checkForNullHandleInvocation("CommandBuffer", "copyBuffer");
  vk_cmd_buffer_.copyBuffer(src.getVkHandle(), dst.getVkHandle(), regions);
}

void CommandBuffer::copyImage(const Image& src, const vk::ImageLayout src_layout, const Image& dst,
                              const vk::ImageLayout dst_layout, const std::vector<vk::ImageCopy>& regions) const {
  checkForNullHandleInvocation("CommandBuffer", "copyImage");
  vk_cmd_buffer_.copyImage(src.getVkHandle(), src_layout, dst.getVkHandle(), dst_layout, regions);
}

void CommandBuffer::copyBufferToImage(const Buffer& src_buffer, const Image& dst_image,
                                      const vk::ImageLayout dst_layout,
                                      const std::vector<vk::BufferImageCopy>& regions) const {
  checkForNullHandleInvocation("CommandBuffer", "copyBufferToImage");
  vk_cmd_buffer_.copyBufferToImage(src_buffer.getVkHandle(), dst_image.getVkHandle(), dst_layout, regions);
}

void CommandBuffer::copyImageToBuffer(const Image& src_image, const vk::ImageLayout src_layout,
                                      const Buffer& dst_buffer, const std::vector<vk::BufferImageCopy>& regions) const {
  checkForNullHandleInvocation("CommandBuffer", "copyImageToBuffer");
  vk_cmd_buffer_.copyImageToBuffer(src_image.getVkHandle(), src_layout, dst_buffer.getVkHandle(), regions);
}

void CommandBuffer::fillBuffer(const Buffer& buffer, const uint64_t offset, const uint64_t size,
                               const uint32_t data) const {
  checkForNullHandleInvocation("CommandBuffer", "fillBuffer");
  vk_cmd_buffer_.fillBuffer(buffer.getVkHandle(), offset, size, data);
}

void CommandBuffer::updateBuffer(const Buffer& buffer, const uint64_t offset, const uint64_t size,
                                 const void* data) const {
  checkForNullHandleInvocation("CommandBuffer", "updateBuffer");
  vk_cmd_buffer_.updateBuffer(buffer.getVkHandle(), offset, size, data);
}

void CommandBuffer::resolveImage(const Image& src_image, const vk::ImageLayout src_layout, const Image& dst_image,
                                 const vk::ImageLayout dst_layout, const std::vector<vk::ImageResolve>& regions) const {
  checkForNullHandleInvocation("CommandBuffer", "resolveImage");
  vk_cmd_buffer_.resolveImage(src_image.getVkHandle(), src_layout, dst_image.getVkHandle(), dst_layout, regions);
}

void CommandBuffer::blitImage(const Image& src_image, const vk::ImageLayout src_layout, const Image& dst_image,
                              const vk::ImageLayout dst_layout, std::vector<vk::ImageBlit> regions,
                              const vk::Filter filter) const {
  checkForNullHandleInvocation("CommandBuffer", "blitImage");
  vk_cmd_buffer_.blitImage(src_image.getVkHandle(), src_layout, dst_image.getVkHandle(), dst_layout, regions, filter);
}

void CommandBuffer::dispatch(const uint32_t x_count, const uint32_t y_count, const uint32_t z_count) const {
  checkForNullHandleInvocation("CommandBuffer", "dispatch");
  vk_cmd_buffer_.dispatch(x_count, y_count, z_count);
}

void CommandBuffer::dispatchBase(const uint32_t base_x, const uint32_t base_y, const uint32_t base_z,
                                 const uint32_t x_count, const uint32_t y_count, const uint32_t z_count) const {
  checkForNullHandleInvocation("CommandBuffer", "dispatchBase");
  vk_cmd_buffer_.dispatchBase(base_x, base_y, base_z, x_count, y_count, z_count);
}

void CommandBuffer::dispatchIndirect(const Buffer& buffer, const uint64_t offset) const {
  checkForNullHandleInvocation("CommandBuffer", "dispatchIndirect");
  vk_cmd_buffer_.dispatchIndirect(buffer.getVkHandle(), offset);
}

void CommandBuffer::draw(const uint32_t vertex_count, const uint32_t first_vertex, const uint32_t instance_count,
                         const uint32_t first_instance) const {
  checkForNullHandleInvocation("CommandBuffer", "draw");
  vk_cmd_buffer_.draw(vertex_count, instance_count, first_vertex, first_instance);
}

void CommandBuffer::drawIndirect(const Buffer& buffer, const uint64_t offset, const uint32_t draw_count,
                                 const uint32_t stride) const {
  checkForNullHandleInvocation("CommandBuffer", "drawIndirect");
  vk_cmd_buffer_.drawIndirect(buffer.getVkHandle(), offset, draw_count, stride);
}

void CommandBuffer::drawIndexed(const uint32_t index_count, const uint32_t instance_count, const uint32_t first_index,
                                const int32_t vertex_offset, const uint32_t first_instance) const {
  checkForNullHandleInvocation("CommandBuffer", "drawIndexed");
  vk_cmd_buffer_.drawIndexed(index_count, instance_count, first_index, vertex_offset, first_instance);
}

void CommandBuffer::drawIndexedIndirect(const Buffer& buffer, const uint64_t offset, const uint32_t draw_count,
                                        const uint32_t stride) const {
  checkForNullHandleInvocation("CommandBuffer", "drawIndexedIndirect");
  vk_cmd_buffer_.drawIndexedIndirect(buffer.getVkHandle(), offset, draw_count, stride);
}

void CommandBuffer::setBlendConstants(const std::array<float, 4>& blend_constants) const {
  checkForNullHandleInvocation("CommandBuffer", "setBlendConstants");
  vk_cmd_buffer_.setBlendConstants(blend_constants.data());
}

void CommandBuffer::setDepthBias(const float constant_factor, const float clamp, const float slope_factor) const {
  checkForNullHandleInvocation("CommandBuffer", "setDepthBias");
  vk_cmd_buffer_.setDepthBias(constant_factor, clamp, slope_factor);
}

void CommandBuffer::setDepthBounds(const float min_depth_bounds, const float max_depth_bounds) const {
  checkForNullHandleInvocation("CommandBuffer", "setDepthBounds");
  vk_cmd_buffer_.setDepthBounds(min_depth_bounds, max_depth_bounds);
}

void CommandBuffer::setLineWidth(const float width) const {
  checkForNullHandleInvocation("CommandBuffer", "setLineWidth");
  vk_cmd_buffer_.setLineWidth(width);
}

void CommandBuffer::setScissor(const uint32_t first_scissor, const std::vector<vk::Rect2D>& scissors) const {
  checkForNullHandleInvocation("CommandBuffer", "setScissor");
  vk_cmd_buffer_.setScissor(first_scissor, scissors);
}

void CommandBuffer::setViewports(uint32_t first_viewport, const std::vector<vk::Viewport>& viewports) const {
  checkForNullHandleInvocation("CommandBuffer", "setViewports");
  vk_cmd_buffer_.setViewport(first_viewport, viewports);
}

void CommandBuffer::setStencilCompareMask(const vk::StencilFaceFlags& face_mask, const uint32_t compare_mask) const {
  checkForNullHandleInvocation("CommandBuffer", "setStencilCompareMask");
  vk_cmd_buffer_.setStencilCompareMask(face_mask, compare_mask);
}

void CommandBuffer::setStencilReference(const vk::StencilFaceFlags& face_mask, const uint32_t reference) const {
  checkForNullHandleInvocation("CommandBuffer", "setStencilReference");
  vk_cmd_buffer_.setStencilReference(face_mask, reference);
}

void CommandBuffer::setStencilWriteMask(const vk::StencilFaceFlags& face_mask, const uint32_t write_mask) const {
  checkForNullHandleInvocation("CommandBuffer", "setStencilWriteMask");
  vk_cmd_buffer_.setStencilWriteMask(face_mask, write_mask);
}

void CommandBuffer::setDeviceMask(const uint32_t device_mask) const {
  checkForNullHandleInvocation("CommandBuffer", "setDeviceMask");
  vk_cmd_buffer_.setDeviceMask(device_mask);
}

void CommandBuffer::signalEvent(const Event& event, const vk::PipelineStageFlags& stage_mask) const {
  checkForNullHandleInvocation("CommandBuffer", "signalEvent");
  vk_cmd_buffer_.setEvent(event.getVkHandle(), stage_mask);
}

void CommandBuffer::resetEvent(const Event& event, const vk::PipelineStageFlags& stage_mask) const {
  checkForNullHandleInvocation("CommandBuffer", "resetEvent");
  vk_cmd_buffer_.resetEvent(event.getVkHandle(), stage_mask);
}

void CommandBuffer::end() const {
  checkForNullHandleInvocation("CommandBuffer", "end");
  vk_cmd_buffer_.end();
}

const vk::CommandBuffer& CommandBuffer::getVkHandle() const {
  checkForNullHandleInvocation("CommandBuffer", "getVkHandle");
  return vk_cmd_buffer_;
}

bool CommandBuffer::isResetable() const {
  return static_cast<bool>(getOwner().flags() & vk::CommandPoolCreateFlagBits::eResetCommandBuffer));
}

CommandBuffer::operator vk::CommandBuffer() const {
  checkForNullHandleInvocation("CommandBuffer", "operator vk::CommandBuffer()");
  return vk_cmd_buffer_;
}

void CommandBuffer::free() {
  if (valid()) {
    getOwner().freeCommandBuffer(*this);
    DestroyableOwnedHandle<CommandPool>::destroy();
  }
}

CommandBuffer::~CommandBuffer() = default;

} // namespace logi
