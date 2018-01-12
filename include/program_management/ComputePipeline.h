#ifndef BASE_COMPUTE_PIPELINE_H
#define BASE_COMPUTE_PIPELINE_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include "program_management/PipelineLayout.h"

namespace vkr {

class ComputePipeline {
public:
	ComputePipeline(const vk::Device& device, PipelineLayout* pipeline_layout);

	const std::string& getName() const;

	const PipelineLayout* getPipelineLayout() const;

	const vk::Pipeline& getVkHandle() const;

	~ComputePipeline();

private:
	vk::Device device_;
	vk::Pipeline pipeline_;

	PipelineLayout* pipeline_layout_;
};

}

#endif