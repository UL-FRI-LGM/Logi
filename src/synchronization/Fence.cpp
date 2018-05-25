#include "synchronization/Fence.h"

namespace vkr {

Fence::Fence(const std::shared_ptr<ManagedVkFence>& vk_fence) : vk_fence_(vk_fence) {}

bool Fence::getStatus() const {
	return vk_fence_->getOwner().getFenceStatus(vk_fence_->get()) == vk::Result::eSuccess;
}

void Fence::reset() const {
	vk_fence_->getOwner().resetFences({ vk_fence_->get() });
}

vk::Result Fence::wait(uint64_t timeout) const {
	return vk_fence_->getOwner().waitForFences({ vk_fence_->get() }, true, timeout);
}

vk::Result Fence::wait(const std::vector<Fence>& fences, bool wait_all, uint64_t timeout) {
	std::vector<vk::Fence> vk_fences{};
	vk_fences.reserve(fences.size());

	for (auto it = fences.begin(); it != fences.end(); ++it) {
		vk_fences.emplace_back((*it).getVkHandle());
	}

	return fences[0].vk_fence_->getOwner().waitForFences(vk_fences, true, timeout);
}

const vk::Fence& Fence::getVkHandle() const {
	return vk_fence_->get();
}

void Fence::destroy() const {
	vk_fence_->destroy();
}

}
