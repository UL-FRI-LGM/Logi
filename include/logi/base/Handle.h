/*
 * Copyright (C) 2017 by Primoz Lavric
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BASE_HANDLE_H
#define BASE_HANDLE_H

#include <atomic>
#include <memory>
#include <mutex>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include "Exceptions.h"

namespace logi {

template <typename Type, typename SearchedOwner>
class has_get_owner {
 private:
  template <typename T, typename SO>
  static std::true_type test(decltype(&T::template getOwner<SO>));

  template <typename T, typename SO>
  static std::false_type test(...);

 public:
  static constexpr bool value = std::is_same<decltype(test<Type, SearchedOwner>(0)), std::true_type>::value;
};

class Handle {
  template <typename, typename>
  friend class HandleGenerator;

 public:
  /**
   * @brief Initializes handle.
   *
   * @param valid Initial validity flag.
   */
  explicit Handle(bool valid = false);

  /**
   * @brief   Retrieve handle unique identifier.
   *
   * @return  Handle unique identifier.
   */
  size_t id() const;

  /**
   * @brief   Return true if handles refer to the same resource.
   *
   * @param   other Other handle.
   * @return  true	If handles refer to the same resource.
   */
  bool operator==(const Handle& other) const;

  /**
   * @brief   Return true if the handle is valid.
   */
  operator bool() const;

  /**
   * @brief   Return true if the handle is not valid.
   */
  bool operator!() const;

  /**
   * @brief   Return true if the handle is valid.
   *
   * @return  true  If object is valid.
   */
  bool valid() const;

  inline void checkForNullHandleInvocation(const char* object_name, const char* method_name) const {
    if (!valid()) {
      throw IllegalInvocation(std::string("Tried to invoke method ") + method_name + "() on a null " + object_name +
                              " handle.");
    }
  }

 protected:
  /**
   * @brief Invalidates handle.
   */
  virtual void free();

 private:
  /**
   * @brief Handle internal data.
   */
  struct Data {
    /**
     * @brief Initializes validity flag and identifier.
     *
     * @param valid Validity flag.
     * @param id    Identifier.
     */
    Data(bool valid, size_t id);

    /**
     * Flag that specifies validity of the handle.
     */
    std::atomic<bool> valid;

    /**
     * Handle identifier.
     */
    size_t id;
  };

  /**
   * Static atomic size_t value used for identifier generation.
   */
  static std::atomic<size_t> id_generator_;

  /**
   * Handle internal data (should not be null).
   */
  std::shared_ptr<Data> data_;
};

/**
 * @brief Destroyable handle.
 */
class DestroyableHandle : public Handle {
 public:
  using Handle::Handle;

  /**
   * @brief	Calls handle free method.
   */
  void destroy();
};

/**
 * @brief   Handle with pointer to the owner.
 *
 * @tparam  OwnerTypes Possible owner types.
 */
template <typename... OwnerTypes>
class OwnedHandle : public Handle {
 public:
  OwnedHandle();

  /**
   * @brief Initializes base Handle class and owner handle.
   *
   * @param owner Owner handle.
   * @param valid Handle validity flag.
   */
  template <typename T>
  explicit OwnedHandle(const T& owner, bool valid = true);

  /**
   * @brief   Search for owner in the hierarchy.
   *
   * @return  Owner handle.
   */
  template <typename SearchedOwner>
  const SearchedOwner& getOwner() const {
    return std::visit(
      [](const auto& value) -> const SearchedOwner& {
        // Deduce owner type.
        typedef typename std::remove_cv<typename std::remove_reference<decltype(value)>::type>::type OwnerType;

        if constexpr (std::is_same<OwnerType, SearchedOwner>::value) {
          // Found requested owner.
          return value;
        } else if constexpr (has_get_owner<OwnerType, SearchedOwner>::value) {
          // Descend into the actual owner in search for the SearchedOwner.
          return value.template getOwner<SearchedOwner>();
        } else {
          // Actual has no owner.
          throw IllegalInvocation("Could not find owner.");
        }
      },
      *owner_);
  }

 protected:
  /**
   * @brief Frees owner instance.
   */
  void free() override;

  /**
   * @brief Shared owner instance.
   */
  std::shared_ptr<std::variant<OwnerTypes...>> owner_;
};

template <typename... OwnerTypes>
OwnedHandle<OwnerTypes...>::OwnedHandle() : Handle(false), owner_(nullptr) {}

template <typename... OwnerTypes>
template <typename T>
OwnedHandle<OwnerTypes...>::OwnedHandle(const T& owner, bool valid)
  : Handle(valid), owner_(std::make_shared<std::variant<OwnerTypes...>>(owner)) {}

template <typename... OwnerTypes>
void OwnedHandle<OwnerTypes...>::free() {
  owner_.reset();
  Handle::free();
}

/**
 * @brief Destroyable handle.
 */
template <typename SubType, typename... OwnerTypes>
class DestroyableOwnedHandle : public OwnedHandle<OwnerTypes...> {
 public:
  using OwnedHandle<OwnerTypes...>::OwnedHandle;

  /**
   * @brief	Calls handle free method.
   */
  virtual void destroy();
};

template <typename, typename>
class HandleGenerator;

template <typename SubType, typename... OwnerTypes>
void DestroyableOwnedHandle<SubType, OwnerTypes...>::destroy() {
  // Check if the object is still alive before trying to destroy it.
  if (OwnedHandle<OwnerTypes...>::valid()) {
    std::visit(
      [this](auto& owner) {
        // Deduce owner type.
        static_cast<HandleGenerator<typename std::remove_reference<decltype(owner)>::type, SubType>&>(owner)
          .destroyHandle(static_cast<const SubType&>(*this));
      },
      *OwnedHandle<OwnerTypes...>::owner_);
  }
}

/**
 * @brief
 *
 * @tparam GeneratorType
 * @tparam HandleType
 */
template <typename GeneratorType, typename HandleType>
class HandleGenerator {
  template <typename, typename...>
  friend class DestroyableOwnedHandle;

 public:
  /**
   * @brief Initializes HandleGenerator internal data.
   */
  HandleGenerator();

  /**
   * @brief   Check if the handle with the given identifier was generated by this generator.
   *
   * @param   id  Handle identifier.
   * @return  True if the handle was found.
   */
  bool hasHandle(size_t id) const;

  /**
   * @brief   Retrieve handle with the given identifier.
   * @throws  Exception if the handle is not found.
   *
   * @param   id  Handle identifier.
   * @return  Const reference to the handle.
   */
  const HandleType& getHandle(size_t id) const;

 protected:
  /**
   * @brief   Creates new handle of HandleType type with the given arguments.
   *
   * @tparam  Args  Argument types.
   * @param   args  Arguments.
   * @return  Reference to the newly created handle.
   */
  template <typename... Args>
  const HandleType& createHandle(Args... args) const;

  /**
   * @brief	Destroys the given handle and unregisters it from HandleManager.
   *
   * @param	handle	Handle that should be destroyed.
   */
  void destroyHandle(const HandleType& handle) const;

  /**
   * @brief	Destroys all handles generated by this HandleGenerator.
   */
  void destroyAllHandles() const;

 private:
  /**
   * @brief Handle generator internal data.
   */
  struct Data {
    /**
     * Lock used to synchronize resource creation.
     */
    std::mutex handles_lock;

    /**
     * Owned handles.
     */
    std::unordered_map<size_t, std::unique_ptr<HandleType>> handles;
  };

  /**
   * @brief Internal data (should not be null).
   */
  std::shared_ptr<Data> data_;
};

template <typename GeneratorType, typename HandleType>
HandleGenerator<GeneratorType, HandleType>::HandleGenerator() : data_(std::make_shared<Data>()) {}

template <typename GeneratorType, typename HandleType>
template <class... Args>
const HandleType& HandleGenerator<GeneratorType, HandleType>::createHandle(Args... args) const {
  std::lock_guard<std::mutex> guard(data_->handles_lock);

  // Create handle and move it into handles map.
  // TODO
  std::unique_ptr<HandleType> handle =
    std::make_unique<HandleType>(/*static_cast<const GeneratorType&>(*this), args...*/);
  auto it = data_->handles.emplace(handle->id(), std::move(handle)).first;
  return *it->second;
}

template <typename GeneratorType, typename HandleType>
bool HandleGenerator<GeneratorType, HandleType>::hasHandle(const size_t id) const {
  return data_->handles.find(id) != data_->handles.end();
}

template <typename GeneratorType, typename HandleType>
const HandleType& HandleGenerator<GeneratorType, HandleType>::getHandle(const size_t id) const {
  const auto it = data_->handles.find(id);

  if (it == data_->handles.end()) {
    throw std::runtime_error("Failed to find the requested handle.");
  }

  return *it->second;
}

template <typename GeneratorType, typename HandleType>
void HandleGenerator<GeneratorType, HandleType>::destroyHandle(const HandleType& handle) const {
  std::lock_guard<std::mutex> guard(data_->handles_lock);

  // If the handle is found free its resources and erase it.
  auto it = data_->handles.find(handle.id());
  if (it != data_->handles.end()) {
    static_cast<Handle*>(it->second.get())->free();
    data_->handles.erase(it);
  }
}

template <typename GeneratorType, typename HandleType>
void HandleGenerator<GeneratorType, HandleType>::destroyAllHandles() const {
  std::lock_guard<std::mutex> guard(data_->handles_lock);

  // Remove all handles.
  for (auto& handle : data_->handles) {
    static_cast<Handle*>(handle.second.get())->free();
  }

  data_->handles.clear();
}

} // namespace logi

#endif