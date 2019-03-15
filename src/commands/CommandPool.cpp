#include "logi/commands/CommandPool.h"
#include <vulkan/vulkan.hpp>
#include "logi/base/LogicalDevice.h"
#include "logi/commands/CommandBuffer.h"
#include "logi/queues/QueueFamily.h"

namespace logi {

CommandPool::CommandPool(const QueueFamily& queue_family, const vk::CommandPoolCreateFlags& flags)
  : DestroyableOwnedHandle(queue_family, true), data_(std::make_shared<Data>(queue_family.getOwner(), flags)) {
  // Create command pool.
  vk::CommandPoolCreateInfo pool_ci;
  pool_ci.flags = data_->flags;
  pool_ci.queueFamilyIndex = queue_family.configuration().properties.family_index;

  data_->vk_cmd_pool = ManagedVkCommandPool(data_->vk_device, data_->vk_device.createCommandPool(pool_ci));
}

const PrimaryCommandBuffer& CommandPool::createPrimaryCommandBuffer() const {
  checkForNullHandleInvocation("CommandPool", "createPrimaryCommandBuffer");
  vk::CommandBuffer buffer = allocateCommandBuffers(vk::CommandBufferLevel::ePrimary, 1u)[0];
  HandleGenerator<CommandPool, PrimaryCommandBuffer>::createHandle(buffer);
}

const SecondaryCommmandBuffer& CommandPool::createSecondaryCommandBuffer() const {
  checkForNullHandleInvocation("CommandPool", "createSecondaryCommandBuffer");
  vk::CommandBuffer buffer = allocateCommandBuffers(vk::CommandBufferLevel::eSecondary, 1u)[0];
  HandleGenerator<CommandPool, SecondaryCommmandBuffer>::createHandle(buffer);
}

void CommandPool::reset(const vk::CommandPoolResetFlags& flags) const {
  checkForNullHandleInvocation("CommandPool", "reset");
  data_->vk_device.resetCommandPool(data_->vk_cmd_pool.get(), flags);
}

void CommandPool::freeCommandBuffer(const CommandBuffer& command_buffer) const {
  checkForNullHandleInvocation("CommandPool", "freeCommandBuffer");
  vk::CommandBuffer vk_cmd_buffer = command_buffer;
  data_->vk_device.freeCommandBuffers(data_->vk_cmd_pool.get(), 1u, &vk_cmd_buffer);
}

const vk::CommandPoolCreateFlags& CommandPool::flags() const {
  checkForNullHandleInvocation("CommandPool", "flags");
  return data_->flags;
}

std::vector<vk::CommandBuffer> CommandPool::allocateCommandBuffers(const vk::CommandBufferLevel level,
                                                                   const uint32_t count) const {
  vk::CommandBufferAllocateInfo allocate_info;
  allocate_info.commandPool = data_->vk_cmd_pool.get();
  allocate_info.level = level;
  allocate_info.commandBufferCount = count;

  // Allocate buffers from the pool.
  return data_->vk_device.allocateCommandBuffers(allocate_info);
}

void CommandPool::free() {
  if (valid()) {
    HandleGenerator<CommandPool, PrimaryCommandBuffer>::destroyAllHandles();
    HandleGenerator<CommandPool, SecondaryCommmandBuffer>::destroyAllHandles();
    data_->vk_cmd_pool.destroy();
    DestroyableOwnedHandle<QueueFamily>::free();
  }
}

CommandPool::Data::Data(const vk::Device& vk_device, const vk::CommandPoolCreateFlags& flags)
  : vk_device(vk_device), flags(flags) {}

} // namespace logi