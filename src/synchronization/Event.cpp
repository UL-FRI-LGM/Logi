#include "logi/synchronization/Event.h"

namespace logi {

EventCreateInfo::EventCreateInfo(const vk::EventCreateFlags& flags) : flags(flags) {}

vk::EventCreateInfo EventCreateInfo::build() const {
	vk::EventCreateInfo create_info(flags);
	create_info.pNext = buildExtensions();

	return create_info;
}

Event::Event() : DependentDestroyableHandle({}, false) {
    
}

Event::Event(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const EventCreateInfo& event_ci)
    : DependentDestroyableHandle(owner), vk_event_(nullptr) {
	vk_event_ = std::make_shared<ManagedVkEvent>(device, device.createEvent(event_ci.build()));
}

bool Event::status() const {
	checkForNullHandleInvocation("Event", "status");
	return vk_event_->getOwner().getEventStatus(vk_event_->get()) == vk::Result::eEventSet;
}

void Event::reset() const {
	checkForNullHandleInvocation("Event", "reset");
	vk_event_->getOwner().resetEvent(vk_event_->get());
}

void Event::signal() const {
	checkForNullHandleInvocation("Event", "signal");
	vk_event_->getOwner().setEvent(vk_event_->get());
}

vk::Event Event::getVkHandle() const {
	return vk_event_->get();
}


}
