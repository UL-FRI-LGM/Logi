#ifndef COMMANDS_PRIMARY_SECONDARY_BUFFER_H
#define COMMANDS_PRIMARY_SECONDARY_BUFFER_H

#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/commands/CommandBuffer.h"

namespace logi {

class SecondaryCommmandBuffer : public CommandBuffer {
public:
	SecondaryCommmandBuffer() = default;

	SecondaryCommmandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device,
		const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, bool resetable);
};

}

#endif