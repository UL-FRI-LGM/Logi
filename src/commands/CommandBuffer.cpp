#include "commands/CommandBuffer.h"

namespace vkr {

CommandBuffer::CommandBuffer(const vk::CommandBuffer& command_buffer, bool resetable) : command_buffer_(command_buffer), resetable_(resetable) {

}

CommandBuffer::~CommandBuffer() {
}

const vk::CommandBuffer & CommandBuffer::getVkHandle() const {
	return command_buffer_;
}

void CommandBuffer::bindPipeline(const vk::Pipeline& pipeline, const vk::PipelineBindPoint& bind_point) {
	command_buffer_.bindPipeline(bind_point, pipeline);
}

void CommandBuffer::bindDescriptorSets(const vk::PipelineLayout& layout, const vk::PipelineBindPoint& bind_point, uint32_t first_set, const std::vector<vk::DescriptorSet>& descriptor_sets, const std::vector<uint32_t>& dynamic_offsets) {
	command_buffer_.bindDescriptorSets(bind_point, layout, first_set, descriptor_sets, dynamic_offsets);
}

void CommandBuffer::computeDispatch(uint32_t group_count_x, uint32_t group_count_y, uint32_t group_count_z) {
	command_buffer_.dispatch(group_count_x, group_count_y, group_count_z);
}

void CommandBuffer::endCommandBuffer() {
	command_buffer_.end();
}

bool CommandBuffer::isResetable() {
	return resetable_;
}


PrimaryCommandBuffer::PrimaryCommandBuffer(const vk::CommandBuffer& command_buffer, bool resetable) : CommandBuffer(command_buffer, resetable) {
}

void PrimaryCommandBuffer::beginCommandBuffer(vk::CommandBufferUsageFlags usage_flags) {
	vk::CommandBufferBeginInfo begin_info{};
	begin_info.flags = usage_flags;

	return command_buffer_.begin(begin_info);
}

SecondaryCommmandBuffer::SecondaryCommmandBuffer(const vk::CommandBuffer& command_buffer, bool resetable) : CommandBuffer(command_buffer, resetable) {
}

} /// !namespace vkr