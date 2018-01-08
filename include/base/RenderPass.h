#ifndef BASE_RENDER_PASS_H
#define BASE_RENDER_PASS_H

#include <vulkan/vulkan.hpp>
#include <tuple>

namespace vkr {

class RenderPass {
public:
	RenderPass(vk::Device device);

	std::tuple<bool, uint32_t> addColorAttachment(vk::Format format, 
																								vk::SampleCountFlagBits sample_count, 
																								vk::AttachmentLoadOp load_op, 
																								vk::AttachmentStoreOp store_op,
																								vk::ImageLayout initial_layout,
																								vk::ImageLayout final_layout,
																								bool may_alias);

	std::tuple<bool, uint32_t> addDepthStencilAttachment(vk::Format format,
																													vk::SampleCountFlagBits sample_count,
																													vk::AttachmentLoadOp depth_load_op,
																													vk::AttachmentStoreOp depth_store_op,
																													vk::AttachmentLoadOp stencil_load_op,
																													vk::AttachmentStoreOp stencil_store_op,
																													vk::ImageLayout initial_layout,
																													vk::ImageLayout final_layout,
																													bool may_alias);

	std::tuple<bool, uint32_t> addSubpass();

	std::tuple<bool, uint32_t> addSubpassColorAttachment(uint32_t subpass_idx, 
																									uint32_t attachment_idx, 
																									vk::ImageLayout layout, 
																									uint32_t location);


private:
	vk::RenderPassCreateInfo configuration_;
	vk::RenderPass render_pass_;
	bool dirty_;
};

}

#endif