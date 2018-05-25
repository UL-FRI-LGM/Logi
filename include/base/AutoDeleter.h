/*
* 
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef BASE_AUTO_DELETER_H
#define BASE_AUTO_DELETER_H

#include <memory>
#include <atomic>
#include <vulkan/vulkan.hpp>

namespace vkr {

template <typename Type>
class AutoDeleter {
public:
	AutoDeleter(const Type& object, const std::function<void(const Type&)>& deleter_fn) : reference_count_(std::make_shared<std::atomic<size_t>>(1u)), object_(object), deleter_fn_(deleter_fn) {}

	AutoDeleter(AutoDeleter&& other) noexcept {
		*this = std::move(other);
	}

	AutoDeleter(const AutoDeleter& other) {
		*this = other;
	}

	AutoDeleter& operator=(AutoDeleter&& other) noexcept {
		if (this != &other) {
			reference_count_ = other.reference_count_;
			object_ = other.object_;
			deleter_fn_ = other.deleter_fn_;
			other.object_ = nullptr;
		}

		return *this;
	}

	AutoDeleter& operator=(const AutoDeleter& other) {
		reference_count_ = other.reference_count_;
		object_ = other.object_;
		deleter_fn_ = other.deleter_fn_;

		if (object_) {
			++(*reference_count_);
		}

		return *this;
	}

	~AutoDeleter() {
		if (object_) {
			--(*reference_count_);

			if ((*reference_count_).load(std::memory_order_acquire) == 0u) {
				deleter_fn_(object_);
			}
		}
	}

protected:
	const Type& get() const {
		return object_;
	}

private:
	std::shared_ptr<std::atomic<size_t>> reference_count_;
	Type object_;
	std::function<void(const Type&)> deleter_fn_;
};

}

#endif