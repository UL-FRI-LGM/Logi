/*
*
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef BASE_MANAGED_RESOURCE_H
#define BASE_MANAGED_RESOURCE_H

#include <list>
#include <memory>
#include <atomic>
#include <functional>
#include <vulkan/vulkan.hpp>

namespace vkr {

/**
 * @brief Defines interface for deletable objects and provides on delete callback functionality.
 */
class Deletable {
public:
	/**
	 * @brief	Initialize deletable object.
	 *
	 * @param	deleted	The object may be initialized as deleted.
	 */
	explicit Deletable(bool deleted);

	/**
	 * @brief	Call all on delete functions.
	 */
	virtual void destroy() = 0;

	/**
	 * @brief	Add callback function that will be fired once the managed object is deleted.
	 *
	 * @param	callback On delete callback.
	 */
	void addOnDeleteCallback(const std::function<void()>& callback);

	/**
	* @brief	Return true if the managed object was not yet deleted.
	*/
	operator bool() const;

	/**
	* @brief	Return true if the managed object was already deleted.
	*/
	bool operator!() const;

	/**
	 * @brief	Return true if the managed object was already deleted.
	 */
	bool isDeleted() const;

	/**
	 * @brief	Default virtual destructor.
	 */
	virtual ~Deletable() = default;

private:
	std::atomic<bool> deleted_;									///< Tracks if object state (deleted/not deleted).	
	std::vector<std::function<void()>> on_delete_callbacks_{};	///< Functions that are called once the object is deleted.	
};


/**
 * @brief	Class used to wrap Vulkan resources to provide convinient deletion.
 *
 * @tparam	OwnerType	Type of the object that was used to create the resource and will be used to destroy the object.
 * @tparam	Type		Type of the managed object.
 * @tparam	Dispatch	Type of the Dispatch.
 * @tparam	deleter		OwnerType object member function used to destroy managed object.
 */
template <typename OwnerType, typename Type, typename Dispatch, void(OwnerType::*deleter)(Type, const vk::AllocationCallbacks*, const Dispatch&) const>
class ManagedResource : public Deletable {
public:
	/**
	 * @brief	Initialize managed Vulkan object.
	 *
	 * @param	owner_object			Parent object responsible for deallocation.
	 * @param	object					Managed object.
	 * @param	allocation_callbacks	Allocation callbacks.
	 * @param	dispatch				Dispatch.
	 */
	ManagedResource(const OwnerType& owner_object, const Type& object, const vk::AllocationCallbacks* allocation_callbacks = nullptr, const Dispatch& dispatch = Dispatch()) 
		: Deletable(!object), owner_object_(owner_object), object_(object), allocation_callbacks_(allocation_callbacks), dispatch_(dispatch) {}

	/**
	* @brief	Destroy the managed object using the deleter function that was specified via deleter function.
	*/
	void destroy() override;

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

private:
	OwnerType owner_object_;									///< Owner object.
	Type object_;												///< Managed object.
	const vk::AllocationCallbacks* allocation_callbacks_;		///< Allocation callbacks.
	const Dispatch& dispatch_;									///< Dispatch function.
};

template <typename OwnerType, typename Type, typename Dispatch, void(OwnerType::*deleter)(Type, const vk::AllocationCallbacks*, const Dispatch&) const>
void ManagedResource<OwnerType, Type, Dispatch, deleter>::destroy() {
	if (!isDeleted()) {
		// Additionally check if the object was actually allocated.
		(owner_object_.*deleter)(object_, allocation_callbacks_, dispatch_);
		Deletable::destroy();
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


/**
 * @brief	Class used to manage dependent Deletable objects.
 */
class ResourceComposite : public Deletable {
public:
	/**
	 * @brief	Destroy all dependend Deletable objects.
	 */
	void destroy() override;

	/**
	 * @brief	Register dependent resource that will be destroyed once the destroy function is called.
	 *
	 * @param	resource	Dependent resources.
	 */
	void registerDependentResource(const std::shared_ptr<Deletable>& resource);

private:
	std::list<std::shared_ptr<Deletable>> dependent_resources_{};	///< List of dependent resources.
};

} ///	namespace vkr

#endif ///	HEADER_GUARD
