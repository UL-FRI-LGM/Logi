#include "logi/synchronization/Fence.h"
#include "logi/base/LogicalDevice.h"

namespace logi {

Fence::Fence(const LogicalDevice& device, const vk::FenceCreateFlags& flags)
  : DestroyableOwnedHandle<Fence, LogicalDevice>(device, true), vk_fence_(nullptr) {
  vk::FenceCreateInfo fence_ci{};
  fence_ci.flags = flags;

  const vk::Device& vk_handle = device.getLogicalDeviceHandle();
  vk_fence_ = std::make_shared<ManagedVkFence>(vk_handle, vk_handle.createFence(fence_ci));
}

bool Fence::getStatus() const {
  checkForNullHandleInvocation("Fence", "getStatus");
  return static_cast<vk::Device>(getOwner<LogicalDevice>()).getFenceStatus(vk_fence_->get()) == vk::Result::eSuccess;
}

void Fence::reset() const {
  checkForNullHandleInvocation("Fence", "reset");
  static_cast<vk::Device>(getOwner<LogicalDevice>()).resetFences({vk_fence_->get()});
}

vk::Result Fence::wait(const uint64_t timeout) const {
  checkForNullHandleInvocation("Fence", "wait");
  return static_cast<vk::Device>(getOwner<LogicalDevice>()).waitForFences({vk_fence_->get()}, true, timeout);
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
  checkForNullHandleInvocation("Fence", "getVkHandle");
  return vk_fence_->get();
}

Fence::operator vk::Fence() const {
  checkForNullHandleInvocation("Fence", "operator vk::Fence()");
  return vk_fence_->get();
}

void Fence::free() {
  if (valid()) {
    vk_fence_->destroy();
    DestroyableOwnedHandle<Fence, LogicalDevice>::free();
  }
}

} // namespace logi
