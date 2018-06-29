#include "program/render_pass/ComputePipeline.h"

namespace logi {

ComputePipeline::ComputePipeline() : Pipeline() {
}

ComputePipeline::ComputePipeline(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, 
	const vk::Pipeline& pipeline, const PipelineLayout& layout) : Pipeline(owner, device, pipeline, layout) {
}

}