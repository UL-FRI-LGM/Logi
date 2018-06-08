#include "commands/CommandPool.h"
#include "commands/CommandBuffer.h"
#include <vulkan/vulkan.hpp>

namespace logi {

CommandPool::CommandPool(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const uint32_t queue_family_index, const vk::CommandPoolCreateFlags& flags) 
	: DependentDestroyableHandle(owner), data_(std::make_shared<CommandPoolData>(flags)), handle_manager_(std::make_shared<HandleManager>()) {

	// Create command pool.
	vk::CommandPoolCreateInfo pool_ci{};
	pool_ci.flags = data_->flags;
	pool_ci.queueFamilyIndex = queue_family_index;
	
	vk_cmd_pool_ = std::make_shared<ManagedVkCommandPool>(device, device.createCommandPool(pool_ci));
}

std::vector<vk::CommandBuffer> CommandPool::allocateCommandBuffers(const vk::CommandBufferLevel level, const uint32_t count) const {
	vk::CommandBufferAllocateInfo allocate_info{};
	allocate_info.commandPool = vk_cmd_pool_->get();
	allocate_info.level = level;
	allocate_info.commandBufferCount = count;

	// Allocate buffers from the pool.
	return vk_cmd_pool_->getOwner().allocateCommandBuffers(allocate_info);
}

PrimaryCommandBuffer CommandPool::createPrimaryCommandBuffer() const {
	vk::CommandBuffer buffer = allocateCommandBuffers(vk::CommandBufferLevel::ePrimary, 1u)[0];

	return handle_manager_->createHandle<PrimaryCommandBuffer>(vk_cmd_pool_->getOwner(), vk_cmd_pool_->get(), buffer, static_cast<bool>(data_->flags & vk::CommandPoolCreateFlagBits::eResetCommandBuffer));
}

SecondaryCommmandBuffer CommandPool::createSecondaryCommandBuffer() const {
	vk::CommandBuffer buffer = allocateCommandBuffers(vk::CommandBufferLevel::eSecondary, 1u)[0];

	return handle_manager_->createHandle<SecondaryCommmandBuffer>(vk_cmd_pool_->getOwner(), vk_cmd_pool_->get(), buffer, static_cast<bool>(data_->flags & vk::CommandPoolCreateFlagBits::eResetCommandBuffer));
}

void CommandPool::reset(const vk::CommandPoolResetFlags& flags) const {
	if (!alive()) {
		throw std::runtime_error("Called 'reset' on destroyed CommandPool.");
	}

	vk_cmd_pool_->getOwner().resetCommandPool(vk_cmd_pool_->get(), flags);
}

const vk::CommandPoolCreateFlags& CommandPool::flags() const {
	return data_->flags;
}

void CommandPool::free() {
	handle_manager_->destroyAllHandles();
	vk_cmd_pool_->destroy();
}

CommandPool::CommandPoolData::CommandPoolData(const vk::CommandPoolCreateFlags& flags) : flags(flags) {}

} /// !namespace vkr