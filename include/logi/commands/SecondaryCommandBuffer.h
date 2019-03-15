#ifndef COMMANDS_PRIMARY_SECONDARY_BUFFER_H
#define COMMANDS_PRIMARY_SECONDARY_BUFFER_H

#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/commands/CommandBuffer.h"

namespace logi {

class SecondaryCommmandBuffer : public CommandBuffer {
 public:
  using CommandBuffer::CommandBuffer;
};

} // namespace logi

#endif