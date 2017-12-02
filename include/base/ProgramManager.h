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
#include "VulkanDevice.h"

namespace vkr {
namespace filesystem = std::experimental::filesystem;  ///< todo(plavric): Change when the filesystem will no longer be experimental.

class ProgramManager {
public:
	/**
	 * @brief Retrieve reference to singleton (lazily evaluated).
	 *
	 * @return Reference to the ProgramManager.
	 */
	static ProgramManager& instance();

	void initialize(const std::string& shaders_dir);

	/**
	 * @brief Loads shaders for the given device.
	 */
	void loadShaders(VulkanDevice* device);

	ProgramManager(ProgramManager const&) = delete;
	void operator=(ProgramManager const&) = delete;

private:
	struct ShaderMeta {
		std::string id;
		filesystem::path shader_path;
		vk::ShaderStageFlagBits stage;

		ShaderMeta(std::string id, filesystem::path shader_path, vk::ShaderStageFlagBits stage) : id(id), shader_path(shader_path), stage(stage) { }
	};

	struct PipelineMeta {
		std::string id;
		std::vector<size_t> shaders_indices;

		PipelineMeta(std::string id, std::vector<size_t> shaders_indices) : id(id), shaders_indices(shaders_indices) {}
	};

	std::vector<ShaderMeta> shaders_metadata_;
	std::vector<PipelineMeta> pipelines_metadata_;
	
	bool initialized_;

	ProgramManager();

	void loadMetaData(const filesystem::path& shaders_dir);

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