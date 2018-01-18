#ifndef COMMANDS_COMMAND_BUFFER_H
#define COMMANDS_COMMAND_BUFFER_H

#include <vulkan/vulkan.hpp>

namespace vkr {

class CommandBuffer {
public:
	CommandBuffer(const vk::CommandBuffer& command_buffer, bool resetable);

	virtual ~CommandBuffer() = 0;
	
	const vk::CommandBuffer& getVkHandle() const;

	void bindPipeline(const vk::Pipeline& pipeline, vk::PipelineBindPoint bind_point);

	void bindDescriptorSets(const vk::PipelineLayout& layout, const vk::PipelineBindPoint& bind_point, uint32_t first_set, const std::vector<vk::DescriptorSet>& descriptor_sets, const std::vector<uint32_t>& dynamic_offsets = {});

	void computeDispatch(uint32_t group_count_x, uint32_t group_count_y, uint32_t group_count_z);

	void endCommandBuffer();

	bool isResetable();

protected:
	vk::CommandBuffer command_buffer_;

private:
	bool resetable_;
};


class PrimaryCommandBuffer : public CommandBuffer {
public:
	PrimaryCommandBuffer(const vk::CommandBuffer& command_buffer, bool resetable);

	void beginCommandBuffer(vk::CommandBufferUsageFlags usage_flags);

};


class SecondaryCommmandBuffer : public CommandBuffer {
public:
	SecondaryCommmandBuffer(const vk::CommandBuffer& command_buffer, bool resetable);
};

}

#endif // !COMMANDS_COMMAND_BUFFER_H
