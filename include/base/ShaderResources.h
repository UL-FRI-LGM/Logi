/*
* Shader manager.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef SHADER_RESOURCES_H
#define SHADER_RESOURCES_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include "VulkanDevice.h"

namespace vkr {

class ShaderResources {
public:

private:
	struct PipelineResources {
		std::map<
		vk::PipelineLayout layout;
		std::vector<vk::DescriptorSetLayout> descriptor_sets;
	};

	std::vector<vk::ShaderModule> shaders_;
	

};

}

#endif