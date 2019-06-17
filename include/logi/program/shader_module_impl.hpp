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

#ifndef LOGI_PROGRAM_SHADER_MODULE_IMPL_HPP
#define LOGI_PROGRAM_SHADER_MODULE_IMPL_HPP

#include <optional>
#include <spirv_cross.hpp>
#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;

struct EntryPointInfo {
  EntryPointInfo(std::string name, vk::ShaderStageFlagBits stage);

  std::string name;
  vk::ShaderStageFlagBits stage;
};

struct SpecializationConstantInfo {
  SpecializationConstantInfo(uint32_t id, uint32_t size);

  std::string name;
  uint32_t id;
  uint32_t size;
};

struct DescriptorBindingInfo {
  DescriptorBindingInfo(std::string name, uint32_t binding, vk::DescriptorType descriptorType,
                        uint32_t descriptorCount);

  std::string name;
  uint32_t binding;
  vk::DescriptorType descriptorType;
  uint32_t descriptorCount;
};

struct VertexAttributeInfo {
  VertexAttributeInfo(std::string name, uint32_t location, uint32_t binding, uint32_t elementSize, vk::Format format);

  std::string name;
  uint32_t location;
  uint32_t binding;
  uint32_t elementSize;
  vk::Format format;
};

struct PushConstantInfo {
  PushConstantInfo(std::string name, uint32_t offset, uint32_t size);

  std::string name;
  uint32_t offset;
  uint32_t size;
};

class ShaderModuleImpl : public VulkanObject<ShaderModuleImpl> {
 public:
  ShaderModuleImpl(LogicalDeviceImpl& logicalDevice, const vk::ShaderModuleCreateInfo& createInfo,
                   const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Vulkan Declarations

  // endregion

  // region Logi Declarations

  std::vector<EntryPointInfo> reflectEntryPoints() const;

  std::vector<std::vector<DescriptorBindingInfo>> reflectDescriptorSets() const;

  std::vector<PushConstantInfo> reflectPushConstants() const;

  std::vector<VertexAttributeInfo> reflectVertexAttributes() const;

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::ShaderModule() const;

 protected:
  void free() override;

  static vk::ShaderStageFlagBits executionModelToStage(spv::ExecutionModel execModel);

  static vk::Format SPIRTypeToVertexBufferFormat(const spirv_cross::SPIRType& formatInfo);

  // endregion

 private:
  LogicalDeviceImpl& logicalDevice_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::ShaderModule vkShaderModule_;

  spirv_cross::Compiler compiler_;
};

} // namespace logi

#endif // LOGI_PROGRAM_SHADER_MODULE_IMPL_HPP
