#ifndef COMMANDS_COMMAND_POOL_H
#define COMMANDS_COMMAND_POOL_H
#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/base/ManagedResource.h"
#include "logi/commands/CommandBuffer.h"
#include "logi/commands/PrimaryCommandBuffer.h"
#include "logi/commands/SecondaryCommandBuffer.h"

namespace logi {

class QueueFamily;

class CommandPool : public DestroyableOwnedHandle<QueueFamily>,
                    public HandleGenerator<CommandPool, PrimaryCommandBuffer>,
                    public HandleGenerator<CommandPool, SecondaryCommmandBuffer> {
 public:
  /**
   * @brief Default placeholder constructor.
   */
  CommandPool() = default;

  /**
   * @brief	Creates and initializes Vulkan CommandPool handle.
   *
   * @param	device  Queue family handle.
   * @param	flags		Flags indicating usage behavior for the pool and command buffers allocated from it.
   */
  CommandPool(const QueueFamily& queue_family, const vk::CommandPoolCreateFlags& flags);

  /**
   * @brief	Allocates Vulkan primary CommandBuffer from the pool and returns a PrimaryCommandBuffer handle that is used
   * to access the buffer.
   *
   * @return	PrimaryCommandBuffer handle.
   */
  const PrimaryCommandBuffer& createPrimaryCommandBuffer() const;

  /**
   * @brief	Allocates Vulkan primary CommandBuffer from the pool and returns a PrimaryCommandBuffer handle that is used
   * to access the buffer.
   *
   * @return	SecondaryCommandBuffer handle.
   */
  const SecondaryCommmandBuffer& createSecondaryCommandBuffer() const;

  /**
   * @brief	Resets command pool. This frees resources of all CommandBuffers allocated from this pool and puts them in
   * the INITIAL state.
   *
   * @param	flags	Flags used to control reset operation
   */
  void reset(const vk::CommandPoolResetFlags& flags) const;

  /**
   * @brief Release the given command buffer.
   *
   * @param command_buffer CommandBuffer to be released.
   */
  void freeCommandBuffer(const CommandBuffer& command_buffer) const;

  /**
   * @brief	Retrieve flags with which the CommandPool has been instantiated.
   *
   * @return	Command pool flags.
   */
  const vk::CommandPoolCreateFlags& flags() const;

 protected:
  std::vector<vk::CommandBuffer> allocateCommandBuffers(vk::CommandBufferLevel level, uint32_t count) const;

  /**
   * @brief Free resources.
   */
  void free() override;

 private:
  using ManagedVkCommandPool =
    ManagedResource<vk::Device, vk::CommandPool, vk::DispatchLoaderStatic, &vk::Device::destroyCommandPool>;

  /**
   * @brief	Structure containing CommandPool handle data.
   */
  struct Data {
    /**
     * @brief	Initializes CommandPoolData members.
     *
     * @param vk_device Vulkan device handle.
     * @param	flags     Flags indicating usage behavior for the pool and command buffers allocated from it.
     */
    explicit Data(const vk::Device& vk_device, const vk::CommandPoolCreateFlags& flags);

    /**
     * Flags indicating usage behavior for the pool and command buffers allocated from it.
     */
    vk::CommandPoolCreateFlags flags;

    /**
     * Vulkan device handle.
     */
    vk::Device vk_device;

    /**
     * Managed vulkan CommandPool handle.
     */
    ManagedVkCommandPool vk_cmd_pool;
  };

  /**
   * Command pool internal data.
   */
  std::shared_ptr<Data> data_;
};

} // namespace logi

#endif // !COMMANDS_COMMAND_POOL_H
