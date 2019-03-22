#include "logi/synchronization/Semaphore.h"
#include "logi/base/LogicalDevice.h"

namespace logi {

Semaphore::Semaphore(const LogicalDevice& device, const vk::SemaphoreCreateFlags& flags)
  : DestroyableOwnedHandle<Semaphore, LogicalDevice>(device, true), vk_semaphore_(nullptr) {
  vk::SemaphoreCreateInfo semaphore_ci{};
  semaphore_ci.flags = flags;

  const vk::Device& vk_handle = device.getLogicalDeviceHandle();
  std::make_shared<ManagedVkSemaphore>(vk_handle, vk_handle.createSemaphore(semaphore_ci));
}

const vk::Semaphore& Semaphore::getVkHandle() const {
  checkForNullHandleInvocation("Semaphore", "getVkHandle");
  return vk_semaphore_->get();
}

Semaphore::operator vk::Semaphore() const {
  checkForNullHandleInvocation("Semaphore", "operator vk::Semaphore()");
  return vk_semaphore_->get();
}

void Semaphore::free() {
  if (valid()) {
    vk_semaphore_->destroy();
    DestroyableOwnedHandle<Semaphore, LogicalDevice>::free();
  }
}

} // namespace logi
