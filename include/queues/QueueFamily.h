/*
* 
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef QUEUES_QUEUE_FAMILY_H
#define QUEUES_QUEUE_FAMILY_H

#include <vulkan/vulkan.hpp>
#include "commands/CommandPool.h"
#include "queues/Queue.h"
#include <vector>

namespace logi {

/**
 * @brief	Contains QueueFamily properties.
 */
struct QueueFamilyProperties {
	/**
	 * @brief	QueueFamilyProperties default constructor.
	 *
	 * @param	family_index		Index of the queue family.
	 * @param	properties			Queue family properties.
	 */
	QueueFamilyProperties(uint32_t family_index, const vk::QueueFamilyProperties& properties);

	const uint32_t family_index;						///< Index of the queue family.
	const vk::QueueFlags queue_flags;					///< Bitmask indicating capabilities of the queues in this queue family.
	const uint32_t max_queue_count;						///< Number of available queues.
	const uint32_t timestamp_valid_bits;				///< Count of meaningful bits in the timestamps.
	const vk::Extent3D min_image_transfer_granularity;	///< Minimum granularity supported for image transfer operations.
};

/**
 * @brief	Structure used to specify QueueFamily configuration during LogicalDevice creation.
 */
struct QueueFamilyConfig {
	/**
	 * @brief	Populates QueueFamilyConfig with the given values.
	 *
	 * @param	properties	Queue family properties.
	 * @param	queue_count	Number of queues that should be instantiated.
	 * @param	priorities	Vector containing queue priorities.
	 * @param	flags		Specify behaviour of the queues.
	 */
	explicit QueueFamilyConfig(const QueueFamilyProperties& properties, uint32_t queue_count = 0u, const std::vector<float>& priorities = {}, const vk::DeviceQueueCreateFlags& flags = {});

	QueueFamilyProperties properties;	///< Queue family properties.
	vk::DeviceQueueCreateFlags flags;	///< Specify behaviour of the queues.
	uint32_t queue_count;				///< Number of queues that should be instantiated.
	std::vector<float> priorities;		///< Vector containing queue priorities.
};


/**
 * @brief QueueFamily handle that is used to access Vulkan queue resources.
 */
class QueueFamily : public Handle {
public:
	/**
	 * @brief Initialize members of QueueFamily and fetches the queues from the device.
	 * 
	 * @param	device			Vulkan logical device handle.
	 * @param	configuration	Queue family configuration.
	 */
	QueueFamily(const vk::Device device, const QueueFamilyConfig& configuration);

	/**
	 * @brief	Create Vulkan CommandPool for the QueueFamily and retrieve CommandPool handle.
	 *
	 * @param	flags	Flags indicating usage behavior for the pool and command buffers allocated from it.
	 * @return	CommandPool handle.
	 */
	CommandPool createCommandPool(const vk::CommandPoolCreateFlags& flags = {})  const;

	/**
	 * @brief	Retrieve QueueFamily properties.
	 *
	 * @return	QueueFamily configuration.
	 */
	const QueueFamilyConfig& configuration() const;

	/**
	 * @brief	Retrieve handle for the queue with the given index.
	 *
	 * @return	Queue handle.
	 */
	Queue getQueue(size_t index) const;

protected:
	/**
	 * @brief	Free resources.
	 */
	void free() override;

private:
	struct QueueFamilyData {
		QueueFamilyData(const vk::Device& vk_device, const QueueFamilyConfig& config);

		vk::Device			vk_device;			///< Vulkan logical device handle.
		QueueFamilyConfig	configuration;		///< Queue family configuration.
		std::vector<Queue>	queues{};			///< Vector of instantiated Queues.
	};

	std::shared_ptr<QueueFamilyData> data_;	///< QueueFamily internal data
	std::shared_ptr<HandleManager> handle_manager_;	///< Used to manage Queue and CommandPool handles.
};

}

#endif //! QUEUES_QUEUE_MANAGER_H