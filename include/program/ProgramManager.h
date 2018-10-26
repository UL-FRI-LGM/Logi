/*
* Shader manager.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef PROGRAM_MANAGER_H
#define PROGRAM_MANAGER_H

#if defined(__GNUC__)
#include <experimental/filesystem>
#else
#include <filesystem>
#endif
#include <vector>
#include <vulkan/vulkan.hpp>

#include "base/Handle.h"
#include "program/layout/PipelineLayout.h"
#include "program/layout/Shader.h"
#include "render_pass/RenderPass.h"
#include "render_pass/ComputePipeline.h"

namespace logi {
namespace filesystem = std::experimental::filesystem;  ///< todo(plavric): Change when the filesystem will no longer be experimental.

struct GraphicalPipelineCreateInfo {
	GraphicalPipelineCreateInfo() = default;

	GraphicalPipelineCreateInfo(const RenderPass& render_pass, const uint32_t subpass_index, const PipelineLayout& layout, const PipelineState& state)
		: render_pass(render_pass), subpass_index(subpass_index), layout(layout), state(state) { }

	RenderPass render_pass{};
	uint32_t subpass_index{};
	PipelineLayout layout{};
	PipelineState state{};
};

struct ComputePipelineCreateInfo {
	explicit ComputePipelineCreateInfo(const PipelineLayout& layout = {}, const vk::PipelineCreateFlags& flags = {})
		: layout(layout), flags(flags) { }

	PipelineLayout layout;
	vk::PipelineCreateFlags flags;
};



class ProgramManager : public Handle {
public:
	/**
	 * @brief	Default placeholder constructor.
	 */
	ProgramManager();

	/**
	 * @brief	Default constructor.
	 *
	 * @param	device	Vulkan logical device handle.
	 */
    explicit ProgramManager(const vk::Device& device);

	/**
	 * @brief	Creates pipeline layout that consist of the given shaders. Shader reflection is performed to extract the PipelineLayout data.
	 *
	 * @param	shader_stages	Vector containing Shader handles.
	 * @return	PipelineLayout handle.
	 */
	PipelineLayout createPipelineLayout(const std::vector<PipelineShaderStage>& shader_stages);

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
	vk::Device device_;
	std::shared_ptr<HandleManager> handle_manager_;

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
                                              const ExtTypes& extensions, ...) const {
    ShaderModuleConfig config(readShaderFile(path), flags);
    config.addExtensions(extensions...);
    return handle_manager_->createHandle<ShaderModule>(device_, config);
}

}

#endif