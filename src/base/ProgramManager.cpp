#include "base/ProgramManager.h"
#include <fstream>
#include <json.hpp>
#include <vulkan/vulkan.hpp>
#include <spirv_cross.hpp>
#include "base\ProgramManager.h"
#include "base\ProgramManager.h"

namespace vkr {

static const std::map<std::string, vk::ShaderStageFlagBits> stage_map{
	{ "vertex", vk::ShaderStageFlagBits::eVertex },
	{ "geometry", vk::ShaderStageFlagBits::eGeometry },
	{ "fragment", vk::ShaderStageFlagBits::eFragment },
	{ "compute", vk::ShaderStageFlagBits::eCompute }
};

ProgramManager::ProgramManager() : shaders_metadata_(), pipelines_metadata_(), initialized_(false) { }

ProgramManager& ProgramManager::instance() {
	static ProgramManager instance;
	return instance;
}

void ProgramManager::initialize(const std::string & shaders_dir) {
	if (initialized_) {
		std::wcerr << "Program manager is already initialized.";
		return;
	}

	loadMetaData(shaders_dir);
	initialized_ = true;
}

void ProgramManager::loadShaders(VulkanDevice* device) {
	for (ShaderMeta shader_meta : shaders_metadata_) {
		std::vector<uint32_t> code = readShaderFile(shader_meta.shader_path);

		spirv_cross::Compiler comp(code);
		// The SPIR-V is now parsed, and we can perform reflection on it.
		spirv_cross::ShaderResources resources = comp.get_shader_resources();

		for (auto& resource : resources.stage_inputs) {
			uint32_t loc = comp.get_decoration(resource.id, spv::DecorationLocation);
			uint32_t bind = comp.get_decoration(resource.id, spv::DecorationBinding);
			auto type = comp.get_type_from_variable(resource.id);
			std::cout << loc;
			spirv_cross::SPIRType::BaseType::
		}

		// Get all sampled images in the shader.
		for (auto &resource : resources.sampled_images) {
			unsigned set = comp.get_decoration(resource.id, spv::DecorationDescriptorSet);
			unsigned binding = comp.get_decoration(resource.id, spv::DecorationBinding);

			std::cout << "binding" << binding;
		}
	}
}

void ProgramManager::loadMetaData(const filesystem::path & shaders_dir) {
	using json = nlohmann::json;

	std::ifstream shader_meta_file(filesystem::canonical(filesystem::path(shaders_dir) / filesystem::path("shaders_metadata.json")), std::ios::in);

	// Check if shader_map.json exists.
	if (!shader_meta_file.is_open()) {
		throw std::runtime_error("There is no \"shaders_metadata.json\" file in the \"" + shaders_dir.string() + "\".");
	}

	// Parse json.
	json parser;
	shader_meta_file >> parser;

	// Validate json root container.
	if (parser.find("shaders") == parser.end() || parser.find("pipelines") == parser.end()) {
		throw std::runtime_error("Could not find required root elements in \"shaders_metadata.json\".");
	}

	// Used to map shaders to pipelines using indices instead of strings.
	std::map<std::string, size_t> shader_to_idx;

	// Parse shader data
	for (auto& shader : parser["shaders"]) {
		// Validate shader containter.
		if (shader.find("id") == shader.end() || shader.find("path") == shader.end() || shader.find("stage") == shader.end()) {
			throw std::runtime_error("Encountered incomplete shader entry while parsing \"shaders_metadata.json\".");
		}

		// Fetch the data.
		std::string id = shader["id"].get<std::string>();
		filesystem::path shader_path = filesystem::canonical(filesystem::path(shaders_dir) / shader["path"].get<std::string>());
		std::string stage = shader["stage"].get<std::string>();

		auto stage_it = stage_map.find(stage);
		// Validate stage data.
		if (stage_it == stage_map.end()) {
			throw std::runtime_error("Invalid stage. Shader id: \"" + id + "\".");
		}

		// Validate shader path.
		if (!filesystem::is_regular_file(shader_path)) {
			throw std::runtime_error("Invalid shader path. Shader id: \"" + id + "\".");
		}

		// Store shader meta.
		if (shader_to_idx.find(id) == shader_to_idx.end()) {
			shader_to_idx[id] = shaders_metadata_.size();
			shaders_metadata_.emplace_back(ShaderMeta(id, shader_path, stage_it->second));
		}
		else {
			throw std::runtime_error("Duplicate shader id: \"" + id + "\".");
		}
	}

	// Parse pipeline data
	for (auto& pipeline : parser["pipelines"]) {
		// Validate pipeline container.
		if (pipeline.find("id") == pipeline.end() || pipeline.find("shaders") == pipeline.end()) {
			throw std::runtime_error("Encountered incomplete pipeline entry while parsing \"shaders_metadata.json\".");
		}

		std::string id = pipeline["id"].get<std::string>();
		std::vector<size_t> shaders_indices;
		
		// Fetch shader indices.
		for (auto& shader : pipeline["shaders"]) {
			auto& shader_idx = shader_to_idx.find(shader.get<std::string>());
			// Check if the shader is defined.
			if (shader_idx == shader_to_idx.end()) {
				throw std::runtime_error("Encountered undefined shader in pipeline. Pipeline id: \"" + id + "\".");
			}

			shaders_indices.push_back(shader_idx->second);
		}

		// Store pipeline metadata.
		pipelines_metadata_.push_back(PipelineMeta(id, shaders_indices));
	}
}

std::vector<uint32_t> ProgramManager::readShaderFile(const filesystem::path& path) {
	std::ifstream file(path, std::ios::ate | std::ios::binary);

	// Check if the shader file was successfully openeed.
	if (!file.is_open()) {
		throw std::runtime_error("Failed to load shader file: \"" + path.string() + "\". Make sure that the file exists.");
	}

	size_t file_size = (size_t)file.tellg();
	std::vector<uint32_t> buffer(file_size / sizeof(uint32_t));

	file.seekg(0);
	file.read(reinterpret_cast<char*>(buffer.data()), file_size);
	file.close();

	return buffer;
}

}