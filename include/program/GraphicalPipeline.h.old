#ifndef PROGRAM_GRAPHICAL_PIPELINE_H
#define PROGRAM_GRAPHICAL_PIPELINE_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include "program/layout/PipelineLayout.h"
#include "program/layout/PipelineState.h"

namespace vkr {

class GraphicalPipeline {
public:
	GraphicalPipeline(const vk::Device& device, PipelineLayout* pipeline_layout, const PipelineState& pipeline_state, const vk::RenderPass& render_pass, subpass_index_t subpass_index);

	const std::string& getName() const;

	const PipelineLayout* getPipelineLayout() const;

	const vk::Pipeline& getVkHandle() const;

	~GraphicalPipeline();

private:
	vk::Device device_;
	vk::Pipeline pipeline_;

	PipelineLayout* pipeline_layout_;
	const PipelineState state_;
};

} ///!	namespace vkr

#endif ///! HEADER_GUARD