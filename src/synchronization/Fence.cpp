#include "..\..\include\synchronization\Fence.h"

namespace vkr {

Fence::Fence(const vk::Device& device, bool initialy_signaled) : device_(device) {
	vk::FenceCreateInfo fence_ci{};
	
	if (initialy_signaled) {
		fence_ci.flags = vk::FenceCreateFlagBits::eSignaled;
	}

	fence_ = device_.createFence(fence_ci);
}

bool Fence::getStatus() {
	return device_.getFenceStatus(fence_) == vk::Result::eSuccess;
}

void Fence::reset() {
	device_.resetFences({ fence_ });
}

vk::Result Fence::wait(uint64_t timeout) {
	return device_.waitForFences({ fence_ }, true, timeout);
}

vk::Result Fence::wait(const std::vector<Fence*>& fences, bool wait_all, uint64_t timeout) {
	std::vector<vk::Fence> vk_fences{};
	vk_fences.reserve(fences.size());

	for (auto it = fences.begin(); it != fences.end(); it++) {
		vk_fences.emplace_back((*it)->getVkHandle());
	}

	return fences[0]->device_.waitForFences(vk_fences, true, timeout);
}

const vk::Fence& Fence::getVkHandle() const {
	return fence_;
}

Fence::~Fence() {
	device_.destroyFence(fence_);
}

}