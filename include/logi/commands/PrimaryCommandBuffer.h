#ifndef COMMANDS_PRIMARY_COMMAND_BUFFER_H
#define COMMANDS_PRIMARY_COMMAND_BUFFER_H

#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/commands/CommandBuffer.h"
#include "logi/commands/SecondaryCommandBuffer.h"
#include "logi/program/render_pass/RenderPass.h"

namespace logi {

/**
 * @brief   Aggregates data required to build Vulkan CommandBufferBeginInfo.
 */
struct CommandBufferBeginInfo : public BuildableExtendable {
  /**
   * @brief	Initializes CommandBufferBeginInfo data.
   *
   * @param	flags   Flags specifying usage behavior for the command buffer.
   */
  explicit CommandBufferBeginInfo(const vk::CommandBufferUsageFlags& flags = {});

  /**
   * @brief	Builds Vulkan CommandBufferBeginInfo.
   *
   * @return	Vulkan CommandBufferBeginInfo.
   */
  vk::CommandBufferBeginInfo build() const;

  /**
   * Flags specifying usage behavior for the command buffer.
   */
  vk::CommandBufferUsageFlags flags;
};

class PrimaryCommandBuffer : public CommandBuffer {
 public:
  using CommandBuffer::CommandBuffer;

  /**
   * @brief	Begins recording PrimaryCommandBuffer.
   *
   * @param	begin_info Command buffer begin info.
   */
  void begin(const CommandBufferBeginInfo& begin_info = CommandBufferBeginInfo()) const;

  /**
   * @brief	Execute secondary command buffers from a primary command buffer
   *
   * @param	cmd_buffers
   */
  void executeCommands(std::vector<SecondaryCommmandBuffer>& cmd_buffers) const;

  /**
   * @brief	Transition to the next subpass of a render pass
   *
   * @param	contents    Specifies how the commands in the next subpass will be provided.
   */
  void nextSubpass(vk::SubpassContents contents = vk::SubpassContents::eInline) const;
};

} // namespace logi

#endif // !COMMANDS_PRIMARY_COMMAND_BUFFER_H