#include "commands/PrimaryCommandBuffer.h"

namespace logi {

CommandBufferBeginInfo::CommandBufferBeginInfo(const vk::CommandBufferUsageFlags& flags) 
    : flags(flags) {}

vk::CommandBufferBeginInfo CommandBufferBeginInfo::build() const {
	vk::CommandBufferBeginInfo begin_info(flags, nullptr);
	begin_info.pNext = buildExtensions();

	return begin_info;
}

RenderPassBeginInfo::RenderPassBeginInfo(RenderPass render_pass, Framebuffer framebuffer, vk::Rect2D render_area, 
	                                     std::vector<vk::ClearValue> clear_values)
    : render_pass(std::move(render_pass)), framebuffer(std::move(framebuffer)), render_area(std::move(render_area)),
	  clear_values(std::move(clear_values)) {}

vk::RenderPassBeginInfo RenderPassBeginInfo::build() const {
	vk::RenderPassBeginInfo begin_info(render_pass.getVkHandle(), framebuffer.getVkHandle(), render_area, clear_values.size(), clear_values.data());
	begin_info.pNext = buildExtensions();

	return begin_info;
}


PrimaryCommandBuffer::PrimaryCommandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device,
	const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer,
	const bool resetable)
	: CommandBuffer(owner, vk_device, vk_cmd_pool, vk_cmd_buffer, resetable) {
}

void PrimaryCommandBuffer::begin(const CommandBufferBeginInfo& begin_info) const {
	checkForNullHandleInvocation("PrimaryCommandBuffer", "begin");

	data_->vk_cmd_buffer.begin(begin_info.build());
}

void PrimaryCommandBuffer::executeCommands(std::vector<SecondaryCommmandBuffer>& cmd_buffers) const {
	checkForNullHandleInvocation("CommandBuffer", "executeCommands");

	std::vector<vk::CommandBuffer> vk_cmd_buffers;
	vk_cmd_buffers.reserve(cmd_buffers.size());

	for (const auto& cmd_buffer : cmd_buffers) {
		vk_cmd_buffers.emplace_back(cmd_buffer.getVkHandle());
	}

	data_->vk_cmd_buffer.executeCommands(vk_cmd_buffers);
}

void PrimaryCommandBuffer::nextSubpass(vk::SubpassContents contents) const {
	checkForNullHandleInvocation("CommandBuffer", "nextSubpass");

	data_->vk_cmd_buffer.nextSubpass(contents);
}


}
