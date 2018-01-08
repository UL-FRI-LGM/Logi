#ifndef COMMANDS_COMMAND_POOL_H
#define COMMANDS_COMMAND_POOL_H
#include <vulkan/vulkan.hpp>
#include "commands/CommandBuffer.h"
#include <list>

namespace vkr {

class CommandPool {
public:
	CommandPool(const vk::Device& device, uint32_t queue_family_index, bool transistent, bool resetable_buffers);
	~CommandPool();

	/**
	 * @brief Allocates primary CommandBuffer. Primary command buffers cannot be called on their own but are called from the primary command buffers.
	 *
	 * @return Shared pointer to the allocated secondary CommandBuffer.
	 */
	std::shared_ptr<PrimaryCommandBuffer> allocatePrimaryCommandBuffer();

	/**
	 * @brief Allocates secondary CommandBuffer. Secondary command buffers cannot be called on their own but are called from the primary command buffers.
	 *
	 * @return Shared pointer to the allocated secondary CommandBuffer.
	 */
	std::shared_ptr<SecondaryCommmandBuffer> allocateSecondaryCommandBuffer();

	/**
	 * @brief Resets command pool. This effectively invalidates all CommandBuffers produced by this pool.
	 *
	 * @param release_resources If true the allocated resources will be released too.
	 */
	void resetCommandPool(bool release_resources);

	/**
	 * @brief Release the given command buffer.
	 *
	 * @param command_buffer CommandBuffer to be released.
	 */
	void freeCommandBuffer(const std::shared_ptr<CommandBuffer>& command_buffer);

	/**
	 * @brief Returns true if the pool is transistent. Its command buffers are expected do be short lived.
	 *
	 * @raturn True if the buffers are allowed to be reset.
	 */
	bool isTransistent() const;
	
	/**
	 * @brief Returns true if the buffers are allowed to be reset.
	 *
	 * @raturn True if the buffers are allowed to be reset.
	 */
	bool allowsBufferReset() const;

protected:
	vk::CommandBuffer allocateCommandBuffer(const vk::CommandBufferLevel&& level);
private:
	vk::Device device_;
	vk::CommandPool cmd_pool_;

	bool transistent_;
	bool resetable_buffers_;
};

}

#endif // !COMMANDS_COMMAND_POOL_H
