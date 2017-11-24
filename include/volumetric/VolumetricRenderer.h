/*
* Vulkan Renderer base class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef VOLUMETRIC_RENDERER_H_
#define VOLUMETRIC_RENDERER_H_

#include <vulkan/vulkan.hpp>
#include <memory>
#include <vector>
#include <functional>
#include "base/RendererBase.h"

namespace vkr {

class VolumetricRender : public RendererBase {
public:
	VolumetricRender(std::vector<char *>& global_extensions, std::vector<char *>& device_extensions, vk::PhysicalDeviceFeatures& features);

private:
	VulkanDevice* gpu_;
};

}

#endif