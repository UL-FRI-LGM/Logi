#include "program_management/ComputePipeline.h"

namespace vkr {

ComputePipeline::ComputePipeline(const vk::Device& device, PipelineLayout* pipeline_layout) : device_(device), pipeline_(nullptr), pipeline_layout_(pipeline_layout) {
	if (pipeline_layout_->getPipelineType() != PipelineType::COMPUTE) {
		throw std::runtime_error("Invalid pipeline layout type.");
	}

	// Create compute pipeline.
	vk::ComputePipelineCreateInfo cpipeline_ci{};
	cpipeline_ci.layout = pipeline_layout_->getVkHandle();
	cpipeline_ci.stage = pipeline_layout_->getVkShaderHandles()[0];

	pipeline_ = device.createComputePipeline(vk::PipelineCache(nullptr), cpipeline_ci);
}

const std::string& ComputePipeline::getName() const {
	return pipeline_layout_->getName();
}

const PipelineLayout* ComputePipeline::getPipelineLayout() const {
	return pipeline_layout_;
}

const vk::Pipeline& ComputePipeline::getVkHandle() const {
	return pipeline_;
}

ComputePipeline::~ComputePipeline() {
	device_.destroyPipeline(pipeline_);
}

}
