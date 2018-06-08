#include "commands/CommandBuffer.h"

namespace logi {

CommandBuffer::CommandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device, const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, bool resetable)
	: DependentDestroyableHandle(owner), data_(std::make_shared<CommandBufferData>(vk_device, vk_cmd_pool, vk_cmd_buffer, resetable)) { }


CommandBuffer::CommandBufferData::CommandBufferData(const vk::Device& vk_device, const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, bool resetable)
	: vk_device(vk_device), vk_cmd_pool(vk_cmd_pool), vk_cmd_buffer(vk_cmd_buffer), resetable(resetable) { }

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

CommandBuffer::~CommandBuffer() {
}


PrimaryCommandBuffer::PrimaryCommandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device, const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, const bool resetable)
	: CommandBuffer(owner, vk_device, vk_cmd_pool, vk_cmd_buffer, resetable) { }


SecondaryCommmandBuffer::SecondaryCommmandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device, const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, const bool resetable)
	: CommandBuffer(owner, vk_device, vk_cmd_pool, vk_cmd_buffer, resetable) { }

} /// !namespace vkr