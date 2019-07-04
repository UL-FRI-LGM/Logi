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

#ifndef LOGI_STRUCTURES_EXTENSION_HPP
#define LOGI_STRUCTURES_EXTENSION_HPP

namespace logi {

template <typename BaseConfigType>
class VkNextProxy {
 public:
  VkNextProxy() : next_(nullptr) {}

  template <typename NextConfigType>
  VkNextProxy(NextConfigType& next) : next_(&next) {
    static_assert(vk::isStructureChainValid<BaseConfigType, NextConfigType>::value,
                  "NextConfigType must extend BaseConfigType");
  }

  operator void*() const {
    return next_;
  }

 private:
  void* next_;
};

template <typename BaseConfigType>
class ConstVkNextProxy {
 public:
  ConstVkNextProxy() : next_(nullptr) {}

  template <typename NextConfigType>
  ConstVkNextProxy(const NextConfigType& next) : next_(&next) {
    static_assert(vk::isStructureChainValid<BaseConfigType, NextConfigType>::value,
                  "NextConfigType must extend BaseConfigType");
  }

  operator const void*() const {
    return next_;
  }

 private:
  const void* next_;
};

} // namespace logi

#endif // LOGI_STRUCTURES_EXTENSION_HPP
