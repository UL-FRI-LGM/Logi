#include "commands/SecondaryCommandBuffer.h"

namespace logi {

SecondaryCommmandBuffer::SecondaryCommmandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device,
	const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, const bool resetable)
	: CommandBuffer(owner, vk_device, vk_cmd_pool, vk_cmd_buffer, resetable) {
}

}