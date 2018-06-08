#include "program/render_pass/RenderPass.h"
#include "program/render_pass/GraphicalPipeline.h"

namespace logi {

RenderPass::RenderPass() : DependentDestroyableHandle({}, false), data_(nullptr), handle_manager_(nullptr) { }

RenderPass::RenderPass(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const RenderPassLayout& layout)
	: DependentDestroyableHandle(owner), data_(std::make_shared<RenderPassData>(layout)), handle_manager_(std::make_shared<HandleManager>()){
	data_->vk_render_pass = ManagedVkRenderPass(device, device.createRenderPass(*data_->layout.buildCreateInfo()));
}

vk::Extent2D RenderPass::renderAreaGranularity() const {
	return data_->vk_render_pass.getOwner().getRenderAreaGranularity(data_->vk_render_pass.get());
}

vk::RenderPass RenderPass::getVkHandle() const {
	return data_->vk_render_pass.get();
}

GraphicalPipeline RenderPass::addGraphicalPipeline(const vk::Pipeline& pipeline, const PipelineLayout& layout, const PipelineState& state) const {
	return handle_manager_->createHandle<GraphicalPipeline>(data_->vk_render_pass.getOwner(), pipeline, layout, state);
}

RenderPass::RenderPassData::RenderPassData(const RenderPassLayout& layout)
	: layout(layout) { }

}
