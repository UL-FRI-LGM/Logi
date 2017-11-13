/*
* Vulkan Renderer base class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "base/RendererBase.h"
#include <algorithm>
#include "GlobalConfig.h"
#include "DebugUtils.h"

namespace vkr {

RendererBase::RendererBase(std::vector<char *> extension_names) {
	// Create ApplicationInfo using the global configuration.
	vk::ApplicationInfo appInfo{};
	appInfo.pApplicationName = global_config::kAppName;
	appInfo.applicationVersion = global_config::kAppVersion;
	appInfo.pEngineName = global_config::kEngineName;
	appInfo.engineVersion = global_config::kEngineVersion;
	appInfo.apiVersion = VK_API_VERSION_1_0;

	vk::InstanceCreateInfo createInfo{};
	createInfo.pApplicationInfo = &appInfo;

	// Initialize requested extensions.
	// Add debug report extension if the validation is enabled.
	if (debug_utils::kEnableValidation) {
		extension_names.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extension_names.size());
	createInfo.ppEnabledExtensionNames = extension_names.data();

	// Check if requested validation layers are supported.
	if (debug_utils::kEnableValidation && !checkValidationLayerSupport(debug_utils::kValidationLayers)) {
		throw std::runtime_error("Requested validation layers are not supported!");
	}

	// Setup validation layers if enabled.
	if (debug_utils::kEnableValidation) {
		createInfo.ppEnabledLayerNames = debug_utils::kValidationLayers.data();
		createInfo.enabledLayerCount = static_cast<uint32_t>(debug_utils::kValidationLayers.size());
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	// Try to create instance.
	if (vk::createInstance(&createInfo, nullptr, &vk_instance_) != vk::Result::eSuccess) {
		throw std::runtime_error("Failed to create instance!");
	}

	// Fetch available physical devices.
	uint32_t device_count = 0;
	vk_instance_.enumeratePhysicalDevices(&device_count, nullptr);

	// No supported device was found.
	if (device_count == 0) {
		return;
	}

	std::vector<vk::PhysicalDevice> physical_devices(device_count);
	vk_instance_.enumeratePhysicalDevices(&device_count, physical_devices.data());

	// Wrap the PhysicalDevice handle.
	for (vk::PhysicalDevice& p_dev : physical_devices) {
		devices_.emplace_back(std::move(std::make_unique<VulkanDevice>(p_dev)));
	}
}

bool RendererBase::checkValidationLayerSupport(const std::vector<char*>& layers) {
	uint32_t layerCount;
	vk::enumerateInstanceLayerProperties(&layerCount, nullptr);

	// Fetch validation layers
	std::vector<vk::LayerProperties> availableLayers(layerCount);
	vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// Check if all requested validation layers are present among the available layers.
	for (const char* layerName : layers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

void RendererBase::setupDebugCallback(const vk::DebugReportFlagsEXT& flags, PFN_vkDebugReportCallbackEXT callback) {
	// Validation must be enabled in order to setup a debug callback.
	if (!vkr::debug_utils::kEnableValidation) {
		return;
	}

	// Generate callback create info
	vk::DebugReportCallbackCreateInfoEXT createInfo{};
	createInfo.flags = flags;
	createInfo.pfnCallback = callback;

	// Setup callback.
	auto setup_callback_fn = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vk_instance_.getProcAddr("vkCreateDebugReportCallbackEXT"));

	// Check if the setup callback function was successfully retrieved.
	if (setup_callback_fn == nullptr) {
		throw std::runtime_error("Failed to set up debug callback!");
	}

	debug_callbacks_.push_back({});
	setup_callback_fn((VkInstance) vk_instance_, &static_cast<VkDebugReportCallbackCreateInfoEXT>(createInfo), nullptr, (VkDebugReportCallbackEXT*) &debug_callbacks_.back());
}

std::vector<std::pair<int32_t, VulkanDevice*>> RendererBase::retrieveDevices(const std::function<int32_t(VulkanDevice*)>& f_score) {
	std::vector<std::pair<int32_t, VulkanDevice*>> devices_p;
	
	// Fetch raw pointers to devices.
	for (auto& it : devices_) {
		int32_t score = f_score(it.get());
		if (score >= 0) {
			devices_p.push_back(std::make_pair(score, it.get()));
		}
	}

	// Sort the pointers based on the score.
	std::sort(devices_p.begin(), devices_p.end(), [](const std::pair<int32_t, VulkanDevice*> &a, const std::pair<int32_t, VulkanDevice*> &b) { return a.first > b.first; });

	return devices_p;
}

RendererBase::~RendererBase() {

	if (vkr::debug_utils::kEnableValidation) {
		// Clear debug callbacks.
		auto destroy_callback_fn = (PFN_vkDestroyDebugReportCallbackEXT)vk_instance_.getProcAddr("vkDestroyDebugReportCallbackEXT");

		// Check if the destroy callback function was successfully retrieved.
		if (destroy_callback_fn == nullptr) {
			throw std::runtime_error("Failed to destroy debug callbacks!");
		}

		for (auto& callback : debug_callbacks_) {
			destroy_callback_fn((VkInstance) vk_instance_, (VkDebugReportCallbackEXT) callback, nullptr);
		}
	}
}

} /// !namespace vkr