#ifndef PROGRAM_RENDER_PASS_RENDER_PASS_H
#define PROGRAM_RENDER_PASS_RENDER_PASS_H

#include <vulkan/vulkan.hpp>
#include "program/render_pass/RenderPassLayout.h"
#include "base/VulkanDevice.h"

namespace vkr {

class RenderPass {
public:
	RenderPass(VulkanDevice* device, RenderPassLayout& layout);


private:
	VulkanDevice* device_;
	vk::RenderPass vk_render_pass_;
};

}

#endif