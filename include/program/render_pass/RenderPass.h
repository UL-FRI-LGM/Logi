#ifndef PROGRAM_RENDER_PASS_RENDER_PASS_H
#define PROGRAM_RENDER_PASS_RENDER_PASS_H

#include <vulkan/vulkan.hpp>

#include "program/render_pass/RenderPassLayout.h"
#include "program/layout/PipelineState.h"
#include "program/render_pass/GraphicalPipeline.h"
#include "memory/Framebuffer.h"

namespace logi {

class ProgramManager;

class RenderPass : public DependentDestroyableHandle {
friend class ProgramManager;
public:
	RenderPass();

	RenderPass(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const RenderPassLayout& layout);

	vk::Extent2D renderAreaGranularity() const;

	Framebuffer createFramebuffer(const std::vector<ImageView>& attachments, uint32_t width, uint32_t height, uint32_t layers, const vk::FramebufferCreateFlags& flags = {}) const;

	vk::RenderPass getVkHandle() const;

protected:
	GraphicalPipeline addGraphicalPipeline(const vk::Pipeline& pipeline, const PipelineLayout& layout, const PipelineState& state) const;

private:
	using ManagedVkRenderPass = ManagedResource<vk::Device, vk::RenderPass, vk::DispatchLoaderStatic, &vk::Device::destroyRenderPass>;

	struct RenderPassData {

		explicit RenderPassData(RenderPassLayout layout);

		ManagedVkRenderPass vk_render_pass{};
		RenderPassLayout layout;
	};

	std::shared_ptr<RenderPassData> data_;
	std::shared_ptr<HandleManager> handle_manager_;
};

}

#endif