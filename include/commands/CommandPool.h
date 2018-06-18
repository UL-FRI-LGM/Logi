#ifndef COMMANDS_COMMAND_POOL_H
#define COMMANDS_COMMAND_POOL_H
#include <vulkan/vulkan.hpp>
#include "commands/CommandBuffer.h"
#include "base/Handle.h"
#include "base/ManagedResource.h"

namespace logi {

class CommandPool : public DependentDestroyableHandle {
public:
	CommandPool();

	/**
	 * @brief	Creates and initializes Vulkan CommandPool handle.
	 *
	 * @param	owner				Pointer to HandleManager responsible for this CommandPool handle.
	 * @param	device				Vulkan device handle.
	 * @param	queue_family_index	Index of the queue family.
	 * @param	flags				Flags indicating usage behavior for the pool and command buffers allocated from it.
	 */
	CommandPool(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, uint32_t queue_family_index, const vk::CommandPoolCreateFlags& flags);
	
	/**
	 * @brief	Allocates Vulkan primary CommandBuffer from the pool and returns a PrimaryCommandBuffer handle that is used to access the buffer.
	 *
	 * @return	PrimaryCommandBuffer handle.
	 */
	PrimaryCommandBuffer createPrimaryCommandBuffer() const;

	/**
	 * @brief	Allocates Vulkan primary CommandBuffer from the pool and returns a PrimaryCommandBuffer handle that is used to access the buffer.
	 *
	 * @return	SecondaryCommandBuffer handle.
	 */
	SecondaryCommmandBuffer createSecondaryCommandBuffer() const;

	/**
	 * @brief	Resets command pool. This frees resources of all CommandBuffers allocated from this pool and puts them in the INITIAL state.
	 *
	 * @param	flags	Flags used to control reset operation
	 */
	void reset(const vk::CommandPoolResetFlags& flags) const;

	/**
	 * @brief Release the given command buffer.
	 *
	 * @param command_buffer CommandBuffer to be released.
	 */
	//void freeCommandBuffer(CommandBuffer*  command_buffer);

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
	using ManagedVkCommandPool = ManagedResource<vk::Device, vk::CommandPool, vk::DispatchLoaderStatic, &vk::Device::destroyCommandPool>;

	/**
	 * @brief	Structure containing CommandPool handle data.
	 */
	struct CommandPoolData {
		/**
		 * @brief	Initializes CommandPoolData members.
		 *
		 * @param	flags	Flags indicating usage behavior for the pool and command buffers allocated from it. 
		 */
		explicit CommandPoolData(const vk::CommandPoolCreateFlags& flags);

		vk::CommandPoolCreateFlags flags;	///< Flags indicating usage behavior for the pool and command buffers allocated from it.
	};

	std::shared_ptr<CommandPoolData> data_;				///< CommandPool data.
	std::shared_ptr<ManagedVkCommandPool> vk_cmd_pool_;	///< Managed vulkan CommandPool handle.
	std::shared_ptr<HandleManager> handle_manager_;	///< Command buffer handle manager.
};

}

#endif // !COMMANDS_COMMAND_POOL_H
