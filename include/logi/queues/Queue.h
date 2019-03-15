/*
 *
 *
 * Copyright (C) 2017 by Primoz Lavric
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef QUEUES_QUEUE_H
#define QUEUES_QUEUE_H

#include "logi/base/Handle.h"
#include "logi/synchronization/Fence.h"
#include <vulkan/vulkan.hpp>

namespace logi {

class QueueFamily;

class Queue : public OwnedHandle<QueueFamily> {
 public:
  /**
   * @brief Placeholder constructor.
   */
  Queue() = default;

  /**
   * @brief	Default constructor.
   *
   * @param family  Queue family handle.
   * @param	queue   Queue that will be wrapped.
   */
  explicit Queue(const QueueFamily& family, const vk::Queue& queue);

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

  const vk::Queue& getVkHandle() const;

 private:
  vk::Queue queue_; ///< Vulkan queue handle.
};

} // namespace logi

#endif