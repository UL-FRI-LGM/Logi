#include "logi/synchronization/Event.h"
#include "logi/base/LogicalDevice.h"

namespace logi {

EventCreateInfo::EventCreateInfo(const vk::EventCreateFlags& flags) : flags(flags) {}

vk::EventCreateInfo EventCreateInfo::build() const {
  vk::EventCreateInfo create_info(flags);
  create_info.pNext = buildExtensions();

  return create_info;
}

Event::Event(const LogicalDevice& device, const EventCreateInfo& event_ci)
  : DestroyableOwnedHandle<Event, LogicalDevice>(device, true), vk_event_(nullptr) {
  vk::Device vk_device = device;
  vk_event_ = std::make_shared<ManagedVkEvent>(vk_device, vk_device.createEvent(event_ci.build()));
}

bool Event::status() const {
  checkForNullHandleInvocation("Event", "status");
  return static_cast<vk::Device>(getOwner<LogicalDevice>()).getEventStatus(vk_event_->get()) == vk::Result::eEventSet;
}

void Event::reset() const {
  checkForNullHandleInvocation("Event", "reset");
  static_cast<vk::Device>(getOwner<LogicalDevice>()).resetEvent(vk_event_->get());
}

void Event::signal() const {
  checkForNullHandleInvocation("Event", "signal");
  static_cast<vk::Device>(getOwner<LogicalDevice>()).setEvent(vk_event_->get());
}

vk::Event Event::getVkHandle() const {
  checkForNullHandleInvocation("Event", "getVkHandle");
  return vk_event_->get();
}

Event::operator vk::Event() const {
  checkForNullHandleInvocation("Event", "operator vk::Event()");
  return vk_event_->get();
}

void Event::free() {
  if (valid()) {
    vk_event_->destroy();
    DestroyableOwnedHandle<Event, LogicalDevice>::free();
  }
}

} // namespace logi
