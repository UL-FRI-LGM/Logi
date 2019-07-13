/**
 * Project Logi source code
 * Copyright (C) 2019 Primoz Lavric
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LOGI_BASE_HANDLE_HPP
#define LOGI_BASE_HANDLE_HPP

#include <memory>

namespace logi {

template <typename T>
class Handle {
 public:
  /**
   * @brief Default null handle constructor.
   */
  Handle() = default;

  /**
   * @brief Initialize the referred object.
   *
   * @param object  Referred object.
   */
  explicit Handle(std::shared_ptr<T> object);

  /**
   * @brief     Retrieve object identifier.
   *
   * @return    Object identfier.
   */
  size_t id() const;

  /**
   * @brief Converts to true if the referred VulkanObject is valid
   */
  operator bool() const;

  /**
   * @brief Check if the given handles refer to the same object.
   */
  bool operator==(const Handle<T>& other) const;

  /**
   * @brief Check if the given handles do not refer to the same object.
   */
  bool operator!=(const Handle<T>& other) const;

  /**
   * @brief Return true if the referred VulkanObject is not valid.
   */
  bool operator!() const;

 protected:
  std::shared_ptr<T> object_;
};

template <typename T>
Handle<T>::Handle(std::shared_ptr<T> object) : object_(std::move(object)) {}

template <typename T>
size_t Handle<T>::id() const {
  return object_->id();
}

template <typename T>
Handle<T>::operator bool() const {
  return object_ && object_->valid();
}

template <typename T>
bool Handle<T>::operator==(const Handle<T>& other) const {
  return object_ == other.object_;
}

template <typename T>
bool Handle<T>::operator!=(const Handle<T>& other) const {
  return object_ != other.object_;
}

template <typename T>
bool Handle<T>::operator!() const {
  return !object_ || !object_->valid();
}

} // namespace logi

#endif // LOGI_BASE_HANDLE_HPP
