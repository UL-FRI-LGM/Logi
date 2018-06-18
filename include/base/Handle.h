/*
*
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef BASE_HANDLE_H
#define BASE_HANDLE_H

#include <mutex>
#include <iostream>
#include <atomic>
#include <unordered_map>

namespace logi {

// Forward declaration of HandleManager for use in Handle and DependentDestroyableHandle.
class HandleManager;

/**
 * @brief	Base class for all Handle objects.
 */
class Handle {
	friend class HandleManager;
public:
	/**
	 * @brief	Create new handle object. Assigns a unique identifier to the handle.
	 * 
	 * @param	alive	Initial alive state.
	 */
	explicit Handle(bool alive = true);

	/**
	 * @brief	Retrieve handle unique identifier.
	 *
	 * @return	Handle unique identifier.
	 */
	size_t id() const;

	/**
	 * @brief	Return true if handles refer to the same resource.
	 *
	 * @param	other	Other handle.
	 * @return	true	If handles refer to the same resource.
	 */
	bool operator==(const Handle& other) const;

	/**
	 * @brief	Return true if the handle is valid.
	 */
	operator bool() const;

	/**
	 * @brief	Return true if the handle is not valid.
	 */
	bool operator!() const;

	/**
	 * @brief	Return true if the handle is valid.
	 *
	 * @return	true	If object is valid.
	 */
	bool alive() const;

	/**
	 * @brief	Default virtual destructor.
	 */
	virtual ~Handle() = default;

protected:
	/**
	 * @brief	Destroy handle resources.
	 */
	virtual void free();

private:
	std::shared_ptr<std::atomic<bool>> alive_;	///< If true the handle is still valid.
	size_t id_;									///< Handle identifier.
	static size_t id_generator_;				///< Used to generate identifiers.
};

/**
 * @brief	Provides destroy functionality to the Handle.
 */
class DestroyableHandle : public Handle {
public:
	explicit DestroyableHandle(bool alive = true);

	/**
	 * @brief	Calls handle free method.
	 */
	virtual void destroy();
};


/**
 * @brief	Provides destroy functionality to the Handle. Destroy forwards the destroy call to the HandleManager.
 */
class DependentDestroyableHandle : public Handle {
public:
	/**
	 * @brief	Constructs destroyable handle object.
	 *
	 * @param	owner	HandleManager responsible for this Handle.
	 */
	explicit DependentDestroyableHandle(std::weak_ptr<HandleManager> owner, bool alive = true);

	/**
	 * @brief	Delegates destroy call to the Handle owner.
	 */
	virtual void destroy() const;

private:
	std::weak_ptr<HandleManager> owner_;	///< Pointer to the HandleManager that is responsible for this Handle.
};

/**
 * @brief	HandleManager is an object that is responsible for Handle creation and destruction.
 */
class HandleManager : public std::enable_shared_from_this<HandleManager> {
public:
	/**
	 * @brief	Default constructor.
	 */
	HandleManager() = default;

	/**
	 * @brief	Create and register DestroyableHandles.
	 *
	 * @tparam	T		Handle type (must inherit from DependentDestroyableHandle).
	 * @tparam	Args	Handle constructor argument types.
	 * @param	args	Handle constructor arguments.
	 * @return	Constructed handle of type T.
	 */
	template <typename T, class... Args, typename std::enable_if<std::is_base_of<DependentDestroyableHandle, T>::value>::type* = nullptr>
	T createHandle(Args&&...args);

	/**
	 * @brief	Create and register non DestroyableHandles.
	 *
	 * @tparam	T		Handle type (must inherit from Handle).
	 * @tparam	Args	Handle constructor argument types.
	 * @param	args	Handle constructor arguments.
	 * @return	Constructed handle of type T.
	 */
	template <typename T, class... Args, typename std::enable_if<std::is_base_of<Handle, T>::value && !std::is_base_of<DependentDestroyableHandle, T>::value>::type* = nullptr>
	T createHandle(Args&&...args);

	/**
	 * @brief	Retrieve Handle with the given identifier.
	 * 
	 * @tparam	T	Type of the handle.
	 * @param	id	Handle unique identifier.
	 * @return	Handle with the given identifier.
	 */
	template <typename T>
	T getHandle(size_t id) const;

	/**
	 * @brief	Destroys the given handle and unregisters it from HandleManager.
	 *
	 * @param	handle	Handle that should be destroyed.
	 */
	void destroyHandle(const Handle& handle);

	/**
	 * @brief	Destroys all handles managed by this HandleManager.
	 */
	void destroyAllHandles();

private:
	std::mutex resources_lock_;						/// Mutex used to synchronize Handle creation and destruction.
	std::unordered_map<size_t, std::unique_ptr<Handle>> handles_;	/// Map that maps Handle identifier to Handle.
};

template <typename T, class ... Args, typename std::enable_if<std::is_base_of<DependentDestroyableHandle, T>::value>::type*>
T HandleManager::createHandle(Args&&... args) {
	// Construct handle, register and return it.
	T handle = T(std::weak_ptr<HandleManager>(shared_from_this()), std::forward<Args>(args)...);

	std::lock_guard<std::mutex> guard(resources_lock_);
	handles_.emplace(std::make_pair(handle.id(), std::make_unique<T>(handle)));
	return handle;
}

template <typename T, class... Args, typename std::enable_if<std::is_base_of<Handle, T>::value && !std::is_base_of<DependentDestroyableHandle, T>::value>::type*>
T HandleManager::createHandle(Args&&... args) {
	// Construct handle, register and return it.
	T handle = T(std::forward<Args>(args)...);

	std::lock_guard<std::mutex> guard(resources_lock_);
	handles_.emplace(std::make_pair(handle.id(), std::make_unique<T>(handle)));
	return handle;
}

template <typename T>
T HandleManager::getHandle(size_t id) const {
	const auto it = handles_.find(id);

	if (it == handles_.end()) {
		throw std::runtime_error("Failed to find the requested handle.");
	}

	return static_cast<T>(*it);
}

}

#endif BASE_HANDLE_H