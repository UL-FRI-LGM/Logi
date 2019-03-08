/*
* Vulkan Renderer base class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "logi/base/VulkanInstance.h"

namespace logi {

ApplicationInfo::ApplicationInfo(std::string app_name, const uint32_t app_version, std::string engine_name, 
	                             const uint32_t engine_version)
	: app_name(std::move(app_name)), app_version(app_version), engine_name(std::move(engine_name)), 
      engine_version(engine_version) {}

const vk::ApplicationInfo& ApplicationInfo::build() {
	vk_app_info_ = vk::ApplicationInfo(app_name.data(), app_version, engine_name.data(), engine_version, VK_API_VERSION_1_1);
	vk_app_info_.pNext = buildExtensions();

	return vk_app_info_;
}

InstanceConfig::InstanceConfig(ApplicationInfo application_info, std::vector<const char*> extensions,
	                                         std::vector<const char*> validation_layers, 
	                                         const vk::InstanceCreateFlags& flags)
	: application_info(std::move(application_info)), extensions(std::move(extensions)), 
      validation_layers(std::move(validation_layers)), flags(flags) {}

const vk::InstanceCreateInfo& InstanceConfig::build() {
	vk_instance_ci_ = vk::InstanceCreateInfo(flags, &application_info.build(), static_cast<uint32_t>(validation_layers.size()),
		                                     validation_layers.data(), static_cast<uint32_t>(extensions.size()), extensions.data());
	vk_instance_ci_.pNext = buildExtensions();

	return vk_instance_ci_;
}

VulkanInstance::VulkanInstance() : data_(nullptr) {}

VulkanInstance::VulkanInstance(InstanceConfig configuration)
	: data_(std::make_shared<InstanceData>(std::move(configuration))) {

	// Try to create Vulkan instance.
	data_->vk_instance = vk::createInstance(data_->configuration.build());

	// Fetch available physical devices.
	std::vector<vk::PhysicalDevice> physical_devices = data_->vk_instance.enumeratePhysicalDevices();

	// Create Vulkan device handles.
	for (vk::PhysicalDevice& p_dev : physical_devices) {
		data_->physical_devices.emplace_back(data_->handle_manager->createHandle<PhysicalDevice>(p_dev));
	}
}

void VulkanInstance::setupDebugCallback(const vk::DebugReportFlagsEXT& flags, PFN_vkDebugReportCallbackEXT callback) const {

	// Generate callback create info
	VkDebugReportCallbackCreateInfoEXT create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	create_info.flags = static_cast<VkDebugReportFlagsEXT>(flags);
	create_info.pfnCallback = callback;

	// Setup callback.
    const auto setup_callback_fn = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(data_->vk_instance.getProcAddr("vkCreateDebugReportCallbackEXT"));

	// Check if the setup callback function was successfully retrieved.
	if (setup_callback_fn == nullptr) {
		throw InitializationError("Failed to setup debug callback! Check if extension VK_EXT_DEBUG_REPORT_EXTENSION_NAME is enabled.");
	}

	data_->debug_callbacks.emplace_back();
	setup_callback_fn(static_cast<VkInstance>(data_->vk_instance), &create_info, nullptr, reinterpret_cast<VkDebugReportCallbackEXT*>(&data_->debug_callbacks.back()));
}

const InstanceConfig& VulkanInstance::configuration() const {
	return data_->configuration;
}

const std::vector<PhysicalDevice>& VulkanInstance::devices() const {
	return data_->physical_devices;
}

const vk::Instance& VulkanInstance::getVkHandle() const {
	return data_->vk_instance;
}

bool VulkanInstance::checkValidationLayerSupport(const std::vector<const char*>& layers) {
	// Fetch validation layers
	std::vector<vk::LayerProperties> available_layers = vk::enumerateInstanceLayerProperties();

	// Check if all requested validation layers are present among the available layers.
	for (const char* layer : layers) {
		bool found = false;

		for (const auto& layer_properties : available_layers) {
			if (strcmp(layer, layer_properties.layerName) == 0) {
				found = true;
				break;
			}
		}

		if (!found) {
			return false;
		}
	}

	return true;
}

void VulkanInstance::free() {
	if (alive()) {
		data_->handle_manager->destroyAllHandles();

		// Clear debug callbacks.
		const auto destroy_callback_fn = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(data_->vk_instance.getProcAddr("vkDestroyDebugReportCallbackEXT"));

		// Check if the destroy callback function was successfully retrieved.
		if (destroy_callback_fn == nullptr) {
			throw std::runtime_error("Failed to destroy debug callbacks!");
		}

		for (auto& callback : data_->debug_callbacks) {
			destroy_callback_fn(static_cast<VkInstance>(data_->vk_instance), static_cast<VkDebugReportCallbackEXT>(callback), nullptr);
		}

		Handle::free();
	}
}

VulkanInstance::InstanceData::InstanceData(InstanceConfig configuration)
	:  configuration(std::move(configuration)), handle_manager(std::make_shared<HandleManager>()) {}


} /// !namespace logi