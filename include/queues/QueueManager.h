/*
* Descriptor pool
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef QUEUES_QUEUE_MANAGER_H
#define QUEUES_QUEUE_MANAGER_H

#include <vulkan/vulkan.hpp>
#include "synchronization/Fence.h"
#include "synchronization/Semaphore.h"


namespace vkr {

class Queue {
public:
	Queue(const vk::Queue& queue);

	vk::Result submit(const std::vector<Semaphore>& , Fence* fence);

	vk::Result waitIdle();

private:
	vk::Queue queue_;
};


class QueueFamily {
	
public:
	QueueFamily() : family_index(UINT32_MAX), queue_count(0), queues(), command_pool(nullptr) {};
	
private:
	uint32_t family_index; ///< Index of the family.
	uint32_t queue_count; ///< Number of instantiated queues.
	std::vector<vk::Queue> queues; ///< Vulkan queues.
	vk::CommandPool command_pool; ///< Command pool.
	
};


class QueueManager {
public:
	QueueManager(const vk::PhysicalDevice& physical_device);

private:
	vk::PhysicalDevice physical_device_;
	vk::Device logical_device_;
	std::vector<vk::QueueFamilyProperties> queue_family_properties_;
	std::unique_ptr<QueueFamily> dedicated_queues_[3]; ///< Queue family data.
}


#endif //! QUEUES_QUEUE_MANAGER_H