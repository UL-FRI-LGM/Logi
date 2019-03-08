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
#include <vector>
#include "logi/commands/CommandPool.h"
#include "logi/queues/Queue.h"

namespace logi {

/**
 * @brief	Contains Nvidia QueueFamily checkpoint properties.
 */
struct NvidiaQueueFamilyCheckpointProperties : public Extension {
    /**
     * @brief	NvidiaQueueFamilyCheckpointProperties default constructor.
     *
     * @param	checkpoint_execution_stage_mask Mask specifying pipeline checkpoint execution.
     */
    explicit NvidiaQueueFamilyCheckpointProperties(const vk::PipelineStageFlags& checkpoint_execution_stage_mask = {});

    /**
     * @brief   Copies NvidiaQueueFamilyCheckpointProperties object and returns unique pointer to it.
     *
     * @return	Unique pointer to the copy. 
     */
    std::unique_ptr<Extension> clone() const override;

    /**
     * @brief	Compares NvidiaQueueFamilyCheckpointProperties structures.
     *
     * @param	rhs Right hand side value.
     * @return	True if the structures contain equal configurations.
     */
    bool operator==(const NvidiaQueueFamilyCheckpointProperties& rhs) const;

    /**
     * Mask specifying pipeline checkpoint execution.
     */
	vk::PipelineStageFlags checkpoint_execution_stage_mask;
};

/**
 * @brief	Contains QueueFamily properties.
 */
struct QueueFamilyProperties : public Extendable {
	/**
     * @brief	QueueFamilyProperties default constructor.
     *
     * @param	family_index		Index of the queue family.
     * @param	properties			Queue family properties.
     */
	QueueFamilyProperties(uint32_t family_index, const vk::QueueFamilyProperties& properties);

	/**
	 * @brief	QueueFamilyProperties default constructor.
	 *
	 * @param	family_index		Index of the queue family.
	 * @param	properties			Queue family properties.
	 */
	QueueFamilyProperties(uint32_t family_index, const vk::QueueFamilyProperties2& properties);

    /**
     * Queue family index.
     */
	uint32_t family_index;

    /**
     * Bit mask indicating capabilities of the queues in this queue family.
     */
	vk::QueueFlags queue_flags;

    /**
     * Number of available queues
     */
	uint32_t queue_count;

    /**
     * Count of meaningful bits in the timestamps.
     */
	uint32_t timestamp_valid_bits;

    /**
     * Minimum granularity supported for image transfer operations.
     */
	vk::Extent3D min_image_transfer_granularity;

protected:
	using Extendable::addExtensions;
};

/**
 * @brief	Structure used to specify QueueFamily configuration during LogicalDevice creation.
 */
struct QueueFamilyConfig : public BuildableExtendable {
	/**
	 * @brief	Populates QueueFamilyConfig with the given values.
	 *
	 * @param	properties	Queue family properties.
	 * @param	queue_count	Number of queues that should be instantiated.
	 * @param	priorities	Vector containing queue priorities.
	 * @param	flags		Specify behavior of the queues.
	 */
	explicit QueueFamilyConfig(QueueFamilyProperties properties, uint32_t queue_count = 0u,
	                           std::vector<float> priorities = {}, const vk::DeviceQueueCreateFlags& flags = {});


	vk::DeviceQueueCreateInfo build() const;

	QueueFamilyProperties properties;	///< Queue family properties.
	uint32_t queue_count;				///< Number of queues that should be instantiated.
	std::vector<float> priorities;		///< Vector containing queue priorities.
  vk::DeviceQueueCreateFlags flags;	///< Specify behavior of the queues.
};


/**
 * @brief QueueFamily handle that is used to access Vulkan queue resources.
 */
class QueueFamily : public Handle {
public:
	QueueFamily();

	/**
	 * @brief Initialize members of QueueFamily and fetches the queues from the device.
	 * 
	 * @param	device			Vulkan logical device handle.
	 * @param	configuration	Queue family configuration.
	 */
	QueueFamily(vk::Device device, const QueueFamilyConfig& configuration);

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

    /**
     * Vulkan logical device handle.
     */
		vk::Device			    vk_device;

    /**
     * Queue family configuration.
     */
		QueueFamilyConfig	  configuration;

    /**
     * Vector of instantiated Queues.
     */
		std::vector<Queue>  queues{};
	};

	std::shared_ptr<QueueFamilyData> data_;	///< QueueFamily internal data
	std::shared_ptr<HandleManager> handle_manager_;	///< Used to manage Queue and CommandPool handles.
};

}

#endif //! QUEUES_QUEUE_MANAGER_H