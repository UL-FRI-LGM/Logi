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
#include "base/PhysicalDevice.h"

namespace logi {

struct ApplicationInfo {
	ApplicationInfo(const std::string& app_name, uint32_t app_version, const std::string& engine_name, uint32_t engine_version);

	std::string app_name;
	uint32_t app_version;
	std::string engine_name;
	uint32_t engine_version;
};

struct InstanceConfiguration {
	InstanceConfiguration(const std::vector<const char*>& extensions = {}, bool enable_validation = false, const std::vector<const char*>& validation_layers = {});

	std::vector<const char*> extensions;
	bool enable_validation;
	std::vector<const char*> validation_layers;
};


class VulkanInstance : public DestroyableHandle {
public:
	VulkanInstance();

	VulkanInstance(const ApplicationInfo& application_info, const InstanceConfiguration& configuration);

	void setupDebugCallback(const vk::DebugReportFlagsEXT& flags, PFN_vkDebugReportCallbackEXT callback) const;

	const ApplicationInfo& applicationInfo() const;

	const InstanceConfiguration& configuration() const;

	const std::vector<PhysicalDevice>& devices() const;

	const vk::Instance& getVkHandle() const;

	/**
	* @brief Check if the given validation layers are supported.
	*
	* @param layers Vector containing names of validation layers
	* @return True if all layers are supported.
	*/
	static bool checkValidationLayerSupport(const std::vector<const char*>& layers);

protected:
	void free() override;

private:
	struct InstanceData {
		InstanceData(const ApplicationInfo& application_info, const InstanceConfiguration& configuration);

		vk::Instance vk_instance{};
		ApplicationInfo application_info;
		InstanceConfiguration configuration;
		std::vector<PhysicalDevice> physical_devices{};
		std::vector<vk::DebugReportCallbackEXT> debug_callbacks{};
	};

	std::shared_ptr<InstanceData> data_;
	std::shared_ptr<HandleManager> handle_manager_;	//TODO Move this to InstanceData.

};

} /// !namespace vkr

#endif // !RENDERER_BASE_H_