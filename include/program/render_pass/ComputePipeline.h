#ifndef PROGRAM_RENDER_PASS_COMPUTE_PIPELINE_H
#define PROGRAM_RENDER_PASS_COMPUTE_PIPELINE_H

#include <vulkan/vulkan.hpp>
#include <memory>
#include "program/layout/PipelineLayout.h"
#include "program/render_pass/Pipeline.h"

namespace logi {

class ComputePipeline : public Pipeline {
public:
	ComputePipeline();

	ComputePipeline(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const vk::Pipeline& pipeline, 
		const PipelineLayout& layout);

};

}

#endif