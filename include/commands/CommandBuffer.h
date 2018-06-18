#ifndef COMMANDS_COMMAND_BUFFER_H
#define COMMANDS_COMMAND_BUFFER_H

#include <vulkan/vulkan.hpp>
#include "base/Handle.h"

namespace logi {


class CommandBuffer : public DependentDestroyableHandle {
public:
	CommandBuffer();

	CommandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device, const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, bool resetable);


	const vk::CommandBuffer& getVkHandle() const;

	bool isResetable() const;

	void destroy() const override;

	virtual ~CommandBuffer() = 0;

protected:
	/**
	 * @brief	Structure used to hold CommandBuffer data.
	 */
	struct CommandBufferData {
		/**
		 * @brief	Populates CommandBufferData.
		 *
		 * @param	vk_device		Vulkan Device handle.
		 * @param	vk_cmd_pool		Vulkan CommandPool handle.
		 * @param	vk_cmd_buffer	Vulkan CommandBuffer handle.
		 * @param	resetable			If true CommandBuffer may be reset.
		 */
		CommandBufferData(const vk::Device& vk_device, const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, bool resetable);

		vk::Device vk_device;				///< Vulkan Device handle.
		vk::CommandPool vk_cmd_pool;		///< Vulkan CommandPool handle.
		vk::CommandBuffer vk_cmd_buffer;	///< Vulkan CommandBuffer handle.
		bool resetable;						///< If true CommandBuffer may be reset.
	};

	const CommandBufferData& cmdBufferData() const;

private:
	std::shared_ptr<CommandBufferData> data_;
};


class PrimaryCommandBuffer : public CommandBuffer {
public:
	PrimaryCommandBuffer() = default;

	PrimaryCommandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device, const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, bool resetable);
};


class SecondaryCommmandBuffer : public CommandBuffer {
public:
	SecondaryCommmandBuffer() = default;

	SecondaryCommmandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device, const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, bool resetable);
};

}

#endif // !COMMANDS_COMMAND_BUFFER_H
