#include "base/PipelineResources.h"

namespace vkr {

PipelineResources::PipelineResources(vk::Device device) : device_{ device } {
}

#pragma region Shader module management.

std::tuple<shader_id_t, vk::PipelineShaderStageCreateInfo> PipelineResources::createShaderModule(const std::vector<uint32_t>& code, vk::ShaderStageFlagBits stage) {
	// Create and cache the shader.
	vk::ShaderModuleCreateInfo createInfo{};
	createInfo.codeSize = code.size() * 4;
	createInfo.pCode = code.data();

	vk::PipelineShaderStageCreateInfo shader;
	shader.module = device_.createShaderModule(createInfo);
	shader.pName = "main";
	shader.stage = stage;

	shaders_.push_back(shader);
	return std::make_tuple(shaders_.size() - 1, shaders_.back());
}

vk::PipelineShaderStageCreateInfo PipelineResources::getShaderModule(shader_id_t id) {
	return (id < shaders_.size()) ? shaders_[id] : vk::PipelineShaderStageCreateInfo();
}

void PipelineResources::clearShaderModules() {
	// Destroy the shaders.
	for (vk::PipelineShaderStageCreateInfo& sm : shaders_) {
		device_.destroyShaderModule(sm.module);
	}

	shaders_.clear();
}

#pragma endregion

#pragma region Descriptor set layouts management.

std::tuple<descset_id_t, vk::DescriptorSetLayout> PipelineResources::createDescriptorSetLayout(const std::vector<vk::DescriptorSetLayoutBinding>& bindings) {
	vk::DescriptorSetLayoutCreateInfo dsl_ci;
	dsl_ci.bindingCount = bindings.size();
	dsl_ci.pBindings = bindings.data();

	descriptor_sets_.push_back(device_.createDescriptorSetLayout(dsl_ci));
	return std::make_tuple(descriptor_sets_.size() - 1, descriptor_sets_.back());
}

vk::DescriptorSetLayout PipelineResources::getDescriptorSetLayout(descset_id_t id) {
	return (id < descriptor_sets_.size()) ? descriptor_sets_[id] : vk::DescriptorSetLayout(nullptr);
}

void PipelineResources::clearDescriptorSetLayouts() {
	for (vk::DescriptorSetLayout& dsl : descriptor_sets_) {
		device_.destroyDescriptorSetLayout(dsl);
	}

	descriptor_sets_.clear();
}

#pragma endregion

#pragma region Pipeline layouts management.

std::tuple<layout_id_t, vk::PipelineLayout> PipelineResources::createPipelineLayout(const std::vector<descset_id_t>& dsl_ids, const std::vector<vk::PushConstantRange>& push_constants) {
	std::vector<vk::DescriptorSetLayout> dsls(dsl_ids.size());

	// Fetch descriptor set layouts
	std::transform(dsl_ids.begin(), dsl_ids.end(), dsls.begin(), [this](descset_id_t idx) {
		return getDescriptorSetLayout(idx);
	});

	vk::PipelineLayoutCreateInfo pl_ci;
	pl_ci.setLayoutCount = dsls.size();
	pl_ci.pSetLayouts = dsls.data();
	pl_ci.pushConstantRangeCount = push_constants.size();
	pl_ci.pPushConstantRanges = push_constants.data();

	pipeline_layouts_.push_back(device_.createPipelineLayout(pl_ci));
	return std::make_tuple(pipeline_layouts_.size() - 1, pipeline_layouts_.back());
}

vk::PipelineLayout PipelineResources::getPipelineLayout(layout_id_t id) {
	return (id < pipeline_layouts_.size()) ? pipeline_layouts_[id] : vk::PipelineLayout(nullptr);
}

void PipelineResources::clearPipelineLayouts() {
	for (vk::PipelineLayout& pl : pipeline_layouts_) {
		device_.destroyPipelineLayout(pl);
	}

	pipeline_layouts_.clear();
}

#pragma endregion

#pragma region Pipeline management.

void PipelineResources::createComputePipeline(shader_id_t shader_id, layout_id_t layout_id, bool recreate) {
	// Pipeline can only have a single layout. Because of that it's being querried by layout_id.
	auto it = compute_pipelines_.find(shader_id);
	if (it != compute_pipelines_.end()) {
		if (!recreate) {
			return;
		}

		// Destroy existing pipeline.
		device_.destroyPipeline(it->second);
	}

	// Create new pipeline.
	vk::ComputePipelineCreateInfo cp_ci;
	cp_ci.layout = getPipelineLayout(layout_id);
	cp_ci.stage = getShaderModule(shader_id);

	vk::Pipeline pipeline = device_.createComputePipeline(vk::PipelineCache(nullptr), cp_ci); // TODO: Implement cache.
	compute_pipelines_[layout_id] = pipeline;
}

vk::Pipeline PipelineResources::getComputePipeline(shader_id_t shader_id) {
	// Check if this pipeline was already constructed.
	auto it = compute_pipelines_.find(shader_id);
	if (it != compute_pipelines_.end()) {
		return it->second;
	}

	return nullptr;
}

void PipelineResources::clearComputePipelines() {
	for (auto it = compute_pipelines_.begin(); it != compute_pipelines_.end(); it++) {
		device_.destroyPipeline(it->second);
	}

	compute_pipelines_.clear();
}

#pragma endregion

PipelineResources::~PipelineResources() {
	clearComputePipelines();
	clearPipelineLayouts();
	clearDescriptorSetLayouts();
	clearShaderModules();
}

}