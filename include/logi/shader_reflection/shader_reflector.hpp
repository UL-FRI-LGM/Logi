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

#ifndef LOGI_SHADER_REFLECTION_SHADER_REFLECTOR_HPP
#define LOGI_SHADER_REFLECTION_SHADER_REFLECTOR_HPP

#include <numeric>
#include <spirv_cross.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>
#include "logi/structures/vk_structures.hpp"

namespace logi {

class ShaderReflector {
 public:
  void reflectShader(const uint32_t* code, size_t wordSize, const std::string& entryPoint);

  void reflectShader(const uint32_t* code, size_t wordSize, const std::vector<std::string>& entryPoints);

 protected:
  void reflectShaderInternal(spirv_cross::Compiler& compiler, const std::string& entryPoint);

  void reflectPushConstants(const spirv_cross::Compiler& compiler, const spirv_cross::ShaderResources& resources,
                            vk::ShaderStageFlagBits stage);

  void reflectDescriptorBinding(const spirv_cross::Compiler& compiler, vk::ShaderStageFlagBits stage,
                                uint32_t resourceId, vk::DescriptorType type);

  void reflectVertexAttributes(const spirv_cross::Compiler& compiler,
                               const spirv_cross::ShaderResources& shaderResources);

  static spv::ExecutionModel getEntryPointExecutionModel(const spirv_cross::Compiler& compiler,
                                                         const std::string& entryPointName);

  static vk::ShaderStageFlagBits executionModelToStage(spv::ExecutionModel execModel);

  vk::Format static SPIRTypeToVertexBufferFormat(const spirv_cross::SPIRType& format_info);

 private:
  vk::ShaderStageFlags stages;
  PipelineVertexInputStateCreateInfoChain vertexInputStateCI;
  std::vector<DescriptorSetLayoutCreateInfoChain> descriptorSets;
  PipelineLayoutCreateInfoChain layout;
};

} // namespace logi

#endif // LOGI_SHADER_REFLECTION_SHADER_REFLECTOR_HPP
