#include "program/GraphicalPipeline.h"

namespace vkr {

GraphicalPipeline::GraphicalPipeline(const vk::Device& device, PipelineLayout* pipeline_layout, const PipelineState& pipeline_state, const vk::RenderPass& render_pass, subpass_index_t subpass_index) : device_(device), pipeline_(), pipeline_layout_(pipeline_layout), state_(pipeline_state) {
	vk::GraphicsPipelineCreateInfo pipeline_ci{};

	// Bind shader stages.
	const std::vector<vk::PipelineShaderStageCreateInfo>& stages = pipeline_layout->getVkShaderHandles();
	pipeline_ci.stageCount = stages.size();
	pipeline_ci.pStages = &stages[0];

}

} ///!	namespace vkr
