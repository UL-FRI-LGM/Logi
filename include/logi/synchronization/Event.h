#ifndef SYNCHRONIZATION_EVENT_H
#define SYNCHRONIZATION_EVENT_H

#include <vulkan/vulkan.hpp>
#include "logi/base/ExtensionObject.h"
#include "logi/base/Handle.h"
#include "logi/base/ManagedResource.h"

namespace logi {

struct EventCreateInfo : public BuildableExtendable {
  /**
   * @brief	Populates EventCreateInfo configuration structure.
   *
   * @param	flags   Additional configuration flags.
   */
  explicit EventCreateInfo(const vk::EventCreateFlags& flags = {});

  /**
   * @brief	Build Vulkan vk::EventCreateInfo using the member values.
   *
   * @return EventCreateInfo configuration structure.
   */
  vk::EventCreateInfo build() const;

  /**
   * Additional configuration flags.
   */
  vk::EventCreateFlags flags;
};

class LogicalDevice;

/**
 * @brief
 */
class Event : public DestroyableOwnedHandle<Event, LogicalDevice> {
 public:
  /**
   * @brief	Default placeholder constructor.
   */
  Event() = default;

  /**
   * @brief	Create Vulkan event handle.
   *
   * @param	device    Logical device handle.
   * @param	event_ci  Event create info structure.
   */
  Event(const LogicalDevice& device, const EventCreateInfo& event_ci);

  /**
   * @brief
   *
   * @return
   */
  bool status() const;

  /**
   * @brief	Reset event to non-signaled state
   */
  void reset() const;

  /**
   * @brief	Set an event to signaled state
   */
  void signal() const;

  /**
   * @brief	Retrieve Vulkan event handle.
   *
   * @return	Vulkan event handle.
   */
  vk::Event getVkHandle() const;

  operator vk::Event() const;

 protected:
  void free() override;

 private:
  using ManagedVkEvent = ManagedResource<vk::Device, vk::Event, vk::DispatchLoaderStatic, &vk::Device::destroyEvent>;

  /**
   * @brief	Managed Vulkan event handle.
   */
  std::shared_ptr<ManagedVkEvent> vk_event_;
};

} // namespace logi

#endif //! SYNCHRONIZATION_EVENT_H