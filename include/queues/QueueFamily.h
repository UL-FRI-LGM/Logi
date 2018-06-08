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
#include <optional>

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
	 * @param	supports_present	If true QueueFamily can present on the tested surface.
	 */
	QueueFamilyProperties(uint32_t family_index, const vk::QueueFamilyProperties& properties, bool supports_present = false);

	/**
	 * @brief	Specifies queue family configuration for VulkanDevice initialization.
	 *
	 * @param	queue_count			Number of queues that should be created.
	 * @param	create_flags		Bitmask indicating the behavior of the queues.
	 * @param	queue_priorities	Vector of normalized floating point values, specifying priorities of work that will be submitted to each created queue.
	 */
	void configure(uint32_t queue_count, const vk::DeviceQueueCreateFlags& create_flags = {}, std::optional<const std::vector<float>> queue_priorities = {});

	const uint32_t family_index;						///< Index of the queue family.
	const vk::QueueFlags queue_flags;					///< Bitmask indicating capabilities of the queues in this queue family.
	const uint32_t max_queue_count;						///< Number of available queues.
	const uint32_t timestamp_valid_bits;				///< Count of meaningful bits in the timestamps.
	const vk::Extent3D min_image_transfer_granularity;	///< Minimum granularity supported for image transfer operations.
	const bool supports_present;						///< If true QueueFamily can present on the tested surface.

	vk::DeviceQueueCreateFlags create_flags{};			///< Bitmask indicating the behavior of the queues.
	uint32_t queue_count{ 0u };							///< Number of queues. Should be less than max_queue_count.
	std::vector<float> queue_priorities{};				///< Vector of normalized floating point values, specifying priorities of work that will be submitted to each created queue.
};


/**
 * @brief QueueFamily handle that is used to access Vulkan queue resources.
 */
class QueueFamily : public Handle {
public:
	/**
	 * @brief Initialize members of QueueFamily and fetches the queues from the device.
	 * 
	 * @param	device		Vulkan logical device handle.
	 * @param	properties	Queue family properties.
	 */
	QueueFamily(const vk::Device device, const QueueFamilyProperties& properties);

	/**
	 * @brief	Create Vulkan CommandPool for the QueueFamily and retrieve CommandPool handle.
	 *
	 * @param	flags	Flags indicating usage behavior for the pool and command buffers allocated from it.
	 * @return	CommandPool handle.
	 */
	CommandPool createCommandPool(const vk::CommandPoolCreateFlags& flags) const;

	/**
	 * @brief	Retrieve QueueFamily properties.
	 *
	 * @return	QueueFamily configuration.
	 */
	const QueueFamilyProperties& properties() const;

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
		QueueFamilyData(const vk::Device& vk_device, const QueueFamilyProperties& configuration);

		vk::Device				vk_device;			///< Vulkan logical device handle.
		QueueFamilyProperties	configuration;		///< Queue family configuration.
		std::vector<Queue>		queues{};			///< Vector of instantiated Queues.
		HandleManager			handle_manager{};	///< Used to manage Queue and CommandPool handles.
	};

	std::shared_ptr<QueueFamilyData> data_;	///< QueueFamily internal data.
};

}

#endif //! QUEUES_QUEUE_MANAGER_H