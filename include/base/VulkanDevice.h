/*
* Vulkan Device class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef VULKAN_DEVICE_H_
#define VULKAN_DEVICE_H_

#include <vulkan\vulkan.hpp>
#include <vector>
#include <set>
#include <map>
#include <string>

namespace vkr {

/**
 * @brief Structure used to specify queues configuration during initialization of VulkanDevice.
 */
struct QueueConfig {
	const uint32_t graphic_count; ///< Number of graphic queues. If there is not enough graphic queues available, exception will be thrown.
	const uint32_t compute_count; ///< Number of compute queues. If there is not enough compute queues available, graphic queues will be used.
	const bool dedicated_transfer; ///< If true, a dedicated transfer queue will be used.

	/**
	 * @brief Do not allow usage of empty constructor.
	 */
	QueueConfig() = delete;

	/**
	 * @brief Default constructor.
	 *
	 * @param graphic_count Number of graphic queues. If there is not enough graphic queues available, exception will be thrown.
	 * @param compute_count Number of compute queues. If there is not enough compute queues available, graphic queues will be used.
	 * @param dedicated_transfer If true, a dedicated transfer queue will be used.
	 */
	QueueConfig(uint32_t graphic_count, uint32_t compute_count, bool dedicated_transfer)
		: graphic_count(graphic_count), compute_count(compute_count), dedicated_transfer(dedicated_transfer) {
	}
};

/**
 * @brief Used to address specific VulkanDevice QueueFamily.
 */
enum class QueueFamilyType {
	GRAPHIC = 0,
	COMPUTE = 1,
	TRANSFER = 2
};


/**
 * @brief Wraps Vulkan physical and logical device.
 */
class VulkanDevice {
public:
	friend class ProgramManager;

	/**
	 * @brief Default constructor. Wraps the given physical device and queries its properties.
	 *
	 * @param device Physical device that is to be used.
	 */
	VulkanDevice(vk::PhysicalDevice& device);

	/**
	 * @brief Initialize Vulkan logical device with the given features and extensions enabled and initialize queues based
	 * on the given configuration.
	 * @throw std::runtime_error if anything goes wrong during initialization.
	 *
	 * @param features Requested features.
	 * @param extensions Requested extensions.
	 * @param queue_config Queue configuration.
	 */
	void initialize(const vk::PhysicalDeviceFeatures& features, const std::vector<char*>& extensions, const QueueConfig& queue_config);

	/**
	 * @brief Check if the extension is supported by the wrapped physical device.
	 *
	 * @param Name of the extension.
	 * @return True if the extension is supported.
	 */
	bool extensionSupported(const std::string& extension_name) const;

	/**
	* @brief Check if device supports the family of given type.
	*
	* @param type Queue family type.
	* @return True if the family is supported;
	*/
	bool supportsQueueFamily(const QueueFamilyType type) const;

	/**
	 * @brief Fetch number of queues that belong to the family of given type.
	 *
	 * @param type Queue family type.
	 * @return Number of queues.
	 */
	uint32_t getQueueCount(const QueueFamilyType type) const;

	/**
	 * @brief Retrieve queue family properties.
	 *
	 * @return Vector containing queue family properties structures.
	 */
	const std::vector<vk::QueueFamilyProperties>& getQueueFamilyProperties() const;

	/**
	 * @brief Retrieve physical device handle.
	 *
	 * @return Physical device handle.
	 */
	vk::PhysicalDevice getPhysicalDeviceHandle() const;
	
	/**
	 * @brief Retrieve logical device handle.
	 *
	 * @return Logical device handle.
	 */
	vk::Device getLogicalDeviceHandle() const;

	/**
	 * @brief Retrieve index of the family of the given type.
	 *
	 * @return Index of the queue family or UINT32_MAX if the queue of the given type is not supported.
	 */
	uint32_t getFamilyIndex(QueueFamilyType type) const;

	/**
	 * @brief Retrieve device base properties.
	 *
	 * @return Device base properties.
	 */
	const vk::PhysicalDeviceProperties& properties() const;

	/**
	 * @brief Retrieve device features.
	 *
	 * @return Device features.
	 */
	const vk::PhysicalDeviceFeatures& features() const;

	/**
	 * @brief Retrieve device memory properties.
	 *
	 * @return Device memory properties.
	 */
	const vk::PhysicalDeviceMemoryProperties& memoryProperties() const;

	/**
	 * @brief Check if the device is initialized.
	 */
	bool initialized() const;

	~VulkanDevice();

protected:
	/**
	 * @brief Generate Vulkan queue create infos based on the give queue_config configuration.
	 * @throw std::runtime_error if bad configuration is given.
	 *
	 * @param queue_config Queue configuration structure.
	 * @return Vector containing queue create infos.
	 */
	std::vector<vk::DeviceQueueCreateInfo> generateQueueCreateInfos(const QueueConfig& queue_config);

	/**
	 * @brief Create CommandPool for the queue family with the given family index.
	 * @throw std::runtime_error if CommandPool creation fails.
	 *
	 * @param queue_config Queue configuration structure.
	 */
	vk::CommandPool createCommandPool(const uint32_t family_index, const vk::CommandPoolCreateFlags flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

private:
	/**
	 * @brief Holds Vulkan queue family related information.
	 */
	struct QueueFamily {
		uint32_t family_index; ///< Index of the family.
		uint32_t queue_count; ///< Number of instantiated queues.
		std::vector<vk::Queue> queues; ///< Vulkan queues.
		vk::CommandPool command_pool; ///< Command pool.

		/**
		 * @brief Default constructor.
		 */
		QueueFamily() : family_index(UINT32_MAX), queue_count(0), queues(), command_pool(nullptr) {};
	};

	const float kDefaultQueuePriority = 0.0f;

	// Vulkan device handles.
	vk::PhysicalDevice physical_device_; ///< Physical device handle.
	vk::Device logical_device_; ///< Logical device handle.

	// Device properties.
	vk::PhysicalDeviceProperties device_properties_; ///< Structure specifying physical device properties.
	vk::PhysicalDeviceFeatures device_features_; ///< Structure describing the fine-grained features that can be supported by an implementation.
	vk::PhysicalDeviceMemoryProperties memory_properties_; ///< Structure specifying physical device memory properties.
	std::set<std::string> available_extensions_; ///< List of supported extensions.

	// Enabled features.
	vk::PhysicalDeviceFeatures enabled_features_; ///< Features enabled during logical device creation.

	// Queue families.
	std::vector<vk::QueueFamilyProperties> queue_family_properties_; ///< Queue family properties structures.
	QueueFamily queue_families_[3]; ///< Queue family data.

	// List of cached shaders.
	std::vector<vk::ShaderModule> cached_shaders_;

	bool initialized_; ///< Is device initialized.

	/**
	 * @brief Create the shader module using the given code and caches it.
	 *
	 * @param code Shader code.
	 * @return size_t Index of the shader module (this index is used to fetch the shader).
	 */
	size_t createShaderModule(const std::vector<uint32_t>& code);

	/**
	 * @brief Retrieve the shader module with the given index. If the shader module is not found it returns a null handle.
	 *
	 * @param index Shader module index.
	 * @return Shader module.
	 */
	vk::ShaderModule getShaderModule(size_t index);

	/**
	 * @brief Destroy all cached shader modules (this invalidate all indices previously outputted by createShaderModule).
	 */
	void clearShaderModules();
};

} /// !namespace vkr

#endif // !VULKAN_DEVICE_H_