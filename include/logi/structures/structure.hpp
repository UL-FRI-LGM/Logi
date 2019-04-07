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

#ifndef LOGI_STRUCTURES_STRUCTURE_HPP
#define LOGI_STRUCTURES_STRUCTURE_HPP

#include <optional>
#include <tuple>
#include <type_traits>
#include <vector>

namespace logi {

template <typename VkStructureType>
class SimpleStructure {
 public:
  virtual VkStructureType build() const = 0;
};

template <typename VkStructureType>
class Structure : protected VkStructureType {
  template <typename First, typename... Rest>
  friend class StructureChain;

 public:
  using VkType = VkStructureType;

  VkStructureType& build() {
    updateVkStructure();
    return *this;
  }

  /**
   * @brief Updates pointers and sizes in the Vulkan structure.
   */
  virtual void updateVkStructure(){};

 protected:
  template <typename EntryType>
  static void vecToCArr(const std::vector<EntryType>& vec, const EntryType*& data, uint32_t& size) {
    data = vec.empty() ? nullptr : vec.data();
    size = vec.size();
  }

  template <typename EntryType>
  static void vecLogiToVk(std::vector<EntryType>& logi_vec, std::vector<typename EntryType::VkType>& vk_vec) {
    vk_vec.clear();
    vk_vec.reserve(logi_vec.size());
    for (auto& entry : logi_vec) {
      vk_vec.emplace_back(entry.build());
    }
  }
};

template <typename First, typename... Rest>
class StructureChain {
 public:
  explicit StructureChain(bool enable_all = false) {
    if (enable_all) {
      enable<Rest...>();
    }
  }

  const typename First::VkType& build() {
    typename First::VkType& vk_first = first_.build();
    const void** p_next = &vk_first.pNext;

    auto builder = [&p_next](auto& structure) {
      if (structure.has_value()) {
        auto& vk_struct = structure.value().build();
        *p_next = &vk_struct;
        p_next = &vk_struct.pNext;
      }
    };

    std::apply([&builder](auto&... ext) { (..., builder(ext)); }, chain_);

    return vk_first;
  }

  template <typename T, typename... Remaining>
  void enable() {
    std::get<std::optional<T>>(chain_).emplace();
    if constexpr (sizeof...(Remaining) > 0u) {
      enable<Remaining...>();
    }
  }

  template <typename T, typename... Remaining>
  void disable() {
    std::get<std::optional<T>>(chain_).reset();
    if constexpr (sizeof...(Remaining) > 0u) {
      disable<Remaining...>();
    }
  }

  template <typename T>
  bool isEnabled() const {
    if constexpr (std::is_same_v<First, T>()) {
      return true;
    } else {
      return std::get<std::optional<T>>(chain_).has_value();
    }
  }

  template <typename T>
  T& get() {
    if constexpr (std::is_same_v<First, T>) {
      return first_;
    } else {
      return std::get<std::optional<T>>(chain_).value();
    }
  }

  template <typename T>
  void set(const T& structure) {
    if constexpr (std::is_same_v<First, T>) {
      first_ = structure;
    } else {
      std::get<std::optional<T>>(chain_) = structure;
    }
  }

  operator const typename First::VkType&() {
    return build();
  }

 private:
  First first_;
  std::tuple<std::optional<Rest>...> chain_;
};

} // namespace logi

#endif // LOGI_STRUCTURES_STRUCTURE_HPP
