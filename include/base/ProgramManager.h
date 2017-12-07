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
	enum class PipelineType {
		UNKNOWN,
		COMPUTE,
		GRAPHIC
	};

	struct DescriptorBindingMeta {
		uint32_t set;
		uint32_t binding;
		vk::DescriptorType type;
		uint32_t count; // Number of array entries.
	};

	struct ShaderMeta {
		std::string id;
		filesystem::path shader_path;
		vk::ShaderStageFlagBits stage;
		
		// Vertex attributes meta data.
		std::unique_ptr<const std::vector<vk::VertexInputAttributeDescription>> vtx_attribute_descriptions;
		std::unique_ptr<const std::vector<vk::VertexInputBindingDescription>> vtx_bindings_descriptions;

		// Descriptors meta data.
		std::vector<DescriptorBindingMeta> descriptor_set_meta;
		
		bool initialized; ///< True if ShadeMetadata is initialized.

		ShaderMeta(std::string id, filesystem::path shader_path, vk::ShaderStageFlagBits stage) : id(id), shader_path(shader_path), stage(stage), vtx_attribute_descriptions(nullptr), vtx_bindings_descriptions(nullptr), descriptor_set_meta(), initialized(false) { }
	};

	struct PipelineMeta {
		std::string id;
		std::vector<size_t> shaders_indices;
		std::unique_ptr<vk::PipelineVertexInputStateCreateInfo> attributes_info;
		std::unique_ptr<std::vector<const vk::DescriptorSetLayoutBinding>> descriptor_set_layouts;
		
		PipelineType type;

		PipelineMeta(std::string id, std::vector<size_t> shaders_indices) : id(id), shaders_indices(shaders_indices), attributes_info(nullptr), descriptor_set_layouts(nullptr), type(PipelineType::UNKNOWN) {}
	};

	std::vector<ShaderMeta> shaders_metadata_;
	std::vector<PipelineMeta> pipelines_metadata_;
	
	bool initialized_;

	ProgramManager();

	void initializeShaderDescriptorMeta(ShaderMeta& shader_meta_entry, const std::vector<uint32_t>& code);

	void ProgramManager::initializePipelineLayout(PipelineMeta& pipeline_meta_entry);

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