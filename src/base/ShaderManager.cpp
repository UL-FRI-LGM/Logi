#include "base/ShaderManager.h"
#include <fstream>
#include <json.hpp>
#include <vulkan/vulkan.hpp>
#include <tuple>

namespace vkr {

static const std::map<std::string, vk::ShaderStageFlagBits> stage_map{
	{ "fragment", vk::ShaderStageFlagBits::eFragment },
	{ "vertex", vk::ShaderStageFlagBits::eVertex },
	{ "compute", vk::ShaderStageFlagBits::eCompute }
};


void ShaderManager::loadShaders(const std::string& shaders_path) {
	using json = nlohmann::json;

	std::ifstream shader_map_file(filesystem::canonical(filesystem::path(shaders_path) / filesystem::path("shaders_map.json")), std::ios::in);

	// Check if shader_map.json exists.
	if (!shader_map_file.is_open()) {
		throw std::runtime_error("There is no \"shaders_map.json\" file in the \"" + shaders_path + "\".");
	}

	// Parse json.
	json shader_map_j;
	shader_map_file >> shader_map_j;

	for (json::iterator it = shader_map_j.begin(); it != shader_map_j.end(); ++it) {
		std::string pipeline_id = it.key();
		std::vector<ShaderModuleInfo> shader_modules;

		json module_shaders_j = it.value();
		for (auto& shader_j : module_shaders_j) {
			// Check if the shader entry contains the required data.
			if (shader_j.count("stage") == 0 || shader_j.count("path") == 0) {
				throw std::runtime_error("Badly formatted \"shaders_map.json\".");
			}

			// Check if the stage is supported.
			auto stage_it = stage_map.find(shader_j["stage"]);
			if (stage_it == stage_map.end()) {
				throw std::runtime_error("Unknown shader module stage \"" + filesystem::canonical(filesystem::path(shaders_path) / shader_j["stage"].get<std::string>()).string() + "\".");
			}

			// Add shader stage entry.
			ShaderModuleInfo sm_info;
			sm_info.stage = stage_it->second;
			sm_info.shader_path = filesystem::canonical(filesystem::path(shaders_path) / shader_j["path"].get<std::string>());
			shader_modules.push_back(sm_info);
		}

		shader_pipelines_[pipeline_id] = shader_modules;
	}
}

std::vector<vk::PipelineShaderStageCreateInfo> ShaderManager::getShaderPipelineStages(VulkanDevice& device, const std::string& pipeline_id) {
	// Check if the shader pipeline was already complied for this device.
	auto chached_pipeline_it = device.f_shader_cache_.find(pipeline_id);
	if (chached_pipeline_it == device.f_shader_cache_.end()) {
		return chached_pipeline_it->second;
	}

	// Check if the requested shader pipeline exists.
	auto pipeline_it = shader_pipelines_.find(pipeline_id);
	if (pipeline_it == shader_pipelines_.end()) {
		throw std::runtime_error("Could not find the shader pipeline with the given id \"" + pipeline_id + "\".");
	}

	vk::Device logical_device = device.getLogicalDeviceHandle();

	// Generate shader stages create info.
	std::vector<vk::PipelineShaderStageCreateInfo> stages;

	for (auto stage_info : pipeline_it->second) {
		std::vector<char> code = readShaderFile(stage_info.shader_path);

		vk::PipelineShaderStageCreateInfo shader_stage_info{};
		shader_stage_info.stage = stage_info.stage;
		shader_stage_info.module = createShaderModule(logical_device, code);
		shader_stage_info.pName = "main";

		stages.push_back(shader_stage_info);
	}

	// Cache the pipeline stages create infos.
	device.f_shader_cache_[pipeline_id] = stages;

	return stages;
}

vk::ShaderModule ShaderManager::createShaderModule(vk::Device& device, const std::vector<char>& code) {
	vk::ShaderModuleCreateInfo createInfo = {};
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	return device.createShaderModule(createInfo);
}

std::vector<char> ShaderManager::readShaderFile(const filesystem::path& path) {
	std::ifstream file(path, std::ios::ate | std::ios::binary);

	// Check if the shader file was successfully openeed.
	if (!file.is_open()) {
		throw std::runtime_error("Failed to load shader file: \"" + path.string() + "\". Make sure that the file exists.");
	}

	size_t file_size = (size_t)file.tellg();
	std::vector<char> buffer(file_size);

	file.seekg(0);
	file.read(buffer.data(), file_size);
	file.close();

	return buffer;
}

}