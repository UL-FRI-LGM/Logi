/*
* Descriptor pool
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef QUEUES_QUEUE_FAMILY_H
#define QUEUES_QUEUE_FAMILY_H

#include <vulkan/vulkan.hpp>
#include "commands/CommandPool.h"
#include "commands/CommandBuffer.h"
#include "synchronization/Fence.h"
#include "synchronization/Semaphore.h"


namespace vkr {

class Queue {
public:
	/**
	 * @brief Default constructor.
	 *
	 * @param queue Queue that will be wrapped.
	 */
	Queue(const vk::Queue& queue);

	/**
	 * @brief Submit the work to the queue.
	 *
	 * @param submit_infos Submit configurations.
	 * @param fence Fence that will be signaled upon completion.
	 */
	void submit(const std::vector<vk::SubmitInfo>& submit_infos, Fence* fence = nullptr);

	/**
	 * @brief Wait for the queue to finish.
	 */
	void waitIdle();

private:
	vk::Queue queue_; ///< Vulkan queue handle.
};


class QueueFamily {

public:
	/**
	 * @brief Initialize members of QueueFamily and fetches the queues from the device.
	 *
	 * @param family_index Queue family index.
	 * @param queue_flags Queue flags.
	 * @param max_queue_count Maximal number of queues.
	 * @param timestamp_valid_bits Number of relevant bits in the timestamp.
	 * @param min_image_transfer_granularity Minimal image transfer granularity.
	 */
	QueueFamily(uint32_t family_index, vk::QueueFlags queue_flags, size_t max_queue_count, uint32_t timestamp_valid_bits, vk::Extent3D min_image_transfer_granularity);

	/**
	 * @brief Initializes queue_count queues.
	 *
	 * @param device Vulkan logical device.
	 * @param queue_count Number of queues to be retrieved.
	 */
	void initialize(const vk::Device& device, uint32_t queue_count);

	/**
	 * @brief Create command pool.
	 * @throws runtime_error if the command pool already exists.
	 *
	 * @param transistent Indicates that the command buffers will be short lived.
	 * @param resetable_buffers Command buffers are allowed to be reset.
	 * @return Pointer to the created CommandPool.
	 */
	CommandPool* createCommandPool(bool transistent, bool resetable_buffers);

	/**
	 * @brief Retrieve command pool
	 *
	 * @return Pointer to the CommandPool or null pointer if the pool does not exist.
	 */
	CommandPool* getCommandPool();

	/**
	 * @brief Retrieve queue family index.
	 *
	 * @return Queue family index.
	 */
	uint32_t getFamilyIndex() const;

	/**
	 * @brief Retrieve the number of relevant bits in the timestamp.
	 *
	 * @return Number of relevant bits in the timestamp.
	 */
	uint32_t getTimestampValidBits() const;

	/**
	 * @brief Retrieve minimal image transfer granularity.
	 *
	 * @return Minimal image transfer granularity.
	 */
	const vk::Extent3D& getMinImageTransferGranularity() const;

	/**
	 * @brief Get maximal supported number of queues.
	 *
	 * @return Maximal number of supported queues.
	 */
	const uint32_t getMaxSupportedQueueCount() const;

	/**
	 * @brief Returns number of created queues.
	 *
	 * @return Number of queues.
	 */
	size_t queueCount();

	/**
	 * @brief Retrieve pointer to the queue with the given index.
	 *
	 * @return Pointer to the queue.
	 */
	Queue* getQueue(size_t index);

	/**
	 * @brief Free resources.
	 */
	~QueueFamily();

private:
	const vk::QueueFlags queue_flags_; ///< Operations supported by the queue.
	const uint32_t max_queue_count_; ///< Maximal supported number of queues.
	const uint32_t family_index_; ///< Queue family index.
	const uint32_t timestamp_valid_bits_; ///< Number of valid bits in the timestamp.
	const vk::Extent3D min_image_transfer_granularity_; ///< Minimum image transfer granularity.
	
	vk::Device device_; ///< Logical device.
	bool initialized_; ///< Is queue family initialized. If not it contains only meta data.

	std::vector<std::unique_ptr<Queue>> queues_; ///< Vulkan queues.
	std::unique_ptr<CommandPool> command_pool_; ///< Command pool.
};

}

#endif //! QUEUES_QUEUE_MANAGER_H