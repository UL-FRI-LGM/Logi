/*
* Vulkan Instance class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef RENDERER_BASE_H
#define RENDERER_BASE_H

#include <vulkan/vulkan.hpp>
#include <memory>
#include <vector>
#include <functional>
#include "base/PhysicalDevice.h"

namespace logi {

/**
 * @brief	Contains information about engine and application.
 */
struct ApplicationInfo : public BuildableExtendable {
    /**
	 * @brief   Initialize ApplicationInfo data.
	 *
	 * @param	app_name        Application name.
	 * @param	app_version     Application version.
	 * @param	engine_name     Engine name.
	 * @param	engine_version  Engine version.
	 */
    explicit ApplicationInfo(std::string app_name = "", uint32_t app_version = 0u, std::string engine_name = "",
		                     uint32_t engine_version = 0u);

    /**
	 * @brief	Build Vulkan ApplicationInfo structure using the structure members and return a pointer to it.
	 *
	 * @return	Reference to built Vulkan ApplicationInfo structure.
	 */
	const vk::ApplicationInfo& build();

    /**
     * Application name.
     */
	std::string app_name;

    /**
     * Application version.
     */
	uint32_t app_version;

    /**
     * Engine name.
     */
	std::string engine_name;

    /**
     * Engine version.
     */
	uint32_t engine_version;

private:
    /**
     * Vulkan application info structure.
     */
	vk::ApplicationInfo vk_app_info_;
};


/**
 * @brief	Contains instance configuration. Used to initialize VulkanInstance.
 */
struct InstanceConfiguration : public BuildableExtendable {
    /**
     * @brief	Initialize InstanceConfiguration data.
     *
     * @param   application_info    ApplicationInfo structure.
     * @param	extensions          Array specifying which extensions should be enabled. 
     * @param	validation_layers   Array specifying which validation layers should be enabled.
     * @param   flags               Flags specifying additional configuration.
     */
	explicit InstanceConfiguration(ApplicationInfo application_info = ApplicationInfo(),
		                           std::vector<const char*> extensions = {},
		                           std::vector<const char*> validation_layers = {},
	                               const vk::InstanceCreateFlags& flags = {});

    /**
	 * @brief	Build Vulkan InstanceCreateInfo structure using the structure members and return a pointer to it.
	 *
	 * @return	Reference to built Vulkan InstanceCreateInfo structure.
	 */
	const vk::InstanceCreateInfo& build();

    /**
     * ApplicationInfo structure.
     */
	ApplicationInfo application_info;

    /**
	 * Array specifying which extensions should be enabled.
	 */
	std::vector<const char*> extensions;

	/**
     * Array specifying which validation layers should be enabled.
     */
	std::vector<const char*> validation_layers;

	/**
	 * Flags specifying additional configuration.
	 */
	vk::InstanceCreateFlags flags;

private:
	/**
     * Vulkan instance create info structure.
     */
	vk::InstanceCreateInfo vk_instance_ci_;
};


/**
 * @brief	Vulkan instance handle.
 */
class VulkanInstance : public DestroyableHandle {
public:
    /**
	 * @brief	Construct a placeholder handle.
	 */
	VulkanInstance();

    /**
	 * @brief	Construct and initialize VulkanInstance handle.
	 *
	 * @param	configuration   Vulkan instance configuration.
	 */
    explicit VulkanInstance(InstanceConfiguration configuration);

    /**
	 * @brief	Register debug report callback. Debug report callbacks give more detailed feedback on the application’s 
	 *          use of Vulkan when events of interest occur.
	 *
	 * @param	flags       Flags that define conditions under which this callback will be called.
	 * @param	callback    Pointer to the callback function.
	 */
	void setupDebugCallback(const vk::DebugReportFlagsEXT& flags, PFN_vkDebugReportCallbackEXT callback) const;

    /**
	 * @brief	Retrieve configuration that was used to configure the instance.
	 *
	 * @return	Instance configuration.
	 */
	const InstanceConfiguration& configuration() const;

    /**
     * @brief   Retrieve vector of physical devices.
     * 
     * @return  Vector of physical devices.
     */
	const std::vector<PhysicalDevice>& devices() const;

    /**
	 * @brief	Retrieve Vulkan instance handle.
	 *
	 * @return	Vulkan instance handle.
	 */
	const vk::Instance& getVkHandle() const;

	/**
	* @brief Check if the given validation layers are supported.
	*
	* @param layers Vector containing names of validation layers
	* @return True if all layers are supported.
	*/
	static bool checkValidationLayerSupport(const std::vector<const char*>& layers);

protected:
    /**
	 * @brief   Free the instance resources.
	 */
	void free() override;

private:
	struct InstanceData {
	    explicit InstanceData(InstanceConfiguration configuration);

		InstanceConfiguration configuration;
		vk::Instance vk_instance{};
		std::vector<PhysicalDevice> physical_devices{};
		std::vector<vk::DebugReportCallbackEXT> debug_callbacks{};
		std::shared_ptr<HandleManager> handle_manager;
	};

	std::shared_ptr<InstanceData> data_;
};

}

#endif // !RENDERER_BASE_H