#include "logi/queues/Queue.h"

namespace logi {

Queue::Queue(const QueueFamily& family, const vk::Queue& queue) : OwnedHandle(family, true), queue_(queue) {}

void Queue::submit(const std::vector<vk::SubmitInfo>& submit_infos, const Fence& fence) const {
  checkForNullHandleInvocation("Queue", "submit");
  queue_.submit(submit_infos, fence.getVkHandle());
}

void Queue::submit(const std::vector<vk::SubmitInfo>& submit_infos) const {
  checkForNullHandleInvocation("Queue", "submit");
  queue_.submit(submit_infos, vk::Fence());
}

void Queue::waitIdle() const {
  checkForNullHandleInvocation("Queue", "waitIdle");
  queue_.waitIdle();
}

const vk::Queue& Queue::getVkHandle() const {
  return queue_;
}

} // namespace logi
