/*
*
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef QUEUES_QUEUE_H
#define QUEUES_QUEUE_H

#include <vulkan/vulkan.hpp>

#include "base/Handle.h"
#include "synchronization/Fence.h"

namespace logi {

class Queue : public Handle {
public:
	/**
	* @brief	Default constructor.
	*
	* @param	queue	Queue that will be wrapped.
	*/
	explicit Queue(const vk::Queue& queue);

	/**
	* @brief	Submit the work to the queue.
	*
	* @param	submit_infos	Submit configurations.
	* @param	fence			Fence that will be signaled upon completion.
	*/
	void submit(const std::vector<vk::SubmitInfo>& submit_infos, const Fence& fence) const;

	/**
	* @brief	Submit the work to the queue.
	*
	* @param	submit_infos	Submit configurations.
	*/
	void submit(const std::vector<vk::SubmitInfo>& submit_infos) const;

	/**
	* @brief	Wait for the queue to finish.
	*/
	void waitIdle() const;

private:
	vk::Queue queue_;	///< Vulkan queue handle.
};

}

#endif