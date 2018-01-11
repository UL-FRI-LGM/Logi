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
#include "queues/QueueFamily.h"
#include "program_management/ProgramManager.h"
#include "descriptors/DescriptorPool.h"
#include "memory/AllocationManager.h"

namespace vkr {

/**
* @brief Structure used to specify queues configuration during initialization of VulkanDevice.
*/
struct QueueConfig {
	const uint32_t graphic_count;  ///< Number of graphic queues.
	const uint32_t compute_count;  ///< Number of compute queues.
	const uint32_t transfer_count; ///< Number of transfer queues.

	/**
	 * @brief Default constructor.
	 *
	 * @param graphic_count Number of graphic queues.
	 * @param compute_count Number of compute queues.
	 * @param transfer_count Number of transfer queues.
	 */
	QueueConfig(uint32_t graphic_count = 0, uint32_t compute_count = 0, uint32_t transfer_count = 0);
};


/**
 * @brief Wraps Vulkan physical and logical device.
 */
class VulkanDevice {
public:
	friend class ProgramManager;

#pragma region INITIALIZATION
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
#pragma endregion

#pragma region META DATA GETTERS
	/**
	 * @brief Check if the extension is supported by the wrapped physical device.
	 *
	 * @param Name of the extension.
	 * @return True if the extension is supported.
	 */
	bool extensionSupported(const std::string& extension_name) const;

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
	* @brief Get number of dedicated graphical queues.
	* @note Graphical queues also support both compute and transfer operations.
	*
	* @return Number of dedicated graphical queues.
	*/
	uint32_t getGraphicsQueueCount() const;

	/**
	* @brief Get number of dedicated compute queues.
	* @note Compute queues also support transfer operations.
	*
	* @return Number of dedicated graphical queues.
	*/
	uint32_t getComputeQueueCount() const;

	/**
	* @brief Get number of dedicated transfer queues.
	*
	* @return Number of dedicated transfer queues.
	*/
	uint32_t getTransferQueueCount() const;
#pragma endregion

#pragma region QUEUE FAMILIES
	/**
	 * @brief Retrieves dedicated graphic family.
	 *
	 * @return Pointer to the dedicated graphic family or nullptr if the family is not supported.
	 */
	QueueFamily* getGraphicalFamily();

	/**
	 * @brief Retrieves dedicated compute family.
	 *
	 * @return Pointer to the dedicated compute family or nullptr if the family is not supported.
	 */
	QueueFamily* getComputeFamily();

	/**
	 * @brief Retrieves dedicated transfer family.
	 *
	 * @return Pointer to the dedicated transfer family or nullptr if the family is not supported.
	 */
	QueueFamily* getTransferFamily();
#pragma endregion

#pragma region DESCRIPTOR POOL
	/**
	 * @brief Creates descriptor pool. Allocates enough memory to support creation of as many descriptors of a given type as specified in pool_sizes.
	 *
	 * @param pool_sizes Contains number of sets and descriptors of different types.
	 * @param releasable_sets If true the allocated descriptor sets can be released.
	 */
	void createDescriptorPool(const DescriptorsCount& pool_sizes, bool releasable_sets = false);

	/**
	 * @brief Returns pointer to the descriptor pool. Returns nullptr if descriptor pool was not yet created
	 *
	 */
	DescriptorPool* getDescriptorPool();
#pragma endregion

	/**
	 * @brief Check if the device is initialized.
	 */
	bool initialized() const;

	ProgramManager* getProgramManager();

	AllocationManager* getAllocationManager();

	/**
	 * @brief Retrieve physical device handle.
	 *
	 * @return Physical device handle.
	 */
	const vk::PhysicalDevice& getPhysicalDeviceHandle() const;

	/**
	 * @brief Retrieve logical device handle.
	 *
	 * @return Logical device handle.
	 */
	const vk::Device& getLogicalDeviceHandle() const;

	/**
	 * @brief Frees resources.
	 */
	~VulkanDevice();

private:
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

	// Dedicated queue families.
	std::unique_ptr<QueueFamily> graphical_family_;
	std::unique_ptr<QueueFamily> compute_family_;
	std::unique_ptr<QueueFamily> transfer_family_;

	std::unique_ptr<ProgramManager> program_manager_;
	std::unique_ptr<DescriptorPool> descriptor_pool_;
	std::unique_ptr<AllocationManager> allocation_manager_;

	bool initialized_; ///< Is device initialized.
};

} /// !namespace vkr

#endif // !VULKAN_DEVICE_H_