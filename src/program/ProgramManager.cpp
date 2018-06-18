#include "program/ProgramManager.h"
#include <fstream>
#include <json.hpp>
#include <vulkan/vulkan.hpp>

namespace logi {

static const std::map<std::string, vk::ShaderStageFlagBits> stage_map{
	{ "vertex", vk::ShaderStageFlagBits::eVertex },
	{ "geometry", vk::ShaderStageFlagBits::eGeometry },
	{ "fragment", vk::ShaderStageFlagBits::eFragment },
	{ "compute", vk::ShaderStageFlagBits::eCompute }
};

ProgramManager::ProgramManager() : Handle(false), device_(nullptr), handle_manager_(nullptr) { }

ProgramManager::ProgramManager(const vk::Device& device) : device_(device), handle_manager_(std::make_shared<HandleManager>()) {}

PipelineLayout ProgramManager::createPipelineLayout(const std::vector<Shader>& shaders) {
	return handle_manager_->createHandle<PipelineLayout>(device_, shaders);
}

Shader ProgramManager::createShader(const ShaderData& shader_data) {
	return handle_manager_->createHandle<Shader>(device_, shader_data);
}

Shader ProgramManager::loadShader(const std::string& shader_path, vk::ShaderStageFlagBits stage, const std::string& entry_point) {
	ShaderData shader_data(stage, readShaderFile(shader_path), entry_point);
	return handle_manager_->createHandle<Shader>(device_, shader_data);
}

RenderPass ProgramManager::createRenderPass(const RenderPassLayout& layout) const {
	return handle_manager_->createHandle<RenderPass>(device_, layout);
}

std::vector<GraphicalPipeline> ProgramManager::createGraphicalPipelines(std::vector<GraphicalPipelineCreateInfo>& create_infos) const {
	std::vector<std::vector<vk::PipelineShaderStageCreateInfo>> shader_stages_cis{};
	std::vector<std::vector<vk::VertexInputAttributeDescription>> attribute_descriptions(create_infos.size());
	std::vector<std::vector<vk::VertexInputBindingDescription>> attribute_bindings(create_infos.size());
	std::vector<vk::PipelineVertexInputStateCreateInfo> vertex_input_states(create_infos.size());


	// Collect layout data for create info structure creation.
	for (size_t i = 0; i < create_infos.size(); ++i) {
		shader_stages_cis.emplace_back(create_infos[i].layout.getVkShaderHandles());

		for (const VertexAttributeLayout& attr_layout : create_infos[i].layout.getAttributeLayouts()) {
			attribute_descriptions[i].emplace_back(attr_layout.getVkAttributeDescription());
			attribute_bindings[i].emplace_back(attr_layout.getVkBindingDescription());
		}

		vertex_input_states[i].pVertexAttributeDescriptions = attribute_descriptions[i].data();
		vertex_input_states[i].pVertexBindingDescriptions = attribute_bindings[i].data();
	}

	std::vector<vk::GraphicsPipelineCreateInfo> vk_pipeline_cis{};
	for (size_t i = 0; i < create_infos.size(); ++i) {
		PipelineState& state = create_infos[i].state;

		vk::GraphicsPipelineCreateInfo gp_ci{};
		gp_ci.stageCount = shader_stages_cis[i].size();
		gp_ci.pStages = shader_stages_cis[i].data();
		gp_ci.pVertexInputState = &vertex_input_states[i];
		gp_ci.pInputAssemblyState = state.input_assembly.buildCreateInfo();
		gp_ci.pTessellationState = state.tessellation.buildCreateInfo();
		gp_ci.pViewportState = state.viewport.buildCreateInfo();
		gp_ci.pRasterizationState = state.rasterization.buildCreateInfo();
		gp_ci.pMultisampleState = state.multisample.buildCreateInfo();
		gp_ci.pDepthStencilState = state.depth_stencil.buildCreateInfo();
		gp_ci.pColorBlendState = state.color_blend.buildCreateInfo();
		gp_ci.pDynamicState = state.dynamic_states.buildCreateInfo();
		gp_ci.layout = create_infos[i].layout.getVkHandle();
		gp_ci.renderPass = create_infos[i].render_pass.getVkHandle();
		gp_ci.subpass = create_infos[i].subpass_index;

		vk_pipeline_cis.emplace_back(gp_ci);
	}

	std::vector<vk::Pipeline> vk_pipelines = device_.createGraphicsPipelines({}, vk_pipeline_cis);
	std::vector<GraphicalPipeline> pipeline_handles{};

	// Create handles.
	for (size_t i = 0; i < create_infos.size(); ++i) {
		pipeline_handles.emplace_back(create_infos[i].render_pass.addGraphicalPipeline(vk_pipelines[i], create_infos[i].layout, create_infos[i].state));
	}

	return pipeline_handles;
}

void ProgramManager::free() {
	handle_manager_->destroyAllHandles();
}

std::vector<uint32_t> ProgramManager::readShaderFile(const std::string& path) {
	std::ifstream file(path, std::ios::ate | std::ios::binary);

	// Check if the shader file was successfully opened.
	if (!file.is_open()) {
		throw std::runtime_error("Failed to load shader file: \"" + path + "\". Make sure that the file exists.");
	}

	size_t file_size = static_cast<size_t>(file.tellg());
	std::vector<uint32_t> buffer(file_size / sizeof(uint32_t));

	file.seekg(0);
	file.read(reinterpret_cast<char*>(buffer.data()), file_size);
	file.close();

	return buffer;
}

}