#include <utility>
#include "base/ExtensionObject.h"

namespace logi {

void* ExtensionObject::buildExtensions(ExtensionObject& extension) {
	return extension.build();
}

ExtensionWrapper::ExtensionWrapper(std::unique_ptr<ExtensionObject>&& extension,
    std::function<bool(const ExtensionObject&, const ExtensionObject&)> comparator_fn)
    : extension_(std::move(extension)), comparator_fn_(std::move(comparator_fn)) { }

ExtensionWrapper::ExtensionWrapper(const ExtensionWrapper& rhs)
    : extension_(rhs.extension_->clone()), comparator_fn_(rhs.comparator_fn_) { }

ExtensionWrapper& ExtensionWrapper::operator=(const ExtensionWrapper& rhs) {
	extension_ = rhs.extension_->clone();
	comparator_fn_ = rhs.comparator_fn_;
	return *this;
}

bool ExtensionWrapper::operator==(const ExtensionWrapper& rhs) const {
	return comparator_fn_(*extension_, *rhs.extension_);
}

bool ExtensionWrapper::operator!=(const ExtensionWrapper& rhs) const {
	return !comparator_fn_(*extension_, *rhs.extension_);
}

ExtensionObject* ExtensionWrapper::operator->() const {
	return extension_.get();
}

ExtensionObject* ExtensionWrapper::get() const {
	return extension_.get();
}

bool ExtendableObject::operator==(const ExtendableObject& rhs) const {
	// Check if the sizes match.
	if (extensions_.size() != rhs.extensions_.size()) {
		return false;
	}

	// Compare extensions.
	for (auto it = extensions_.begin(); it != extensions_.end(); ++it) {
		const auto jt = rhs.extensions_.find(it->first);

		// Check if the same extension exist in both ExtenadbleObjects and then compare the extensions.
		if ((jt == rhs.extensions_.end()) || (it->second != jt->second)) {
			return false;
		}
	}

	return true;
}

void* ExtendableObject::buildExtensions() const {
	void* next = nullptr;
	for (auto it = extensions_.rbegin(); it != extensions_.rend(); ++it) {
		next = it->second->build(next);
	}

	return next;
}

}
