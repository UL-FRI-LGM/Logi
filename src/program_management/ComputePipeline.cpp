#include "program_management\ComputePipeline.h"

namespace vkr {

ComputePipeline::ComputePipeline(PipelineLayout* pipeline_layout) : pipeline_layout_(pipeline_layout) {
	if (pipeline_layout_->getPipelineType() != PipelineType::COMPUTE) {
		throw std::runtime_error("Invalid pipeline layout type.");
	}
}

const std::string& ComputePipeline::getName() {
	return pipeline_layout_->getName();
}


vk::Pipeline ComputePipeline::getVkHandle(VulkanDevice* device) {
	PipelineResources* resources = device->getPipelineResources();
	std::vector<size_t> shader_indices = pipeline_layout_->getShaderResourceIds(device);

	// Try to fetch existing pipeline
	vk::Pipeline pipeline = resources->getComputePipeline(shader_indices[0]);

	// If the pipeline does not exist create it.
	if (!pipeline) {
		resources->createComputePipeline(shader_indices[0], pipeline_layout_->getLayoutResourceId(device));
		pipeline = resources->getComputePipeline(shader_indices[0]);
	}

	return pipeline;
}

}