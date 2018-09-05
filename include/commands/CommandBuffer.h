#ifndef COMMANDS_COMMAND_BUFFER_H
#define COMMANDS_COMMAND_BUFFER_H

#include <vulkan/vulkan.hpp>
#include "base/Handle.h"
#include "base/ExtensionObject.h"
#include "program/render_pass/RenderPass.h"
#include "descriptors/DescriptorSet.h"
#include "memory/Buffer.h"
#include "memory/Image.h"

namespace logi {

/**
 * @brief   Aggregates data required to build Vulkan RenderPassBeginInfo.
 */
struct RenderPassBeginInfo : public ExtendableObject {

	/**
	 * @brief	Initializes RenderPassBeginInfo data.
	 *
	 * @param	render_pass     Render pass handle.
	 * @param	framebuffer     Framebuffer handle.
	 * @param	render_area     Render area that is affected by the render pass instance.
	 * @param	clear_values    Contains clear values for each attachment.
	 */
	RenderPassBeginInfo(RenderPass render_pass, Framebuffer framebuffer,
		vk::Rect2D render_area, std::vector<vk::ClearValue> clear_values);

	/**
	 * @brief	Builds Vulkan RenderPassBeginInfo.
	 *
	 * @return	Vulkan RenderPassBeginInfo.
	 */
	vk::RenderPassBeginInfo build() const;

	/**
	 * Render pass handle.
	 */
	RenderPass render_pass;

	/**
	 * Framebuffer handle.
	 */
	Framebuffer framebuffer;

	/**
	 * Render area that is affected by the render pass instance.
	 */
	vk::Rect2D render_area;

	/**
	 * Contains clear values for each attachment.
	 */
	std::vector<vk::ClearValue> clear_values;
};


class CommandBuffer : public DependentDestroyableHandle {
public:
	CommandBuffer();

	CommandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device,
	              const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, bool resetable);

    /**
	 * @brief	Begin a new render pass
	 *
	 * @param	begin_info  RenderPassBeginInfo that contains information used to begin render pass.     
	 * @param	contents    Specifies how the commands in the first subpass will be provided.
	 */
	void beginRenderPass(const RenderPassBeginInfo& begin_info,
	                     vk::SubpassContents contents = vk::SubpassContents::eInline) const;

    /**
     * @brief   Ends current render pass.
     */
	void endRenderPass() const;

    /**
	 * @brief	Bind a pipeline to the command buffer.
	 *
	 * @param	pipeline    Pipeline to be bound.
	 */
	void bindPipeline(const Pipeline& pipeline) const;

    /**
     * @brief   Binds descriptor sets to the command buffer.
     *
     * @param   bind_point          Indicates whether the descriptors will be used by graphics pipelines or compute 
     *                              pipelines.
     * @param   pipeline_layout     Pipeline layout object used to program the bindings.
     * @param   first_set           Set number of the first descriptor set to be bound.
     * @param   descriptor_sets     Array of descriptor set handles.
     * @param   dynamic_offsets     Array of uint32_t values specifying dynamic offsets.
     */
	void bindDescriptorSets(vk::PipelineBindPoint bind_point, const PipelineLayout& pipeline_layout, uint32_t first_set, 
		                    const std::vector<DescriptorSet>& descriptor_sets,
		                    const std::vector<uint32_t>& dynamic_offsets = {}) const;

    /**
	 * @brief	Bind an index buffer to the command buffer.
	 *
	 * @param	buffer      Buffer that will be bound.
	 * @param	offset      Starting offset in bytes within buffer used in index buffer address calculations
	 * @param	index_type  Value specifying whether indices are treated as 16 bits or 32 bits.
	 */
	void bindIndexBuffer(const Buffer& buffer, uint64_t offset, vk::IndexType index_type) const;

    /**
	 * @brief	Bind vertex buffers to the command buffer
	 *
	 * @param	first_binding   Index of the first vertex input binding whose state is updated by the command.
	 * @param	buffers         Array of buffer handles.
	 * @param	offsets         Array of buffer offsets.
	 */
	void bindVertexBuffers(uint32_t first_binding, const std::vector<Buffer>& buffers,
		                   const std::vector<uint64_t>& offsets) const;

    /**
	 * @brief	Clear regions within bound framebuffer attachments.
	 *
	 * @param	clear_attachments   Array of ClearAttachment structures defining the attachments to clear and the clear 
	 *                              values to use.
	 * @param	rects               Array of ClearRect structures defining regions within each selected attachment to
	 *                              clear.
	 */
	void clearColorAttachments(const std::vector<vk::ClearAttachment>& clear_attachments,
		                       const std::vector<vk::ClearRect>& rects) const;

    /**
	 * @brief	Clear regions of a color image.
	 *
	 * @param	image   Image to be cleared.
	 * @param	layout  Specifies the current layout of the image subresource ranges to be cleared.
	 * @param	color   Structure that contains the values the image subresource ranges will be cleared to.
	 * @param	ranges  Array of ImageSubresourceRange structures that describe a range of mipmap levels, array layers, 
	 *                  and aspects to be cleared
	 */
	void clearColorImage(const Image& image, vk::ImageLayout layout, const vk::ClearColorValue& color,
		                 const std::vector<vk::ImageSubresourceRange>& ranges) const;

    /**
	 * @brief	Fill regions of a combined depth/stencil image
	 *
	 * @param	image           Image to be cleared.
	 * @param	layout          Specifies the current layout of the image subresource ranges to be cleared.
	 * @param	depth_stencil   Structure that contains the values the depth and stencil image subresource ranges will
	 *                          be cleared to 
	 * @param	ranges          Structures that describe a range of mipmap levels, array layers, and aspects to be
	 *                          cleared.
	 */
	void clearDepthStencilImage(const Image& image, vk::ImageLayout layout,
		                        const vk::ClearDepthStencilValue& depth_stencil,
		                        const std::vector<vk::ImageSubresourceRange>& ranges) const;

    /**
	 * @brief	Copy data between buffer regions.
	 *
	 * @param	src         Source buffer.
	 * @param	dst         Destination buffer.
	 * @param	regions     Array of BufferCopy structures specifying the regions to copy.
	 */
	void copyBuffer(const Buffer& src, const Buffer& dst, const std::vector<vk::BufferCopy>& regions) const;

    /**
	 * @brief	Copy data between images
	 *
	 * @param	src         Source image.
	 * @param	src_layout  Current layout of the source image subresource.
	 * @param	dst         Destination image.
	 * @param	dst_layout  Current layout of the destination image subresource.
	 * @param	regions     Array of ImageCopy structures specifying the regions to copy.
	 */
	void copyImage(const Image& src, vk::ImageLayout src_layout, const Image& dst, vk::ImageLayout dst_layout,
		           const std::vector<vk::ImageCopy>& regions) const;

    /**
	 * @brief	Copy data from a buffer into an image
	 *
	 * @param	src_buffer  Source buffer.
	 * @param	dst_image   Destination image.
	 * @param	dst_layout  Layout of the destination image subresources for the copy.
	 * @param	regions     Array of BufferImageCopy structures specifying the regions to copy.
	 */
	void copyBufferToImage(const Buffer& src_buffer, const Image& dst_image, vk::ImageLayout dst_layout,
		                   const std::vector<vk::BufferImageCopy>& regions) const;

    /**
	 * @brief	Copy image data into a buffer.
	 *
	 * @param	src_image   Source image.
	 * @param	src_layout  Layout of the source image subresources for the copy.
	 * @param	dst_buffer  Destination buffer.
	 * @param	regions     Array of BufferImageCopy structures specifying the regions to copy.
	 */
	void copyImageToBuffer(const Image& src_image, vk::ImageLayout src_layout, const Buffer& dst_buffer,
		                   const std::vector<vk::BufferImageCopy>& regions) const;

    /**
	 * @brief	Fill a region of a buffer with a fixed value
	 *
	 * @param	buffer  Buffer to be filled.
	 * @param	offset  Byte offset into the buffer at which to start filling, and must be a multiple of 4.
	 * @param	size    Number of bytes to fill, and must be either a multiple of 4, or VK_WHOLE_SIZE to fill the range
	 *                  from offset to the end of the buffer.
	 * @param	data    4-byte word written repeatedly to the buffer to fill size bytes of data.
	 */
	void fillBuffer(const Buffer& buffer, uint64_t offset, uint64_t size, uint32_t data) const;

    /**
	 * @brief	Update a buffer's contents from host memory.
	 *
	 * @param	buffer  Handle to the buffer to be updated.
	 * @param	offset  Byte offset into the buffer to start updating, and must be a multiple of 4.
	 * @param	size    Number of bytes to update, and must be a multiple of 4.
	 * @param	data    Pointer to the source data for the buffer update, and must be at least dataSize bytes in size.
	 */
	void updateBuffer(const Buffer& buffer, uint64_t offset, uint64_t size, const void* data) const;

    /**
     * @brief	
     *
     * @tparam	T       Type of the vector elements.
     * @param	buffer  Handle to the buffer to be updated.
     * @param	offset  Byte offset into the buffer to start updating, and must be a multiple of 4.
     * @param	data    Vector containing the data that will be used to update the buffer.
     */
    template <typename T>
    void updateBuffer(const Buffer& buffer, uint64_t offset, const std::vector<T>& data) const;

    /**
	 * @brief	Resolve regions of an image
	 *
	 * @param	src_image   Source image.
	 * @param	src_layout  Layout of the source image subresources for the resolve.
	 * @param	dst_image   Destination image.
	 * @param	dst_layout  Layout of the destination image subresources for the resolve.
	 * @param	regions     Array of ImageResolve structures specifying the regions to resolve.
	 */
	void resolveImage(const Image& src_image, vk::ImageLayout src_layout, const Image& dst_image, 
		              vk::ImageLayout dst_layout, const std::vector<vk::ImageResolve>& regions) const;

    /**
	 * @brief	 Copy regions of an image, potentially performing format conversion.
	 *
	 * @param	src_image   Source image.
	 * @param	src_layout  Layout of the source image subresources for the blit.
	 * @param	dst_image   Destination image.
	 * @param	dst_layout  Layout of the destination image subresources for the blit.
	 * @param	regions     Array of ImageBlit structures specifying the regions to blit.
	 * @param	filter      Filter to apply if the blits require scaling.
	 */
	void blitImage(const Image& src_image, vk::ImageLayout src_layout, const Image& dst_image, vk::ImageLayout dst_layout,
		           std::vector<vk::ImageBlit> regions, vk::Filter filter) const;

    /**
	 * @brief	Dispatch compute work items
	 *
	 * @param	x_count Number of local workgroups to dispatch in the X dimension.
	 * @param	y_count Number of local workgroups to dispatch in the Y dimension.
	 * @param	z_count Number of local workgroups to dispatch in the Z dimension.
	 */
	void dispatch(uint32_t x_count, uint32_t y_count, uint32_t z_count) const;

    /**
	 * @brief	Dispatch compute work items
	 *
	 * @param	base_x  Start value for the X component of WorkgroupId.
	 * @param	base_y  Start value for the Y component of WorkgroupId.
	 * @param	base_z  Start value for the Z component of WorkgroupId.
	 * @param	x_count Number of local workgroups to dispatch in the X dimension.
	 * @param	y_count Number of local workgroups to dispatch in the Y dimension.
	 * @param	z_count Number of local workgroups to dispatch in the Z dimension.
	 */
	void dispatchBase(uint32_t base_x, uint32_t base_y, uint32_t base_z, uint32_t x_count, uint32_t y_count,
		              uint32_t z_count) const;

    /**
	 * @brief	Dispatch compute work items using indirect parameters
	 *
	 * @param	buffer  Buffer containing dispatch parameters.
	 * @param	offset  Byte offset into buffer where parameters begin.
	 */
	void dispatchIndirect(const Buffer& buffer, uint64_t offset) const;

    /**
	 * @brief	Draw primitives.
	 *
	 * @param	vertex_count    Number of vertices to draw.
	 * @param	first_vertex    Number of instances to draw.
	 * @param	instance_count  Index of the first vertex to draw.
	 * @param	first_instance  Instance ID of the first instance to draw.
	 */
	void draw(uint32_t vertex_count, uint32_t first_vertex = 0, uint32_t instance_count = 1,
		      uint32_t first_instance = 0) const;

    /**
	 * @brief   Issue an indirect draw into a command buffer
	 *
	 * @param	buffer      Buffer containing draw parameters.
	 * @param	offset      Byte offset into buffer where parameters begin.
	 * @param	draw_count  Number of draws to execute, and can be zero.
	 * @param	stride      Byte stride between successive sets of draw parameters.
	 */
	void drawIndirect(const Buffer& buffer, uint64_t offset, uint32_t draw_count, uint32_t stride) const;

    /**
	 * @brief	Issue an indexed draw into a command buffer
	 *
	 * @param	index_count     Number of vertices to draw.
	 * @param	instance_count  Number of instances to draw.
	 * @param	first_index     Base index within the index buffer.
	 * @param	vertex_offset   Value added to the vertex index before indexing into the vertex buffer.
	 * @param	first_instance  Instance ID of the first instance to draw.
	 */
	void drawIndexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t vertex_offset,
		             uint32_t first_instance) const;

    /**
	 * @brief	Perform an indexed indirect draw
	 *
	 * @param	buffer      Buffer containing draw parameters.
	 * @param	offset      Byte offset into buffer where parameters begin.
	 * @param	draw_count  Number of draws to execute, and can be zero.
	 * @param	stride      Byte stride between successive sets of draw parameters.
	 */
	void drawIndexedIndirect(const Buffer& buffer, uint64_t offset, uint32_t draw_count, uint32_t stride) const;

    /**
	 * @brief	Set the values of dynamic blend constants.
	 *
	 * @param	blend_constants Array of four values specifying the R, G, B, and A components of the blend constant
	 *                          color used in blending
	 */
	void setBlendConstants(const std::array<float, 4>& blend_constants) const;

    /**
	 * @brief	Set the depth bias dynamic state.
	 *
	 * @param	constant_factor Scalar factor controlling the constant depth value added to each fragment.
	 * @param	clamp           Maximum (or minimum) depth bias of a fragment.
	 * @param	slope_factor    Scalar factor applied to a fragment’s slope in depth bias calculations.
	 */
	void setDepthBias(float constant_factor, float clamp, float slope_factor) const;

    /**
	 * @brief	Set the dynamic depth bounds test values for a command buffer
	 *
	 * @param	min_depth_bounds    Lower bound of the range of depth values used in the depth bounds test.
	 * @param	max_depth_bounds    Upper bound of the range.
	 */
	void setDepthBounds(float min_depth_bounds, float max_depth_bounds) const;

    /**
	 * @brief	Set the dynamic line width state
	 *
	 * @param	width   Width of rasterized line segments.
	 */
	void setLineWidth(float width) const;

    /**
	 * @brief	Set the dynamic scissor rectangles on a command buffer
	 *
	 * @param	first_scissor   Index of the first scissor whose state is updated by the command.
	 * @param	scissors        Array of VkRect2D structures defining scissor rectangles.
	 */
	void setScissor(uint32_t first_scissor, const std::vector<vk::Rect2D>& scissors) const;

    /**
	 * @brief	Set the dynamic viewport on a command buffer
	 *
	 * @param	first_viewport  Index of the first viewport whose parameters are updated by the command.
	 * @param	viewports       Array of Viewport structures specifying viewport parameters.
	 */
	void setViewports(uint32_t first_viewport, const std::vector<vk::Viewport>& viewports) const;

    /**
	 * @brief	Set the stencil compare mask dynamic state
	 *
	 * @param	face_mask       Specifies the set of stencil state for which to update the compare mask.
	 * @param	compare_mask    New value to use as the stencil compare mask.
	 */
	void setStencilCompareMask(const vk::StencilFaceFlags& face_mask, uint32_t compare_mask) const;

    /**
	 * @brief	Set the stencil reference dynamic state
	 *
	 * @param	face_mask   Specifies the set of stencil state for which to update the reference value.
	 * @param	reference   New value to use as the stencil reference value.
	 */
	void setStencilReference(const vk::StencilFaceFlags& face_mask, uint32_t reference) const;

    /**
     * @brief	Set the stencil write mask dynamic state
     *
     * @param	face_mask   Specifies the set of stencil state for which to update the write mask.
     * @param	write_mask  New value to use as the stencil write mask.
     */
    void setStencilWriteMask(const vk::StencilFaceFlags& face_mask, uint32_t write_mask) const;

    /**
	 * @brief	Modify device mask of a command buffer
	 *
	 * @param	device_mask New value of the current device mask.
	 */
	void setDeviceMask(uint32_t device_mask) const;

	/**
	 * @brief   Ends command buffer recording.
	 */
	void end() const;

	const vk::CommandBuffer& getVkHandle() const;

	bool isResetable() const;

	void destroy() const override;

	virtual ~CommandBuffer() = 0;

protected:
	/**
	 * @brief	Structure used to hold CommandBuffer data.
	 */
	struct CommandBufferData {
		/**
		 * @brief	Populates CommandBufferData.
		 *
		 * @param	vk_device		Vulkan Device handle.
		 * @param	vk_cmd_pool		Vulkan CommandPool handle.
		 * @param	vk_cmd_buffer	Vulkan CommandBuffer handle.
		 * @param	resetable			If true CommandBuffer may be reset.
		 */
		CommandBufferData(const vk::Device& vk_device, const vk::CommandPool& vk_cmd_pool,
		                  const vk::CommandBuffer& vk_cmd_buffer, bool resetable);

		vk::Device vk_device;            ///< Vulkan Device handle.
		vk::CommandPool vk_cmd_pool;     ///< Vulkan CommandPool handle.
		vk::CommandBuffer vk_cmd_buffer; ///< Vulkan CommandBuffer handle.
		bool resetable;                  ///< If true CommandBuffer may be reset.
	};

	std::shared_ptr<CommandBufferData> data_;
};

template <typename T>
void CommandBuffer::updateBuffer(const Buffer& buffer, const uint64_t offset, const std::vector<T>& data) const {
    checkForNullHandleInvocation("CommandBuffer", "updateBuffer");

    if ((data.size() * sizeof(T)) % 4 != 0) {
        throw IllegalInvocation("Tried to update buffer with data that was not multiple of 4.");
    }

    data_->vk_cmd_buffer.updateBuffer(buffer.getVkHandle(), offset, data.size() * sizeof(T), data.data());
}

}

#endif // !COMMANDS_COMMAND_BUFFER_H
