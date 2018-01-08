#include "commands/CommandPool.h"
#include "commands/CommandBuffer.h"
#include <vulkan/vulkan.hpp>

namespace vkr {

CommandPool::CommandPool(const vk::Device& device, uint32_t queue_family_index, bool transistent, bool resetable_buffers) : device_(device), cmd_pool_(nullptr), transistent_(transistent), resetable_buffers_(resetable_buffers) {
	vk::CommandPoolCreateFlags flags{};

	// Transistent bit indicates that command buffers allocated from the pool will be short - lived.
	if (transistent) {
		flags |= vk::CommandPoolCreateFlagBits::eTransient;
	}

	// Allows any command buffer allocated from a pool to be individually reset to the initial state.
	if (resetable_buffers) {
		flags |= vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	}
	
	// Create command pool.
	vk::CommandPoolCreateInfo pool_ci{};
	pool_ci.flags = flags;
	pool_ci.queueFamilyIndex = queue_family_index;
	
	cmd_pool_ = device.createCommandPool(pool_ci);
}

vk::CommandBuffer CommandPool::allocateCommandBuffer(const vk::CommandBufferLevel&& level) {
	vk::CommandBufferAllocateInfo commandBufferAllocateInfo{};
	commandBufferAllocateInfo.commandPool = cmd_pool_;
	commandBufferAllocateInfo.level = level;
	commandBufferAllocateInfo.commandBufferCount = 1;

	// Allocate the buffer from the pool.
	vk::CommandBuffer command_buffer;
	device_.allocateCommandBuffers(&commandBufferAllocateInfo, &command_buffer);

	return command_buffer;
}

std::shared_ptr<PrimaryCommandBuffer> CommandPool::allocatePrimaryCommandBuffer() {
	return std::make_shared<PrimaryCommandBuffer>(allocateCommandBuffer(vk::CommandBufferLevel::ePrimary), resetable_buffers_);
}

std::shared_ptr<SecondaryCommmandBuffer> CommandPool::allocateSecondaryCommandBuffer() {
	return std::make_shared<SecondaryCommmandBuffer>(allocateCommandBuffer(vk::CommandBufferLevel::eSecondary), resetable_buffers_);
}

void CommandPool::resetCommandPool(bool release_resources) {
	if (release_resources) {
		device_.resetCommandPool(cmd_pool_, vk::CommandPoolResetFlagBits::eReleaseResources);
	}
	else {
		device_.resetCommandPool(cmd_pool_, vk::CommandPoolResetFlags());
	}
}

void CommandPool::freeCommandBuffer(const std::shared_ptr<CommandBuffer>& command_buffer) {
	device_.freeCommandBuffers(cmd_pool_, command_buffer->getVkHandle());
}

bool CommandPool::isTransistent() const {
	return transistent_;
}

bool CommandPool::allowsBufferReset() const {
	return resetable_buffers_;
}


CommandPool::~CommandPool() {
}

} /// !namespace vkr