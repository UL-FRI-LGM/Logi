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
#include <map>
#include <vulkan/vulkan.hpp>
#include <filesystem>
#include "program_management/ShaderManager.h"
#include "program_management/PipelineLayout.h"
#include "program_management/ComputePipeline.h"
#include "base/Types.h"

namespace vkr {
namespace filesystem = std::experimental::filesystem;  ///< todo(plavric): Change when the filesystem will no longer be experimental.

class ProgramManager {
public:
	/**
	 * @brief Default constructor.
	 *
	 * @param device Logical device handle.
	 */
	ProgramManager(const vk::Device& device);

	/**
	 * @bref Loads shader programs and pipeline layouts from the given directory.
	 *
	 * @param shaders_dir Path to the shader directories.
	 */
	void loadPrograms(const filesystem::path& shaders_dir);

	PipelineLayout* getPipelineLayout(const std::string& name);

	/**
	 * @brief Creates compute pipeline using the pipeline layout with the given id.
	 *
	 * @param pipeline_id Pipeline layout identifier.
	 * @return Compute pipeline.
	 */
	ComputePipeline* getComputePipeline(compute_id_t id);

	compute_id_t getComputePipelineId(const std::string& name);

	/**
	 * @brief Returns descriptors count for all pipeline layouts.
	 *
	 * @return DescriptorCount object.
	 */
	DescriptorsCount getDescriptorsCount();

	/**
	 * @brief Free resources.
	 */
	~ProgramManager();

private:
	vk::Device device_;

	std::unique_ptr<ShaderManager> shader_manager_;
	std::vector<std::unique_ptr<PipelineLayout>> pipeline_layouts_;
	std::vector<std::unique_ptr<ComputePipeline>> compute_pipelines_;

	/**
	* @brief Read raw data from the SPIRV shader file located on the given path.
	* @throw std::runtime_error if something hoes wrong while reading the shader file.
	*
	* @param path Path to the SPIRV shader file.
	* @return Raw SPIRV shader data.
	*/
	std::vector<uint32_t> readShaderFile(const filesystem::path& path);
};

}

#endif