#include <utility>
#include "base/ExtensionObject.h"

namespace logi {

void* ExtensionObject::buildExtensions(ExtensionObject& extension) {
	return extension.build();
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
		if (jt == rhs.extensions_.end() || (!it->second.comparator_fn(it->second.extension, jt->second.extension))) {
			return false;
		}
	}

	return true;
}

void* ExtendableObject::buildExtensions() const {
	void* next = nullptr;
	for (auto it = extensions_.rbegin(); it != extensions_.rend(); ++it) {
		next = it->second.extension->build(next);
	}

	return next;
}

ExtendableObject::ExtensionEntry::ExtensionEntry(std::unique_ptr<ExtensionObject>&& extension,
                                                 std::function<bool(const std::unique_ptr<ExtensionObject>&, const std::unique_ptr<ExtensionObject>&)> comparator_fn)
	: extension(std::move(extension)), comparator_fn(std::move(comparator_fn)){}

ExtendableObject::ExtensionEntry::ExtensionEntry(const ExtensionEntry& rhs)
	: extension(rhs.extension->clone()), comparator_fn(rhs.comparator_fn) { }

ExtendableObject::ExtensionEntry& ExtendableObject::ExtensionEntry::operator=(const ExtensionEntry& rhs) {
	extension = rhs.extension->clone();
	comparator_fn = rhs.comparator_fn;
	return *this;
}

}
