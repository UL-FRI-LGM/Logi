/*
* Project       : VulkanRenderer
*
* Author        : Primoz Lavric
*
* For the terms and conditions of redistribution or use of this software
* refer to the file LICENSE located in the root level directory
* of the project.
*/

#include "vk_instance_manager.h"
#include <vulkan/vulkan.hpp>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include "globals.h"

namespace vkr {

VulkanInstanceManager::VulkanInstanceManager(std::vector<char *> extension_names) {
	// Declare application info using the data from globals.
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = globals::kAppName;
	appInfo.applicationVersion = globals::kAppVersion;
	appInfo.pEngineName = globals::kEngineName;
	appInfo.engineVersion = globals::kEngineVersion;
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// Initialize requested extensions.
	// Add debug report extension if the validation is enabled.
	if (globals::kEnableValidation) {
		extension_names.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extension_names.size());
	createInfo.ppEnabledExtensionNames = extension_names.data();

	// Check if requested validation layers are supported.
	if (globals::kEnableValidation && !checkValidationLayerSupport(globals::kValidationLayers)) {
		throw std::runtime_error("Requested validation layers are not supported!");
	}

	// Setup validation layers if enabled.
	if (globals::kEnableValidation) {
		createInfo.ppEnabledLayerNames = globals::kValidationLayers.data();
		createInfo.enabledLayerCount = static_cast<uint32_t>(globals::kValidationLayers.size());
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	// Try to create instance.
	if (vkCreateInstance(&createInfo, nullptr, &vk_instance_) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create instance!");
	}

	// If validation is enabled also set up the debug callback.
	if (globals::kEnableValidation) {
		setupDebugCallback();
	}
}

VulkanInstanceManager::~VulkanInstanceManager() {
	// Clear debug callback if validation is enabled.
	if (globals::kEnableValidation) {
		auto delete_debug_fn = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(vk_instance_, "vkDestroyDebugReportCallbackEXT");

		if (delete_debug_fn != nullptr) {
			delete_debug_fn(vk_instance_, callback, nullptr);
		}
	}

	vkDestroyInstance(vk_instance_, nullptr);
}

bool VulkanInstanceManager::checkValidationLayerSupport(const std::vector<char*> layers) {
	size_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	// Fetch validation layers
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

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

void VulkanInstanceManager::setupDebugCallback() {
	// Validation must be enabled in order to setup debug callback.
	if (!globals::kEnableValidation) {
		return;
	}

	// Generate callback create info
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = globals::kDebugFlags;
	createInfo.pfnCallback = debugCallback;

	// Try to setup callback.
	auto setup_debug_fn = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(vk_instance_, "vkCreateDebugReportCallbackEXT");

	if (setup_debug_fn != nullptr) {
		setup_debug_fn(vk_instance_, &createInfo, nullptr, &callback);
	}
	else {
		throw std::runtime_error("Failed to set up debug callback!");
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstanceManager::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
	std::cerr << "validation layer: " << msg << std::endl;

	return VK_FALSE;
}

}