#ifndef COMMANDS_COMMAND_BUFFER_H
#define COMMANDS_COMMAND_BUFFER_H

#include <vulkan/vulkan.hpp>
#include "base/Handle.h"
#include "program/render_pass/RenderPass.h"
#include "program/render_pass/ComputePipeline.h"
#include "descriptors/DescriptorSet.h"
#include "memory/Framebuffer.h"

namespace logi {


class CommandBuffer : public DependentDestroyableHandle {
public:
	CommandBuffer();

	CommandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device,
	              const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, bool resetable);

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
		CommandBufferData(const vk::Device& vk_device, const vk::CommandPool& vk_cmd_pool,
		                  const vk::CommandBuffer& vk_cmd_buffer, bool resetable);

		vk::Device vk_device;            ///< Vulkan Device handle.
		vk::CommandPool vk_cmd_pool;     ///< Vulkan CommandPool handle.
		vk::CommandBuffer vk_cmd_buffer; ///< Vulkan CommandBuffer handle.
		bool resetable;                  ///< If true CommandBuffer may be reset.
	};

	std::shared_ptr<CommandBufferData> data_;
};


class PrimaryCommandBuffer : public CommandBuffer {
public:
	PrimaryCommandBuffer() = default;

	PrimaryCommandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device,
	                     const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, bool resetable);

	void begin(const vk::CommandBufferUsageFlags& usage_flags) const;

	void end() const;

    template <typename... Extensions>
	void beginRenderPass(const RenderPass& render_pass, const Framebuffer& framebuffer, const vk::Rect2D& render_area,
	                     std::vector<vk::ClearValue> clear_values,
	                     const vk::SubpassContents contents = vk::SubpassContents::eInline,
	                     Extensions&... extensions) const {

		// Basic info.
		vk::RenderPassBeginInfo begin_info;
		begin_info.renderPass = render_pass.getVkHandle();
		begin_info.framebuffer = framebuffer.getVkHandle();
		begin_info.renderArea = render_area;
		begin_info.pClearValues = clear_values.data();
		begin_info.clearValueCount = clear_values.size();

		begin_info.pNext = ExtensionObject::buildExtensions(extensions...);

		data_->vk_cmd_buffer.beginRenderPass(begin_info, contents);
	}

	void endRenderPass() const;

	void bindPipeline(const GraphicalPipeline& pipeline) const;

	void draw(uint32_t vertex_count, uint32_t first_vertex = 0, uint32_t instance_count = 1,
	          uint32_t first_instance = 0) const;

	void bindPipeline(const ComputePipeline& pipeline) const;

	void dispatch(uint32_t group_count_x, uint32_t group_count_y, uint32_t group_count_z) const;

	void bindDescriptorSets(vk::PipelineBindPoint pipeline_bind_point, const PipelineLayout& layout, uint32_t first_set,
	                        const std::vector<DescriptorSet>& descriptor_sets) const;
};


class SecondaryCommmandBuffer : public CommandBuffer {
public:
	SecondaryCommmandBuffer() = default;

	SecondaryCommmandBuffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device,
	                        const vk::CommandPool& vk_cmd_pool, const vk::CommandBuffer& vk_cmd_buffer, bool resetable);
};

}

#endif // !COMMANDS_COMMAND_BUFFER_H
