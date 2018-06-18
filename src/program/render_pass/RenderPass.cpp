#include <utility>
#include "program/render_pass/RenderPass.h"

namespace logi {

RenderPass::RenderPass() : DependentDestroyableHandle({}, false), data_(nullptr), handle_manager_(nullptr) { }

RenderPass::RenderPass(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const RenderPassLayout& layout)
	: DependentDestroyableHandle(owner), data_(std::make_shared<RenderPassData>(layout)), handle_manager_(std::make_shared<HandleManager>()){
	data_->vk_render_pass = ManagedVkRenderPass(device, device.createRenderPass(*data_->layout.buildCreateInfo()));
}

vk::Extent2D RenderPass::renderAreaGranularity() const {
	return data_->vk_render_pass.getOwner().getRenderAreaGranularity(data_->vk_render_pass.get());
}

Framebuffer RenderPass::createFramebuffer(const std::vector<ImageView>& attachments, uint32_t width, uint32_t height,
	uint32_t layers, const vk::FramebufferCreateFlags& flags) const {
	return handle_manager_->createHandle<Framebuffer>(data_->vk_render_pass.getOwner(), data_->vk_render_pass.get(), attachments, width, height, layers, flags);
}

vk::RenderPass RenderPass::getVkHandle() const {
	return data_->vk_render_pass.get();
}

GraphicalPipeline RenderPass::addGraphicalPipeline(const vk::Pipeline& pipeline, const PipelineLayout& layout, const PipelineState& state) const {
	return handle_manager_->createHandle<GraphicalPipeline>(data_->vk_render_pass.getOwner(), pipeline, layout, state);
}

RenderPass::RenderPassData::RenderPassData(RenderPassLayout layout)
	: layout(std::move(layout)) { }

}
