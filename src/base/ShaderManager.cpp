#include "base/ShaderManager.h"
#include <fstream>
#include <json.hpp>

namespace vkr {

static const std::map<std::string, vk::ShaderStageFlagBits> stage_map{
	{ "fragment", vk::ShaderStageFlagBits::eFragment },
	{ "vertex", vk::ShaderStageFlagBits::eVertex },
	{ "compute", vk::ShaderStageFlagBits::eCompute }
};

ShaderManager::ShaderManager(const std::string & shaders_path) : shaders_path_(shaders_path) {
	loadShaders();
}

void ShaderManager::loadShaders() {
	using json = nlohmann::json;

	std::ifstream shader_map_file(filesystem::canonical(filesystem::path(shaders_path_) / filesystem::path("shaders_map.json")), std::ios::in);

	// Check if shader_map.json exists.
	if (!shader_map_file.is_open()) {
		throw std::runtime_error("There is no \"shaders_map.json\" file in the \"" + shaders_path_.string() + "\".");
	}

	// Parse json.
	json shader_map_j;
	shader_map_file >> shader_map_j;

	for (json::iterator it = shader_map_j.begin(); it != shader_map_j.end(); ++it) {
		std::string module_name = it.key();
		ShaderModuleInfo sm_info;

		json module_shaders_j = it.value();
		for (auto& shader_j : module_shaders_j) {
			// Check if the shader entry contains the required data.
			if (shader_j.count("stage") == 0 || shader_j.count("path") == 0) {
				throw std::runtime_error("Badly formatted \"shaders_map.json\".");
			}

			// Check if the stage is supported.
			auto stage_it = stage_map.find(shader_j["stage"]);
			if (stage_it == stage_map.end()) {
				throw std::runtime_error("Unknown shader module stage \"" + filesystem::canonical(filesystem::path(shaders_path_) / shader_j["stage"].get<std::string>()).string() + "\".");
			}

			// Add shader stage entry.
			sm_info.shaders.push_back(std::make_pair(stage_it->second, filesystem::canonical(filesystem::path(shaders_path_) / shader_j["path"].get<std::string>())));
		}

		shader_modules_[module_name] = sm_info;
	}
}

vk::ShaderModule ShaderManager::getShaderModule(const std::string & name) {
	return vk::ShaderModule();
}

std::vector<char> ShaderManager::readShaderFile(const filesystem::path& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	// Check if the shader file was successfully openeed.
	if (!file.is_open()) {
		throw std::runtime_error("Failed to load shader file: \"" + filename.string() + "\". Make sure that the file exists.");
	}

	size_t file_size = (size_t)file.tellg();
	std::vector<char> buffer(file_size);

	file.seekg(0);
	file.read(buffer.data(), file_size);
	file.close();

	return buffer;
}

}