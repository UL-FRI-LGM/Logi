#ifndef SYNCHRONIZATION_FENCE_H_
#define SYNCHRONIZATION_FENCE_H_

#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/base/ManagedResource.h"

namespace logi {

class LogicalDevice;

class Fence : public DestroyableOwnedHandle<LogicalDevice> {
 public:
  /**
   * @brief	Default placeholder constructor.
   */
  Fence() = default;

  /**
   * @brief	Create a handle used to access Vulkan fence object.
   *
   * @param	device	  Logical device handle.
   * @param	flags	  Used to specify additional Semaphore properties.
   */
  Fence(const LogicalDevice& device, const vk::FenceCreateFlags& flags);

  /**
   * @brief Return true if the fence is signaled.
   *
   * @return True if the fence is signaled.
   */
  bool getStatus() const;

  /**
   * @brief Resets the state to unsignaled.
   */
  void reset() const;

  /**
   * @brief Wait for the fence to be signaled.
   *
   * @param timeout Timeout time in nanoseconds.
   * @return Vulkan operation result.
   */
  vk::Result wait(uint64_t timeout) const;

  /**
   * @brief Wait for the fences to be signaled.
   * @note All fences must originate from the same device!
   * TODO(plavric): Change this so that the fences wont have to originate from the same device.
   *
   * @param	fences		Vector containing the fences.
   * @param	timeout		Timeout time in nanoseconds.
   * @param	wait_all	If true the operation will block until all fences are signaled, otherwise until the first fence is
   * signaled.
   * @return	Vulkan wait result.
   */
  // static vk::Result wait(const std::vector<Fence>& fences, bool wait_all, uint64_t timeout);

  /**
   * @brief Retrieve vk::Fence handle.
   *
   * @return vk::Fence handle.
   */
  const vk::Fence& getVkHandle() const;

  /**
   * @brief  Conversion to vk::Fence handle.
   *
   * @return vk::Fence handle.
   */
  operator vk::Fence() const;

 protected:
  void free() override;

 private:
  using ManagedVkFence = ManagedResource<vk::Device, vk::Fence, vk::DispatchLoaderStatic, &vk::Device::destroyFence>;

  std::shared_ptr<ManagedVkFence> vk_fence_;
};

} // namespace logi

#endif //! SYNCHRONIZATION_FENCE_H_