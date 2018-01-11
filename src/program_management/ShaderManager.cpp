#include "program_management/ShaderManager.h"
#include <tuple>

namespace vkr {

ShaderData::ShaderData(const vk::Device& device, const std::string& id, vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code) : device_(device), shader_module_(nullptr), id_(id), stage_(stage), code_(code) {
	vk::ShaderModuleCreateInfo shader_ci{};
	shader_ci.pCode = code_.data();
	shader_ci.codeSize = code_.size() * 4;

	shader_module_ = device.createShaderModule(shader_ci);
}

const std::string & ShaderData::getId() const {
	return id_;
}

vk::ShaderStageFlagBits ShaderData::getStage() const {
	return stage_;
}

const std::vector<uint32_t>& ShaderData::getCode() const {
	return code_;
}

vk::PipelineShaderStageCreateInfo ShaderData::getVkHandle() {
	vk::PipelineShaderStageCreateInfo shader_stage_ci{};
	shader_stage_ci.stage = stage_;
	shader_stage_ci.module = shader_module_;
	shader_stage_ci.pName = "main";

	return shader_stage_ci;
}

ShaderData::~ShaderData() {
	device_.destroyShaderModule(shader_module_);
}

ShaderManager::ShaderManager(const vk::Device& device) : device_(device), shaders_() {
}

void ShaderManager::addShaderData(const std::string& id, vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code) {
	if (shaders_.find(id) != shaders_.end()) {
		throw std::runtime_error("Duplicate shader id.");
	}

	// Add new shader.
	shaders_[id] = std::make_unique<ShaderData>(device_, id, stage, code);
}

ShaderData* ShaderManager::getShaderData(const std::string& id) {
	return shaders_[id].get();
}

}