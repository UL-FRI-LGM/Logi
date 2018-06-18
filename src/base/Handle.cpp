#include "base/Handle.h"
#include <utility>

namespace logi {

size_t Handle::id_generator_ = 0u;

Handle::Handle(bool alive) : alive_(std::make_shared<std::atomic<bool>>(alive)), id_(id_generator_++) {}

size_t Handle::id() const {
	return id_;
}

bool Handle::operator==(const Handle& other) const {
	return id_ == other.id_;
}

Handle::operator bool() const {
	return alive_->load(std::memory_order_consume);
}

bool Handle::operator!() const {
	return !alive_->load(std::memory_order_consume);
}

bool Handle::alive() const {
	return alive_->load(std::memory_order_consume);
}

void Handle::free() {
	alive_->store(false, std::memory_order_release);
}

DestroyableHandle::DestroyableHandle(bool alive)
	: Handle(true) { }

void DestroyableHandle::destroy() {
	free();
}

DependentDestroyableHandle::DependentDestroyableHandle(std::weak_ptr<HandleManager> owner, const bool alive) : Handle(alive), owner_(std::move(owner)) {}

void DependentDestroyableHandle::destroy() const {
	// Check if the object is still alive before trying to destroy it.
	if (alive()) {
		// Try to get a hold of the owner.
		auto owner_sp = owner_.lock();
		if (owner_sp) {
			owner_sp->destroyHandle(*this);
		}
	}
}

void HandleManager::destroyHandle(const Handle& handle) {
	std::lock_guard<std::mutex> guard(resources_lock_);

	// If the handle was found free its resources and erase it. TODO(plavric): Is this check necessary?
	auto it = handles_.find(handle.id());
	if (it != handles_.end()) {
		it->second->free();
		handles_.erase(it);
	}
}

void HandleManager::destroyAllHandles() {
	std::lock_guard<std::mutex> guard(resources_lock_);

	// Remove all handles.
	for (auto& handle : handles_) {
		handle.second->free();
	}
	handles_.clear();
}

}