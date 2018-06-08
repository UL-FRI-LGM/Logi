#include "program/render_pass/GraphicalPipeline.h"

namespace logi {

GraphicalPipeline::GraphicalPipeline() : DependentDestroyableHandle({}, false), data_(nullptr) {
}

GraphicalPipeline::GraphicalPipeline(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const vk::Pipeline& pipeline, const PipelineLayout& layout, const PipelineState& state)
	: DependentDestroyableHandle(owner), data_(std::make_shared<GraphicalPipelineData>(ManagedVkPipeline(device, pipeline), layout, state)) { }

const vk::Pipeline& GraphicalPipeline::getVkHandle() const {
	return data_->vk_pipeline.get();
}

const PipelineLayout& GraphicalPipeline::layout() const {
	return data_->layout;
}

const PipelineState& GraphicalPipeline::state() const {
	return data_->state;
}

void GraphicalPipeline::free() {
	data_->vk_pipeline.destroy();
}

GraphicalPipeline::GraphicalPipelineData::GraphicalPipelineData(const ManagedVkPipeline& vk_pipeline, const PipelineLayout& layout, const PipelineState& state)
	: vk_pipeline(vk_pipeline), layout(layout), state(state) {
}


}
