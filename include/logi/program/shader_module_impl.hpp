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

#include <map>
#include <optional>
#include <spirv_cross.hpp>
#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;

struct SpecializationConstantReflectionInfo {
  SpecializationConstantReflectionInfo(uint32_t id, uint32_t size);

  std::string name;
  uint32_t id;
  uint32_t size;
};

struct DescriptorBindingReflectionInfo {
  DescriptorBindingReflectionInfo(std::string name, const vk::ShaderStageFlags& stages, uint32_t binding,
                                  vk::DescriptorType descriptorType, uint32_t descriptorCount);

  std::string name;
  vk::ShaderStageFlags stages;
  uint32_t binding;
  vk::DescriptorType descriptorType;
  uint32_t descriptorCount;

  explicit operator vk::DescriptorSetLayoutBinding() const {
    return vk::DescriptorSetLayoutBinding(binding, descriptorType, descriptorCount, stages, nullptr);
  }
};

struct DescriptorSetReflectionInfo {
  explicit DescriptorSetReflectionInfo(uint32_t set, std::vector<DescriptorBindingReflectionInfo> bindings = {});

  uint32_t set;
  std::vector<DescriptorBindingReflectionInfo> bindings;
};

struct VertexAttributeReflectionInfo {
  VertexAttributeReflectionInfo(std::string name, uint32_t location, uint32_t elementSize, vk::Format format);

  std::string name;
  uint32_t location;
  uint32_t elementSize;
  vk::Format format;
};

struct PushConstantReflectionInfo {
  PushConstantReflectionInfo(const vk::ShaderStageFlags& stages, uint32_t offset, uint32_t size);

  vk::ShaderStageFlags stages;
  uint32_t offset;
  uint32_t size;

  explicit operator vk::PushConstantRange() const {
    return vk::PushConstantRange(stages, offset, size);
  }
};

struct EntryPointReflectionInfo {
  EntryPointReflectionInfo(std::string name, vk::ShaderStageFlagBits stage,
                           std::vector<DescriptorSetReflectionInfo> descriptorSets = {},
                           std::optional<PushConstantReflectionInfo> pushConstants = {},
                           std::vector<VertexAttributeReflectionInfo> vertexAttributes = {});

  std::string name;
  vk::ShaderStageFlagBits stage;
  std::vector<DescriptorSetReflectionInfo> descriptorSets;
  std::optional<PushConstantReflectionInfo> pushConstants;
  std::vector<VertexAttributeReflectionInfo> vertexAttributes;
};

class ShaderModuleImpl : public VulkanObject, public std::enable_shared_from_this<ShaderModuleImpl> {
 public:
  ShaderModuleImpl(LogicalDeviceImpl& logicalDevice, const vk::ShaderModuleCreateInfo& createInfo,
                   const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Logi Declarations

  std::vector<std::string> getEntryPointNames() const;

  const EntryPointReflectionInfo& getEntryPointReflectionInfo(const std::string& entryPointName) const;

  const std::vector<DescriptorSetReflectionInfo>&
    getDescriptorSetReflectionInfo(const std::string& entryPointName) const;

  const std::optional<PushConstantReflectionInfo>&
    getPushConstantReflectionInfo(const std::string& entryPointName) const;

  const std::vector<VertexAttributeReflectionInfo>&
    getVertexAttributeReflectionInfo(const std::string& entryPointName) const;

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::ShaderModule&() const;

 protected:
  void reflect(const uint32_t* pCode, size_t codeSize);

  static std::vector<DescriptorSetReflectionInfo> reflectDescriptorSets(const spirv_cross::Compiler& compiler,
                                                                        vk::ShaderStageFlagBits stage);

  static std::optional<PushConstantReflectionInfo> reflectPushConstants(const spirv_cross::Compiler& compiler,
                                                                        vk::ShaderStageFlagBits stage);

  static std::vector<VertexAttributeReflectionInfo> reflectVertexAttributes(const spirv_cross::Compiler& compiler,
                                                                            vk::ShaderStageFlagBits stage);

  void free() override;

  static vk::ShaderStageFlagBits executionModelToStage(spv::ExecutionModel execModel);

  static vk::Format SPIRTypeToVertexBufferFormat(const spirv_cross::SPIRType& formatInfo);

  // endregion

 private:
  LogicalDeviceImpl& logicalDevice_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::ShaderModule vkShaderModule_;

  std::map<std::string, EntryPointReflectionInfo> reflectionData_;
};

} // namespace logi

#endif // LOGI_PROGRAM_SHADER_MODULE_IMPL_HPP
