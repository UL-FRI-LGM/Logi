/*
*
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef BASE_UNIQUE_WRAPPER_H
#define BASE_UNIQUE_WRAPPER_H

#include <memory>
#include <functional>

namespace logi {

/**
 * @brief   Copiable class used to wrap object and make it easier to use.
 */
template <typename BaseType>
class UniqueWrapper {
public:
	/**
	 * @brief   Default constructor.
	 */
	UniqueWrapper() = default;

	/**
	 * @brief	Constructs and populates the structure.
	 *
	 * @param	object		    Unique pointer to the object object.
	 * @param	comparator_fn	Comparator function used to compare object objects.
	 */
	UniqueWrapper(std::unique_ptr<BaseType>&& object, std::function<bool(const BaseType&, const BaseType&)> comparator_fn);

	/**
	 * @brief	Copy constructor that deep copies object object.
	 *
	 * @param	rhs	Right hand side UniqueWrapper.
	 */
	UniqueWrapper(const UniqueWrapper<BaseType>& rhs);

	/**
	 * @brief	Copy assignment operator that deep copies object.
	 *
	 * @param	rhs	Right hand side UniqueWrapper.
	 * @return	Reference to this.
	 */
	UniqueWrapper& operator=(const UniqueWrapper<BaseType>& rhs);

	/**
	 * @brief   Compare the wrapped objects.
	 *
	 * @return  True if the wrapped objects are equal.
	 */
	bool operator==(const UniqueWrapper<BaseType>& rhs) const;

	/**
	 * @brief   Compare the wrapped objects.
	 *
	 * @return  True if the wrapped objects are not equal.
	 */
	bool operator!=(const UniqueWrapper<BaseType>& rhs) const;

	/**
	 * @brief   Derefereneces the wrapped object.
	 *
	 * @return  Pointer to the wrapped object.
	 */
	BaseType* operator->() const;

	/**
	 * @brief   Retrieve pointer to the wrapped object.
	 *
	 * @return  Pointer to the wrapped object.
	 */
	BaseType* get() const;

	/**
	 * @brief   Create UniqueWrapper that wrapps the given object and creates comparator function used to compare it.
	 *
	 * @tparam	DerivedType Derived type of the object.
	 * @param	object      Object to be wrapped.
	 * @return  Object wrapped in the UniqueWrapper.
	 */
	template <typename DerivedType>
    static UniqueWrapper create(const DerivedType& object);

private:
	/**
	 * Unique pointer to the object.
	 */
	std::unique_ptr<BaseType> object_;

	/**
	 * Function used to compare two objects.
	 */
	std::function<bool(const BaseType&, const BaseType&)> comparator_fn_;
};

template <typename BaseType>
UniqueWrapper<BaseType>::UniqueWrapper(std::unique_ptr<BaseType>&& object,
	std::function<bool(const BaseType&, const BaseType&)> comparator_fn)
	: object_(std::move(object)), comparator_fn_(std::move(comparator_fn)) { }

template <typename BaseType>
UniqueWrapper<BaseType>::UniqueWrapper(const UniqueWrapper& rhs)
	: object_(rhs.object_->clone()), comparator_fn_(rhs.comparator_fn_) { }

template <typename BaseType>
UniqueWrapper<BaseType>& UniqueWrapper<BaseType>::operator=(const UniqueWrapper<BaseType>& rhs) {
	object_ = rhs.object_->clone();
	comparator_fn_ = rhs.comparator_fn_;
	return *this;
}

template <typename BaseType>
bool UniqueWrapper<BaseType>::operator==(const UniqueWrapper<BaseType>& rhs) const {
	return comparator_fn_(*object_, *rhs.object_);
}

template <typename BaseType>
bool UniqueWrapper<BaseType>::operator!=(const UniqueWrapper<BaseType>& rhs) const {
	return !comparator_fn_(*object_, *rhs.object_);
}

template <typename BaseType>
BaseType* UniqueWrapper<BaseType>::operator->() const {
	return object_.get();
}

template <typename BaseType>
BaseType* UniqueWrapper<BaseType>::get() const {
	return object_.get();
}

template <typename BaseType>
template <typename DerivedType>
UniqueWrapper<BaseType> UniqueWrapper<BaseType>::create(const DerivedType& object) {
    // Generate comparator.
    const std::function<bool(const BaseType&, const BaseType&)> comparator_fn = [
        ](const BaseType& lhs, const BaseType& rhs) {
        const DerivedType* lhs_ptr = dynamic_cast<const DerivedType*>(&lhs);
        const DerivedType* rhs_ptr = dynamic_cast<const DerivedType*>(&rhs);

        // Left hand side pointer should always be of DerivedType so there is no need to check if dynamic cast succeeded.
        return (rhs_ptr != nullptr) && (*lhs_ptr) == (*rhs_ptr);
    };

    return UniqueWrapper(std::make_unique<DerivedType>(object), comparator_fn);
}

}

#endif
