/*
* Vulkan Device class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef BASE_VULKAN_DEVICE_H_
#define BASE_VULKAN_DEVICE_H_

#include <vulkan/vulkan.hpp>
#include <vector>
#include "queues/QueueFamily.h"
#include "program/ProgramManager.h"
#include "descriptors/DescriptorPool.h"
#include "memory/AllocationManager.h"
#include "descriptors/DecriptorsUpdate.h"

namespace logi {

/**
 * @brief	Creates VulkanDevice handle used to manage resources of the given Vulkan physical device.
 */
class VulkanDevice : public DependentDestroyableHandle {
public:
	VulkanDevice();

	/**
	 * @brief	Creates VulkanDevice handle used to manage resources of the given Vulkan physical device.
	 *
	 * @param	device	Vulkan PhysicalDevice handle.
	 */
	VulkanDevice(const std::weak_ptr<HandleManager>& owner, vk::PhysicalDevice& device);

	/**
	 * @brief Initialize Vulkan logical device with the given features and extensions enabled and initialize queues based
	 * on the given configuration.
	 * @throw std::runtime_error if anything goes wrong during initialization.
	 *
	 * @param features Requested features.
	 * @param extensions Requested extensions.
	 * @param queue_counts Queue configuration.
	 */
	void initialize(const vk::PhysicalDeviceFeatures& features, const std::vector<const char*>& extensions, const std::vector<QueueFamilyProperties>& queue_family_configs) const;

	/**
	 * @brief	Retrieve extensions supported by this device.
	 *
	 * @return	Vector of supported extensions.
	 */
	std::vector<vk::ExtensionProperties> supportedExtensions() const;

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
	std::vector<QueueFamilyProperties> queueFamilyProperties(const vk::SurfaceKHR& surface = {}) const;

	/**
	 * @brief	Get queue family with the given index.
	 *
	 * @param	index	Index of the queue family.
	 * @return	QueueFamily handle.
	 */
	const QueueFamily& getQueueFamily(uint32_t index) const;

	/**
	 * @brief Creates descriptor pool. Allocates enough memory to support creation of as many descriptors of a given type as specified in pool_sizes.
	 *
	 * @param pool_sizes Contains number of sets and descriptors of different types.
	 * @param releasable_sets If true the allocated descriptor sets can be released.
	 */
	DescriptorPool createDescriptorPool(const DescriptorsCount& pool_sizes, const vk::DescriptorPoolCreateFlags& flags = {});

	void executeDescriptorsUpdate(const DescriptorsUpdate& update) const;

	/**
	 * @brief Check if the device is initialized.
	 */
	bool initialized() const;

	const ProgramManager& getProgramManager() const;

	const AllocationManager&  getAllocationManager() const;

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
	void free() override;

private:
	struct DeviceData {
		DeviceData(const vk::PhysicalDevice& physical_device);

		vk::PhysicalDevice physical_device;				///< Physical device handle.
		vk::Device logical_device{ nullptr };			///< Logical device handle.
		vk::PhysicalDeviceFeatures enabled_features{};	///< Enabled features.
		std::vector<const char*> enabled_extensions{};	///< Enabled extensions.

		ProgramManager program_manager{};
		AllocationManager allocation_manager{};
		std::vector<QueueFamily> queue_families_{};		///< Vector of queue families.

		bool initialized_{ false };						///< Is device initialized.
	};

	std::shared_ptr<DeviceData> data_;
	std::shared_ptr<HandleManager> handle_manager_;

};

} /// !namespace logi

#endif // !VULKAN_DEVICE_H_
