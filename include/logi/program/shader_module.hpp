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

#ifndef LOGI_PROGRAM_SHADER_MODULE_HPP
#define LOGI_PROGRAM_SHADER_MODULE_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/handle.hpp"
#include "logi/program/shader_module_impl.hpp"

namespace logi {

class ShaderModuleImpl;
class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;

class ShaderModule : public Handle<ShaderModuleImpl> {
 public:
  using Handle::Handle;

  // region Logi Declarations

  std::vector<std::string> getEntryPointNames() const;

  const EntryPointReflectionInfo& getEntryPointReflectionInfo(const std::string& entryPointName) const;

  const std::vector<std::vector<DescriptorBindingReflectionInfo>>&
    getDescriptorSetReflectionInfo(const std::string& entryPointName) const;

  const std::vector<PushConstantReflectionInfo>& getPushConstantReflectionInfo(const std::string& entryPointName) const;

  const std::vector<VertexAttributeReflectionInfo>&
    getVertexAttributeReflectionInfo(const std::string& entryPointName) const;

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::ShaderModule&() const;

  // endregion
};

struct ShaderStage {
  ShaderStage(ShaderModule shader, std::string entryPoint);

  ShaderModule shader;
  std::string entryPoint;
};

/*
std::vector<std::vector<DescriptorBindingReflectionInfo>> reflectDescriptorSets(const std::vector<ShaderStage>& stages);

std::vector<PushConstantReflectionInfo> reflectPushConstants(const std::vector<ShaderStage>& stages);
*/

} // namespace logi

#endif // LOGI_PROGRAM_SHADER_MODULE_HPP
