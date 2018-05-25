/*
 * Copyright (C) 2017 by Primoz Lavric
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "base/ManagedResource.h"

namespace vkr {

void Deletable::addOnDeleteCallback(const std::function<void()>& callback) {
	on_delete_callbacks_.push_back(callback);
}

Deletable::operator bool() const {
	return !deleted_.load(std::memory_order_consume);
}

bool Deletable::operator!() const {
	return deleted_.load(std::memory_order_consume);
}

bool Deletable::isDeleted() const {
	return deleted_.load(std::memory_order_consume);
}

Deletable::Deletable(const bool deleted) : deleted_(deleted) {
}

void Deletable::destroy() {
	deleted_.store(true, std::memory_order_release);
	// Fire on delete callbacks.
	for (auto& callback : on_delete_callbacks_) {
		callback();
	}
}

void ResourceComposite::destroy() {
	// Destroy all dependent resources.
	while (!dependent_resources_.empty()) {
		std::shared_ptr<Deletable>& res = dependent_resources_.back();

		// If the resource was already deleted pop it from the resources, otherwise destroy the object 
		// and let it be deleted via a onDeleteCalback function.
		if (res->isDeleted()) {
			dependent_resources_.pop_back();
		}
		else {
			res->destroy();
		}
	}
}

void ResourceComposite::registerDependentResource(const std::shared_ptr<Deletable>& resource) {
	// Register dependent resource.
	dependent_resources_.push_back(resource);
	auto it = dependent_resources_.rbegin();

	// On delete remove the object from the dependent resourecs list.
	resource->addOnDeleteCallback([this, it]() {
		dependent_resources_.erase(it.base());
	});
}


}