#include "program/render_pass/GraphicalPipeline.h"

namespace logi {

GraphicalPipeline::GraphicalPipeline() : Pipeline(), state_(nullptr) {
}

GraphicalPipeline::GraphicalPipeline(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const vk::Pipeline& pipeline, const PipelineLayout& layout, const PipelineState& state)
	: Pipeline(owner, device, pipeline, layout), state_(std::make_shared<PipelineState>(state)) { }


const PipelineState& GraphicalPipeline::state() const {
	return *state_;
}

}
