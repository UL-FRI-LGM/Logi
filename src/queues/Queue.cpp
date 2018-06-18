#include "queues/Queue.h"

namespace logi {

Queue::Queue(const vk::Queue& queue) : queue_(queue) {}

void Queue::submit(const std::vector<vk::SubmitInfo>& submit_infos, const Fence& fence) const {
	if (!alive()) {
		throw std::runtime_error("Called 'submit' on destroyed queue.");
	}

	queue_.submit(submit_infos, fence.getVkHandle());
}

void Queue::submit(const std::vector<vk::SubmitInfo>& submit_infos) const {
	if (!alive()) {
		throw std::runtime_error("Called 'submit' on destroyed queue.");
	}

	queue_.submit(submit_infos, vk::Fence());
}

void Queue::waitIdle() const {
	if (!alive()) {
		throw std::runtime_error("Called 'waitIdle' on destroyed queue.");
	}

	queue_.waitIdle();
}

const vk::Queue& Queue::getVkHandle() const {
	return queue_;
}


}
