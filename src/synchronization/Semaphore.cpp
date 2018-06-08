#include "synchronization/Semaphore.h"

namespace logi {

Semaphore::Semaphore(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const vk::SemaphoreCreateFlags& flags)
	: DependentDestroyableHandle(owner), vk_semaphore_(nullptr) {
	vk::SemaphoreCreateInfo semaphore_ci{};
	semaphore_ci.flags = flags;

	vk_semaphore_ = std::make_shared<ManagedVkSemaphore>(device, device.createSemaphore(semaphore_ci));
}


const vk::Semaphore& Semaphore::getVkHandle() const {
	if (!alive()) {
		throw std::runtime_error("Called 'getVkHandle' on destroyed Semaphore object.");
	}

	return vk_semaphore_->get();
}

void Semaphore::free() {
	vk_semaphore_->destroy();
}

}
