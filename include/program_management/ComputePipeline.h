#ifndef BASE_COMPUTE_PIPELINE_H
#define BASE_COMPUTE_PIPELINE_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include "program_management/PipelineLayout.h"

namespace vkr {

class ComputePipeline {
public:
	ComputePipeline(PipelineLayout* pipeline_layout);

	const std::string& getName();

	vk::Pipeline getVkHandle(VulkanDevice* device);

private:
	PipelineLayout* pipeline_layout_;
};

}

#endif