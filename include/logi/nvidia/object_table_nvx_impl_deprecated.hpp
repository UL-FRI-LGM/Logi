/**
 * Project Logi source code
 * Copyright (C) 2019 Lana Besevic
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

#ifndef LOGI_NVIDIA_OBJECT_TABLE_NVX_IMPL_HPP
#define LOGI_NVIDIA_OBJECT_TABLE_NVX_IMPL_HPP

#include <optional>
#include "logi/base/common.hpp"
#include "logi/base/vulkan_object.hpp"

namespace logi {

// class VulkanInstanceImpl;
// class PhysicalDeviceImpl;
// class LogicalDeviceImpl;

// class ObjectTableNVXImpl : public VulkanObject, public std::enable_shared_from_this<ObjectTableNVXImpl> {
//  public:
//   ObjectTableNVXImpl(LogicalDeviceImpl& logicalDevice, const vk::ObjectTableCreateInfoNVX& createInfo,
//                      const std::optional<vk::AllocationCallbacks>& allocator = {});

//   // region Vulkan Declarations

//   vk::ResultValueType<void>::type
//     registerObjectsNVX(vk::ArrayProxy<const vk::ObjectTableEntryNVX* const> objectTableEntries,
//                        vk::ArrayProxy<const uint32_t> objectIndices) const;

//   vk::ResultValueType<void>::type unregisterObjectsNVX(vk::ArrayProxy<const vk::ObjectEntryTypeNVX> objectEntryTypes,
//                                                        vk::ArrayProxy<const uint32_t> objectIndices) const;

//   // endregion

//   // region Logi Declarations

//   VulkanInstanceImpl& getInstance() const;

//   PhysicalDeviceImpl& getPhysicalDevice() const;

//   LogicalDeviceImpl& getLogicalDevice() const;

//   const vk::DispatchLoaderDynamic& getDispatcher() const;

//   void destroy() const;

//   operator const vk::ObjectTableNVX&() const;

//  protected:
//   void free() override;

//   // endregion

//  private:
//   LogicalDeviceImpl& logicalDevice_;
//   std::optional<vk::AllocationCallbacks> allocator_;
//   vk::ObjectTableNVX vkObjectTableNVX_;
// };
} // namespace logi

#endif // LOGI_NVIDIA_OBJECT_TABLE_NVX_IMPL_HPP
