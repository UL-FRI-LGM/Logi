#ifndef SYNCHRONIZATION_SEMAPHORE_H_
#define SYNCHRONIZATION_SEMAPHORE_H_

#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/base/ManagedResource.h"

namespace logi {

class LogicalDevice;

class Semaphore : public DestroyableOwnedHandle<LogicalDevice> {
 public:
  /**
   * @brief	Default placeholder constructor.
   */
  Semaphore() = default;

  /**
   * @brief	Create a handle used to access Vulkan semaphore object.
   *
   * @param	device  Logical device handle.
   * @param	flags   Used to specify additional Semaphore properties.
   */
  explicit Semaphore(const LogicalDevice& device, const vk::SemaphoreCreateFlags& flags = {});

  /**
   * @brief	Retrieve Vulkan semaphore handle.
   *
   * @return	Vulkan semaphore handle.
   */
  const vk::Semaphore& getVkHandle() const;

  operator vk::Semaphore() const;

 protected:
  /**
   * @brief	Destroy the wrapped Vulkan semaphore handle.
   */
  void free() override;

 private:
  using ManagedVkSemaphore =
    ManagedResource<vk::Device, vk::Semaphore, vk::DispatchLoaderStatic, &vk::Device::destroySemaphore>;
  std::shared_ptr<ManagedVkSemaphore> vk_semaphore_;
};

} // namespace logi

#endif //! SYNCHRONIZATION_SEMAPHORE_H_