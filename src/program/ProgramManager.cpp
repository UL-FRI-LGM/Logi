#include "program/ProgramManager.h"
#include <fstream>
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

PipelineLayout ProgramManager::createPipelineLayout(const std::vector<PipelineShaderStage>& shader_stages) {
	return handle_manager_->createHandle<PipelineLayout>(device_, shader_stages);
}

ShaderModule ProgramManager::createShaderModule(const ShaderModuleConfig& config) const {
	return handle_manager_->createHandle<ShaderModule>(device_, config);
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

std::vector<ComputePipeline> ProgramManager::createComputePipelines(const std::vector<ComputePipelineCreateInfo>& create_infos) const {
	std::vector<vk::ComputePipelineCreateInfo> vk_create_infos;
	vk_create_infos.reserve(create_infos.size());

	for (const ComputePipelineCreateInfo& ci : create_infos) {
		vk_create_infos.emplace_back(ci.flags, ci.layout.getVkShaderHandles()[0], ci.layout.getVkHandle());
	}

	std::vector<vk::Pipeline> vk_pipelines = device_.createComputePipelines({}, vk_create_infos);
	std::vector<ComputePipeline> pipeline_handles;
	pipeline_handles.reserve(vk_pipelines.size());

	// Create handles.
	for (size_t i = 0; i < vk_pipelines.size(); ++i) {
		pipeline_handles.emplace_back(handle_manager_->createHandle<ComputePipeline>(device_, vk_pipelines[i], create_infos[i].layout));
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

	const size_t file_size = static_cast<size_t>(file.tellg());
	std::vector<uint32_t> buffer(file_size / sizeof(uint32_t));

	file.seekg(0);
	file.read(reinterpret_cast<char*>(buffer.data()), file_size);
	file.close();

	return buffer;
}

}