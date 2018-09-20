#include <utility>
#include "base/ExtensionObject.h"

namespace logi {

Extension::~Extension() = default;

bool Extendable::operator==(const Extendable& rhs) const {
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

void* BuildableExtension::buildExtensions(BuildableExtension& extension) {
	return extension.build();
}

bool BuildableExtendable::operator==(const BuildableExtendable& rhs) const {
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

void* BuildableExtendable::buildExtensions() const {
	void* next = nullptr;
	for (auto it = extensions_.rbegin(); it != extensions_.rend(); ++it) {
		next = it->second->build(next);
	}

	return next;
}

}
