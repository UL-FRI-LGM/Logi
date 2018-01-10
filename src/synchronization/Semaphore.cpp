#include "..\..\include\synchronization\Semaphore.h"

namespace vkr {

Semaphore::Semaphore(const vk::Device& device, const vk::PipelineStageFlags& wait_stages) : device_(device), semaphore_(nullptr) {
	semaphore_ = device_.createSemaphore(vk::SemaphoreCreateInfo{});
}

const vk::Semaphore& Semaphore::getVkHandle() const {
	return semaphore_;
}

void Semaphore::setWaitStages(const vk::PipelineStageFlags& wait_stages) {
	wait_stages_ = wait_stages;
}

const vk::PipelineStageFlags& Semaphore::getWaitStages() const {
	return wait_stages_;
}

Semaphore::~Semaphore() {
	device_.destroySemaphore(semaphore_);
}

}
