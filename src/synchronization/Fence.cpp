#include "synchronization/Fence.h"

namespace logi {

Fence::Fence() : DependentDestroyableHandle({}, false), vk_fence_(nullptr) {}

Fence::Fence(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const vk::FenceCreateFlags& flags) : DependentDestroyableHandle(owner), vk_fence_(nullptr) {
	vk::FenceCreateInfo fence_ci{};
	fence_ci.flags = flags;

	vk_fence_ = std::make_shared<ManagedVkFence>(device, device.createFence(fence_ci));
}

bool Fence::getStatus() const {
	if (!alive()) {
		throw std::runtime_error("Called 'getStatus' on destroyed Fence object.");
	}

	return vk_fence_->getOwner().getFenceStatus(vk_fence_->get()) == vk::Result::eSuccess;
}

void Fence::reset() const {
	if (!alive()) {
		throw std::runtime_error("Called 'reset' on destroyed Fence object.");
	}

	vk_fence_->getOwner().resetFences({ vk_fence_->get() });
}

vk::Result Fence::wait(const uint64_t timeout) const {
	if (!alive()) {
		throw std::runtime_error("Called 'wait' on destroyed Fence object.");
	}

	return vk_fence_->getOwner().waitForFences({ vk_fence_->get() }, true, timeout);
}

/*vk::Result Fence::wait(const std::vector<Fence>& fences, const bool wait_all, const uint64_t timeout) {
	std::vector<vk::Fence> vk_fences{};
	vk_fences.reserve(fences.size());

	for (auto it = fences.begin(); it != fences.end(); ++it) {
		vk_fences.emplace_back((*it).getVkHandle());
	}

	vk_fence_->getOwner().waitForFences()
	return vk_fence_->getOwner().waitForFences(vk_fences, wait_all, timeout);
}*/

const vk::Fence& Fence::getVkHandle() const {
	if (!alive()) {
		throw std::runtime_error("Called 'getVkHandle' on destroyed Fence object.");
	}

	return vk_fence_->get();
}

void Fence::free() {
	vk_fence_->destroy();
}

}
