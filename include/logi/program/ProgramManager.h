#include <utility>

/*
 * Shader manager.
 *
 * Copyright (C) 2017 by Primoz Lavric
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PROGRAM_MANAGER_H
#define PROGRAM_MANAGER_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/program/layout/PipelineLayout.h"
#include "logi/program/layout/Shader.h"
#include "logi/program/render_pass/ComputePipeline.h"
#include "logi/program/render_pass/RenderPass.h"

namespace logi {

struct GraphicalPipelineCreateInfo {
  GraphicalPipelineCreateInfo() = default;

  GraphicalPipelineCreateInfo(const RenderPass& render_pass, const uint32_t subpass_index, PipelineLayout layout,
                              PipelineState state)
    : render_pass(render_pass), subpass_index(subpass_index), layout(std::move(layout)), state(std::move(state)) {}

  RenderPass render_pass{};
  uint32_t subpass_index{};
  PipelineLayout layout{};
  PipelineState state{};
};

struct ComputePipelineCreateInfo {
  explicit ComputePipelineCreateInfo(PipelineLayout layout = {}, const vk::PipelineCreateFlags& flags = {})
    : layout(std::move(layout)), flags(flags) {}

  PipelineLayout layout;
  vk::PipelineCreateFlags flags;
};

class LogicalDevice;

class ProgramManager : public OwnedHandle<LogicalDevice>,
                       public HandleGenerator<ProgramManager, ShaderModule>,
                       public HandleGenerator<ProgramManager, PipelineLayout>,
                       public HandleGenerator<ProgramManager, RenderPass> {
 public:
  /**
   * @brief	Default placeholder constructor.
   */
  ProgramManager() = default;

  /**
   * @brief	Default constructor.
   *
   * @param	device	Logical device handle.
   */
  explicit ProgramManager(const LogicalDevice& device);

  /**
   * @brief	Creates pipeline layout that consist of the given shaders. Shader reflection is performed to extract the
   * PipelineLayout data.
   *
   * @param	shader_stages	Vector containing Shader handles.
   * @return	PipelineLayout handle.
   */
  PipelineLayout createPipelineLayout(const std::vector<PipelineShaderStage>& shader_stages) const;

  /**
   * @brief	Creates Shader handle object and populates it with the given ShaderData.
   *
   * @param	config  Shader module configuration.
   * @return	Shader handle.
   */
  ShaderModule createShaderModule(const ShaderModuleConfig& config) const;

  /**
   * @brief	Load shader file and create a ShaderModule using the loaded code.
   *
   * @param	path    Path to the shader file.
   * @param	flags   Additional ShaderModule configuration flags.
   * @return	Created ShaderModule.
   */
  ShaderModule loadShaderModule(const std::string& path, const vk::ShaderModuleCreateFlags& flags = {}) const;

  /**
   * @brief	Load shader file and create a ShaderModule using the loaded code.
   *
   * @tparam	ExtTypes    Types of extensions.
   * @param	path        Path to the shader file.
   * @param	flags       Additional ShaderModule configuration flags.
   * @param	extensions  ShaderModule extensions.
   * @return	Created ShaderModule.
   */
  template <typename ExtTypes>
  ShaderModule loadShaderModule(const std::string& path, const vk::ShaderModuleCreateFlags& flags,
                                const ExtTypes& extensions...) const;

  /**
   * @brief	TODO
   *
   * @param	layout
   * @return
   */
  RenderPass createRenderPass(const RenderPassLayout& layout) const;

  std::vector<GraphicalPipeline> createGraphicalPipelines(std::vector<GraphicalPipelineCreateInfo>& create_infos) const;

  std::vector<ComputePipeline> createComputePipelines(const std::vector<ComputePipelineCreateInfo>& create_infos) const;

 protected:
  /**
   * @brief	Destroys all Shader and PipelineLayout handles.
   */
  void free() override;

 private:
  /**
   * @brief Read raw data from the SPIRV shader file located on the given path.
   * @throw std::runtime_error if something goes wrong while reading the shader file.
   *
   * @param path Path to the SPIRV shader file.
   * @return Raw SPIRV shader data.
   */
  static std::vector<uint32_t> readShaderFile(const std::string& path);
};

template <typename ExtTypes>
ShaderModule ProgramManager::loadShaderModule(const std::string& path, const vk::ShaderModuleCreateFlags& flags,
                                              const ExtTypes& extensions...) const {
  ShaderModuleConfig config(readShaderFile(path), flags);
  config.addExtensions(extensions);
  return HandleGenerator<ProgramManager, ShaderModule>::createHandle(config);
}

} // namespace logi

#endif