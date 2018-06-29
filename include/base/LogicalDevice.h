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
#include "base/SwapChain.h"
#include "memory/Sampler.h"

namespace logi {

/**
 * @brief	Creates VulkanDevice handle used to manage resources of the given Vulkan physical device.
 */
class LogicalDevice : public DependentDestroyableHandle {
public:
	LogicalDevice();

	/**
	 * @brief	Creates VulkanDevice handle used to manage resources of the given Vulkan physical device.
	 *
	 * @param	owner			Handle owner.
	 * @param	device			Vulkan PhysicalDevice handle.
	 * @param	qfamily_configs	Vector containing configurations for queue families.
	 * @param	extensions		Requested extensions.
	 * @param	features		Requested features.
	 */
	LogicalDevice(const std::weak_ptr<HandleManager>& owner, vk::PhysicalDevice& device, std::vector<QueueFamilyConfig>& qfamily_configs, const std::vector<const char*>& extensions, const vk::PhysicalDeviceFeatures& features);

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
	DescriptorPool createDescriptorPool(const DescriptorsCount& pool_sizes, const vk::DescriptorPoolCreateFlags& flags = {}) const;

	void executeDescriptorsUpdate(const DescriptorsUpdate& update) const;

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

	SwapChain createSwapchain(const vk::SurfaceKHR& surface, uint32_t present_family, const std::vector<uint32_t>& concurrent_families = {}) const;

	Semaphore createSemaphore(const vk::SemaphoreCreateFlags& flags = {}) const;

	Fence createFence(const vk::FenceCreateFlags& flags = {}) const;

	void waitIdle() const;

	vk::Result waitForFences(const std::vector<Fence>& fences, bool wait_all, uint64_t timeout) const;

	Sampler createSampler(const SamplerConfiguration& config) const;

protected:
	/**
	 * @brief Frees resources.
	 */
	void free() override;

private:
	struct DeviceData {
		DeviceData(const vk::PhysicalDevice& physical_device, const std::vector<const char*>& enabled_extensions, const vk::PhysicalDeviceFeatures& enabled_features);

		vk::PhysicalDevice physical_device;				///< Physical device handle.
		vk::Device logical_device{ nullptr };			///< Logical device handle.
		std::vector<const char*> enabled_extensions{};	///< Enabled extensions.
		vk::PhysicalDeviceFeatures enabled_features{};	///< Enabled features.

		ProgramManager program_manager{};
		AllocationManager allocation_manager{};
		std::vector<QueueFamily> queue_families_{};		///< Vector of queue families.
	};

	std::shared_ptr<DeviceData> data_;
	std::shared_ptr<HandleManager> handle_manager_;
};

} /// !namespace logi

#endif // !VULKAN_DEVICE_H_
