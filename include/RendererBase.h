/*
* Vulkan Renderer base class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef RENDERER_BASE_H_
#define RENDERER_BASE_H_

#include <vulkan/vulkan.hpp>
#include <memory>
#include <vector>
#include "VulkanDevice.h"

namespace vkr {

class RendererBase {
public:
	RendererBase(std::vector<char *> extension_names);

	/**
	* @brief Check if the given validation layers are supported.
	*
	* @param layers Vector containing names of validation layers.
	* @return True if all layers are supported.
	*/
	static bool checkValidationLayerSupport(const std::vector<char*>& layers);

	void setupDebugCallback(const vk::DebugReportFlagsEXT& flags, PFN_vkDebugReportCallbackEXT callback);

	~RendererBase();

private:
	vk::Instance vk_instance_;
	std::vector<std::unique_ptr<VulkanDevice>> devices_;

	std::vector<vk::DebugReportCallbackEXT> debug_callbacks_;

};

} /// !namespace vkr

#endif // !RENDERER_BASE_H_