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
#include <functional>
#include "base/VulkanDevice.h"
#include "base/ShaderManager.h"

namespace vkr {

class RendererBase {
public:
	RendererBase(std::vector<char *>& global_extensions);

	/**
	* @brief Check if the given validation layers are supported.
	*
	* @param layers Vector containing names of validation layers
	* @return True if all layers are supported.
	*/
	static bool checkValidationLayerSupport(const std::vector<char*>& layers);

	void setupDebugCallback(const vk::DebugReportFlagsEXT& flags, PFN_vkDebugReportCallbackEXT callback);

	~RendererBase();

protected:
	/**
	 * @brief Retrieves raw pointers to the devices sorted based on the score assigned to them by the give function.
	 * If the function assigns negative score to the device it will be omitted.
	 *
	 * @param f_score Vulkan device scoring function.
	 * @return Vector of pairs containing the assigned score and the pointer to the VulkanDevice.
	 */
	std::vector<std::pair<int32_t, VulkanDevice*>> retrieveDevices(const std::function<int(VulkanDevice*)>& f_score);

private:
	vk::Instance vk_instance_;
	std::vector<std::unique_ptr<VulkanDevice>> devices_;
	std::vector<vk::DebugReportCallbackEXT> debug_callbacks_;

};

} /// !namespace vkr

#endif // !RENDERER_BASE_H_