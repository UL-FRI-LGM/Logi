/*
* Vulkan Physical Device class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef BASE_PHYSICAL_DEVICE_H_
#define BASE_PHYSICAL_DEVICE_H_

#include <vulkan/vulkan.hpp>
#include "queues/QueueFamily.h"
#include "base/Handle.h"
#include "base/LogicalDevice.h"

namespace logi {

class PhysicalDevice : public Handle {
public:
	/**
	 * @brief	Default constructor for placeholder object.
	 */
	PhysicalDevice();

	/**
	 * @brief	Constructs PhysicalDevice handle that wraps 
	 *
	 * @param	vk_physical_device	
	 */
    explicit PhysicalDevice(const vk::PhysicalDevice& vk_physical_device);

	/**
	 * @brief	Retrieve device properties.
	 *
	 * @return	Device properties.
	 */
	vk::PhysicalDeviceProperties properties() const;

	/**
	 * @brief	Retrieve device features.
	 *
	 * @return	Device features.
	 */
	vk::PhysicalDeviceFeatures features() const;

	/**
	 * @brief	Retrieve extensions supported by this device.
	 *
	 * @return	Vector of supported extensions.
	 */
	std::vector<vk::ExtensionProperties> supportedExtensions() const;

	/**
	 * @brief	Retrieve device memory properties.
	 *
	 * @return	Device memory properties.
	 */
	vk::PhysicalDeviceMemoryProperties memoryProperties() const;

	/**
	 * @brief	Retrieve configurable queue family properties.
	 *
	 * @return	Vector containing configurable queue family properties for all queue families.
	 */
	std::vector<QueueFamilyProperties> queueFamilyProperties() const;

	/**
	 * @brief	Check if queue family with the given index supports presentation on the given surface.	
	 *
	 * @return	True if presentation is supported.
	 */
	bool supportsSurfacePresent(const vk::SurfaceKHR& surface, uint32_t family_index) const;

	/**
	 * @brief	Check if queue family with the given properties supports presentation on the given surface.
	 *
	 * @return	True if presentation is supported.
	 */
	bool supportsSurfacePresent(const vk::SurfaceKHR& surface, const QueueFamilyProperties& family_properties) const;

	LogicalDevice createLogicalDevice(std::vector<QueueFamilyConfig>& qfamily_configs, const std::vector<const char*>& extensions = {}, const vk::PhysicalDeviceFeatures& features = {}) const;

	// TODO Implement free.
private:
	std::shared_ptr<vk::PhysicalDevice> vk_physical_device_;	///< Physical device handle.
	std::shared_ptr<HandleManager> handle_manager_;				///< LogicalDevice handle manager.
};


}


#endif
