#include "program_management/ProgramManager.h"
#include <fstream>
#include <json.hpp>
#include <vulkan/vulkan.hpp>
#include <spirv_cross.hpp>
#include "util/FormatConversion.h"

namespace vkr {

static const std::map<std::string, vk::ShaderStageFlagBits> stage_map{
	{ "vertex", vk::ShaderStageFlagBits::eVertex },
	{ "geometry", vk::ShaderStageFlagBits::eGeometry },
	{ "fragment", vk::ShaderStageFlagBits::eFragment },
	{ "compute", vk::ShaderStageFlagBits::eCompute }
};

ProgramManager::ProgramManager(const vk::Device& device) : device_(device), shader_manager_(std::make_unique<ShaderManager>(device_)), pipeline_layouts_(), compute_pipelines_() {}


void ProgramManager::loadPrograms(const filesystem::path& shaders_dir) {
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

	// Parse shader data.
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

		// Store shader meta.
		shader_manager_->addShaderData(id, stage_it->second, readShaderFile(shader_path));
	}

	// Parse pipeline data
	for (auto& pipeline : parser["pipelines"]) {
		// Validate pipeline container.
		if (pipeline.find("id") == pipeline.end() || pipeline.find("shaders") == pipeline.end()) {
			throw std::runtime_error("Encountered incomplete pipeline entry while parsing \"shaders_metadata.json\".");
		}

		std::string id = pipeline["id"].get<std::string>();
		std::vector<ShaderData*> pipeline_shaders{};
		
		// Fetch shader indices.
		for (auto& shader : pipeline["shaders"]) {
			ShaderData* shader_data = shader_manager_->getShaderData(shader.get<std::string>());

			// Check if the shader was found.
			if (shader_data == nullptr) {
				throw std::runtime_error("Encountered undefined shader in pipeline. Pipeline id: \"" + id + "\".");
			}

			pipeline_shaders.emplace_back(shader_data);
		}

		// Store pipeline metadata.
		pipeline_layouts_.emplace_back(std::make_unique<PipelineLayout>(device_, id, pipeline_shaders));
	
		// If the pipeline layout belongs to compute pipeline create it.
		if (pipeline_layouts_.back()->getPipelineType() == PipelineType::COMPUTE) {
			compute_pipelines_.emplace_back(std::make_unique<ComputePipeline>(device_, pipeline_layouts_.back().get()));
		}
	}
}

PipelineLayout* ProgramManager::getPipelineLayout(const std::string& name) {
	// Find layout by name.
	auto it = std::find_if(pipeline_layouts_.begin(), pipeline_layouts_.end(), [&name] (const std::unique_ptr<PipelineLayout>& layout) {
		return layout->getName() == name;
	});

	return (it != pipeline_layouts_.end()) ? it->get() : nullptr;
}

ComputePipeline* ProgramManager::getComputePipeline(compute_id_t pipeline_id) {
	return compute_pipelines_[pipeline_id].get();
}

compute_id_t ProgramManager::getComputePipelineId(const std::string& name) {
	for (size_t i = 0; i < compute_pipelines_.size(); i++) {
		if (compute_pipelines_[i]->getName() == name) {
			return i;
		}
	}

	throw std::runtime_error("Could not find compute pipeline with the given name.");
}

DescriptorsCount ProgramManager::getDescriptorsCount() {
	DescriptorsCount sum;
	for (auto it = pipeline_layouts_.begin(); it != pipeline_layouts_.end(); it++) {
		sum += (*it)->getDescriptorsCount();
	}

	return sum;
}

std::vector<uint32_t> ProgramManager::readShaderFile(const filesystem::path& path) {
	std::ifstream file(path, std::ios::ate | std::ios::binary);

	// Check if the shader file was successfully opened.
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

ProgramManager::~ProgramManager() {
	compute_pipelines_.clear();
	pipeline_layouts_.clear();
	shader_manager_.reset();
}


}