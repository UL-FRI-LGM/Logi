#include "base/ProgramManager.h"
#include <fstream>
#include <json.hpp>
#include <vulkan/vulkan.hpp>
#include <spirv_cross.hpp>
#include "base/ProgramManager.h"
#include "util/FormatConversion.h"
#include "..\..\include\base\ProgramManager.h"

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

void ProgramManager::initializeShaderDescriptorMeta(ShaderMeta& shader_meta_entry, const std::vector<uint32_t>& code) {
	spirv_cross::Compiler comp(code);
	// The SPIR-V is now parsed, and we can perform reflection on it.
	spirv_cross::ShaderResources resources = comp.get_shader_resources();

	// Parse attributes if it's a vertex shader.
	if (shader_meta_entry.stage == vk::ShaderStageFlagBits::eVertex) {
		// Temporary containers for attribute and bindings descriptions.
		std::vector<vk::VertexInputAttributeDescription> attribute_descriptions;
		std::vector<vk::VertexInputBindingDescription> bindings_descriptions;

		for (auto& resource : resources.stage_inputs) {
			uint32_t binding = comp.get_decoration(resource.id, spv::DecorationBinding);
			auto resource_type = comp.get_type_from_variable(resource.id);

			// Add attribute description
			attribute_descriptions.push_back({});

			vk::VertexInputAttributeDescription& attribute_description = attribute_descriptions[attribute_descriptions.size() - 1];
			attribute_description.location = comp.get_decoration(resource.id, spv::DecorationLocation);
			attribute_description.binding = binding;
			attribute_description.offset = 0;
			attribute_description.format = getVertexBufferFormat(resource_type);

			// Add bindings description.
			bindings_descriptions.push_back({});

			vk::VertexInputBindingDescription& binding_description = bindings_descriptions[bindings_descriptions.size() - 1];
			binding_description.binding = binding;
			binding_description.inputRate = vk::VertexInputRate::eVertex; // TODO: Expand this in future.
			binding_description.stride = (resource_type.width * resource_type.vecsize * resource_type.columns) / 8; // Size of the single element in bytes.
		}

		shader_meta_entry.vtx_attribute_descriptions = std::make_unique<const std::vector<vk::VertexInputAttributeDescription>>(std::move(attribute_descriptions));
		shader_meta_entry.vtx_bindings_descriptions = std::make_unique<const std::vector<vk::VertexInputBindingDescription>>(std::move(bindings_descriptions));
	}

	// Helper lambda used to add new DescriptorSetLayoutMeta entry to the shader meta.
	auto addDescriptorMeta = [&shader_meta_entry, &comp](const spirv_cross::Resource& resource, vk::DescriptorType type) {
		auto resource_type = comp.get_type_from_variable(resource.id);

		// Add new descriptor set meta.
		shader_meta_entry.descriptor_set_meta.push_back({});
		DescriptorBindingMeta& descriptor_meta = shader_meta_entry.descriptor_set_meta[shader_meta_entry.descriptor_set_meta.size() - 1];

		descriptor_meta.set = comp.get_decoration(resource.id, spv::DecorationDescriptorSet);
		descriptor_meta.binding = comp.get_decoration(resource.id, spv::DecorationBinding);
		descriptor_meta.count = std::accumulate(resource_type.array.begin(), resource_type.array.end(), 1, std::multiplies<uint32_t>());
		descriptor_meta.type = type;
	};

	// Add uniform buffers meta.
	for (auto& resource : resources.uniform_buffers) {
		addDescriptorMeta(resource, vk::DescriptorType::eUniformBuffer);
	}

	// Add storage buffers meta.
	for (auto& resource : resources.storage_buffers) {
		addDescriptorMeta(resource, vk::DescriptorType::eStorageBuffer);
	}

	// Add storage images meta.
	for (auto& resource : resources.storage_images) {
		addDescriptorMeta(resource, vk::DescriptorType::eStorageImage);
	}

	// Add sampled images (separate) meta.
	for (auto& resource : resources.separate_images) {
		addDescriptorMeta(resource, vk::DescriptorType::eSampledImage);
	}

	// Add sampled images (combined with sampler) meta.
	for (auto& resource : resources.sampled_images) {
		addDescriptorMeta(resource, vk::DescriptorType::eCombinedImageSampler);
	}

	// Add sampler meta (used in conjunction with a sampled image (separate).
	for (auto& resource : resources.separate_samplers) {
		addDescriptorMeta(resource, vk::DescriptorType::eSampler);
	}

	// Add Push constants
	// TODO

	// Mark shader entry as initialized.
	shader_meta_entry.initialized = true;
}

void ProgramManager::initializePipelineLayout(VulkanDevice* device, PipelineMeta& pipeline_meta_entry) {
	std::vector<size_t>& shader_indices = pipeline_meta_entry.shaders_indices;

	// Determine pipeline type.
	if (shader_indices.size() == 1) {
		// If there is a single shader in a pipeline, it has to be compute shader.
		if (shaders_metadata_[shader_indices[0]].stage != vk::ShaderStageFlagBits::eCompute) {
			throw std::runtime_error("Invalid pipeline configuration. Pipeline id: id: \"" + pipeline_meta_entry.id + "\".");
		}
		
		pipeline_meta_entry.type = PipelineType::COMPUTE;
	}
	else {
		pipeline_meta_entry.type = PipelineType::GRAPHIC;

		// Sort shaders into correct order.
		std::sort(shader_indices.begin(), shader_indices.end(), [this](const size_t& idx_a, const size_t& idx_b) {
			return shaders_metadata_[idx_a].stage < shaders_metadata_[idx_b].stage;
		});

		vk::ShaderStageFlags stages{};
		// Check if all required shader stages are present.
		for (size_t idx : shader_indices) {
			// Check if duplicate stage.
			if (stages & shaders_metadata_[idx].stage) {
				throw std::runtime_error("Duplicate shader stage in pipeline. Pipeline id: id: \"" + pipeline_meta_entry.id + "\".");
			}

			stages |= shaders_metadata_[idx].stage;
		}

		// Check if vertex and fragment shaders are present.
		if (!(stages & vk::ShaderStageFlagBits::eVertex) || !(stages & vk::ShaderStageFlagBits::eFragment)) {
			throw std::runtime_error("Missing required shaders. Pipeline id: id: \"" + pipeline_meta_entry.id + "\".");
		}

		// Check if the compute shader is present in graphical pipeline
		if (stages & vk::ShaderStageFlagBits::eCompute) {
			throw std::runtime_error("Compute shader in graphical pipeline. Pipeline id: id: \"" + pipeline_meta_entry.id + "\".");
		}

		// Setup graphic pipeline attribute descriptions and bindings create info.
		pipeline_meta_entry.attributes_info = std::make_unique<vk::PipelineVertexInputStateCreateInfo>();
		ShaderMeta& vertex_shader = shaders_metadata_[shader_indices[0]];

		pipeline_meta_entry.attributes_info->vertexAttributeDescriptionCount = vertex_shader.vtx_attribute_descriptions->size();
		pipeline_meta_entry.attributes_info->pVertexAttributeDescriptions = vertex_shader.vtx_attribute_descriptions->data();
		pipeline_meta_entry.attributes_info->vertexBindingDescriptionCount = vertex_shader.vtx_bindings_descriptions->size();
		pipeline_meta_entry.attributes_info->pVertexBindingDescriptions = vertex_shader.vtx_bindings_descriptions->data();
	}

	std::vector<std::vector<vk::DescriptorSetLayoutBinding>> layout_bindings{};

	for (size_t idx : shader_indices) {
		for (const DescriptorBindingMeta& descriptor_binding : shaders_metadata_[idx].descriptor_set_meta) {
			if (layout_bindings.sidescriptor_binding.set
		}
	}

}



void ProgramManager::loadShaders(VulkanDevice* device) {
	// Initialize shader meta data.
	for (ShaderMeta& shader_meta : shaders_metadata_) {
		std::vector<uint32_t> code = readShaderFile(shader_meta.shader_path);

		// Create shader module for the device
		//device->createShaderModule(code);

		// Initialize shader meta data.
		if (!shader_meta.initialized) {
			initializeShaderDescriptorMeta(shader_meta, code);
		}
	}

	// Initialize pipeline layouts
	for (PipelineMeta& pipeline_meta : pipelines_metadata_) {
			initializePipelineLayout(device, pipeline_meta);
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

		// There must be at least one shader in the pipeline.
		if (shaders_indices.empty()) {
			throw std::runtime_error("Pipeline has no shaders. Pipeline id: \"" + id + "\".");
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