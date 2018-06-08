/*
*
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef BASE_MANAGED_RESOURCE_H
#define BASE_MANAGED_RESOURCE_H

#include <vulkan/vulkan.hpp>

namespace logi {

/**
 * @brief	Class used to wrap Vulkan resources to provide convenient deletion.
 *
 * @tparam	OwnerType	Type of the object that was used to create the resource and will be used to destroy the object.
 * @tparam	Type		Type of the managed object.
 * @tparam	Dispatch	Type of the Dispatch.
 * @tparam	deleter		OwnerType object member function used to destroy managed object.
 */
template <typename OwnerType, typename Type, typename Dispatch, void(OwnerType::*deleter)(Type, const vk::AllocationCallbacks*, const Dispatch&) const>
class ManagedResource {
public:
	/**
	 * @brief	Default placeholder constructor.
	 */
	ManagedResource();

	/**
	 * @brief	Initialize managed Vulkan object.
	 *
	 * @param	owner_object			Parent object responsible for deallocation.
	 * @param	object					Managed object.
	 * @param	allocation_callbacks	Allocation callbacks.
	 * @param	dispatch				Dispatch.
	 */
	ManagedResource(const OwnerType& owner_object, const Type& object, vk::AllocationCallbacks* allocation_callbacks = nullptr, const Dispatch& dispatch = Dispatch());

	/**
	 * @brief	Retrieve reference to the managed object.
	 *
	 * @return	Reference to the managed object.
	 */
	const Type& get() const;

	/**
	 * @brief	Retrieve reference to the owner object.
	 *
	 * @return	Reference to the owner object.
	 */
	const OwnerType& getOwner() const;

	/**
	 * @brief	Destroys managed object.
	 */
	void destroy();

private:
	OwnerType owner_object_;						///< Owner object.
	Type object_;									///< Managed object.
	vk::AllocationCallbacks* allocation_callbacks_;	///< Allocation callbacks.
	Dispatch dispatch_;								///< Dispatch function.	
};

template <typename OwnerType, typename Type, typename Dispatch, void( OwnerType::* deleter)(Type, const vk::
	AllocationCallbacks*, const Dispatch&) const
>
ManagedResource<OwnerType, Type, Dispatch, deleter>::ManagedResource() : owner_object_(nullptr), object_(nullptr), allocation_callbacks_(nullptr), dispatch_() {
}

template <typename OwnerType, typename Type, typename Dispatch, void( OwnerType::* deleter)(Type, const vk::AllocationCallbacks*, const Dispatch&) const>
ManagedResource<OwnerType, Type, Dispatch, deleter>::ManagedResource(const OwnerType& owner_object, const Type& object, vk::AllocationCallbacks* allocation_callbacks, const Dispatch& dispatch)
	: owner_object_(owner_object), object_(object), allocation_callbacks_(allocation_callbacks), dispatch_(dispatch) {
	if (!owner_object_) {
		throw std::runtime_error("Tried to initialize ManagedResource with object that was already destroyed.");
	}
}

template <typename OwnerType, typename Type, typename Dispatch, void(OwnerType::*deleter)(Type, const vk::AllocationCallbacks*, const Dispatch&) const>
const Type& ManagedResource<OwnerType, Type, Dispatch, deleter>::get() const {
	return object_;
}

template <typename OwnerType, typename Type, typename Dispatch, void(OwnerType::*deleter)(Type, const vk::AllocationCallbacks*, const Dispatch&) const>
const OwnerType& ManagedResource<OwnerType, Type, Dispatch, deleter>::getOwner() const {
	return owner_object_;
}

template <typename OwnerType, typename Type, typename Dispatch, void(OwnerType::*deleter)(Type, const vk::AllocationCallbacks*, const Dispatch&) const>
void ManagedResource<OwnerType, Type, Dispatch, deleter>::destroy() {
	(owner_object_.*deleter)(object_, allocation_callbacks_, dispatch_);
}


} ///	namespace logi

#endif ///	HEADER_GUARD
