#include "commands/CommandBuffer.h"

namespace logi {

CommandBuffer::CommandBuffer()
	: DependentDestroyableHandle({}, false) {
}

CommandBuffer::CommandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device,
                             const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, bool resetable)
	: DependentDestroyableHandle(owner), data_(std::make_shared<CommandBufferData>(vk_device, vk_cmd_pool,
	                                                                               vk_cmd_buffer, resetable)) {
}

CommandBuffer::CommandBufferData::CommandBufferData(const vk::Device& vk_device, const vk::CommandPool& vk_cmd_pool,
                                                    const vk::CommandBuffer& vk_cmd_buffer, const bool resetable)
	: vk_device(vk_device), vk_cmd_pool(vk_cmd_pool), vk_cmd_buffer(vk_cmd_buffer), resetable(resetable) {
}

const vk::CommandBuffer& CommandBuffer::getVkHandle() const {
	if (!alive()) {
		throw std::runtime_error("Called getVkHandle on destroyed CommandBuffer.");
	}

	return data_->vk_cmd_buffer;
}

bool CommandBuffer::isResetable() const {
	return data_->resetable;
}

void CommandBuffer::destroy() const {
	if (alive()) {
		// Free the command buffer.
		data_->vk_device.freeCommandBuffers(data_->vk_cmd_pool, 1u, &data_->vk_cmd_buffer);
		DependentDestroyableHandle::destroy();
	}
}

CommandBuffer::~CommandBuffer() = default;

PrimaryCommandBuffer::PrimaryCommandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device,
                                           const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer,
                                           const bool resetable)
	: CommandBuffer(owner, vk_device, vk_cmd_pool, vk_cmd_buffer, resetable) {
}

void PrimaryCommandBuffer::begin(const vk::CommandBufferUsageFlags& usage_flags) const {
	vk::CommandBufferBeginInfo begin_info;
	begin_info.flags = usage_flags;

	data_->vk_cmd_buffer.begin(begin_info);
}

void PrimaryCommandBuffer::end() const {
	data_->vk_cmd_buffer.end();
}

void PrimaryCommandBuffer::endRenderPass() const {
	data_->vk_cmd_buffer.endRenderPass();
}

void PrimaryCommandBuffer::bindPipeline(const GraphicalPipeline& pipeline) const {
	data_->vk_cmd_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.getVkHandle());
}

void PrimaryCommandBuffer::draw(const uint32_t vertex_count, const uint32_t first_vertex, const uint32_t instance_count,
                                const uint32_t first_instance) const {
	data_->vk_cmd_buffer.draw(vertex_count, instance_count, first_vertex, first_instance);
}

void PrimaryCommandBuffer::bindPipeline(const ComputePipeline& pipeline) const {
	data_->vk_cmd_buffer.bindPipeline(vk::PipelineBindPoint::eCompute, pipeline.getVkHandle());
}

void PrimaryCommandBuffer::dispatch(const uint32_t group_count_x, const uint32_t group_count_y,
                                    const uint32_t group_count_z) const {
	data_->vk_cmd_buffer.dispatch(group_count_x, group_count_y, group_count_z);
}

void PrimaryCommandBuffer::bindDescriptorSets(const vk::PipelineBindPoint pipeline_bind_point,
                                              const PipelineLayout& layout, const uint32_t first_set,
                                              const std::vector<DescriptorSet>& descriptor_sets) const {

	// Fet Vulkan descriptor set handles.
	std::vector<vk::DescriptorSet> vk_descriptor_sets;
	vk_descriptor_sets.reserve(vk_descriptor_sets.size());

	for (const DescriptorSet& desc_set : descriptor_sets) {
		vk_descriptor_sets.emplace_back(desc_set.getVkHandle());
	}

	data_->vk_cmd_buffer.bindDescriptorSets(pipeline_bind_point, layout.getVkHandle(), first_set, vk_descriptor_sets, {});

}


SecondaryCommmandBuffer::SecondaryCommmandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device,
                                                 const vk::CommandPool& vk_cmd_pool, 
												 const vk::CommandBuffer& vk_cmd_buffer, const bool resetable)
	: CommandBuffer(owner, vk_device, vk_cmd_pool, vk_cmd_buffer, resetable) {
}

} /// !namespace vkr
