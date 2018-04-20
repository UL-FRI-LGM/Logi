#include "program/layout/Shader.h"

namespace vkr {

Shader::Shader(const vk::Device& device, const std::string& id, vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code, const std::string& entry_point) : device_(device), id_(id), stage_(stage), code_(code), entry_point_(entry_point), shader_module_(nullptr), shader_stage_() {
	// Create shader
	vk::ShaderModuleCreateInfo shader_ci{};
	shader_ci.pCode = code_.data();
	shader_ci.codeSize = code_.size() * 4;

	shader_module_ = device.createShaderModule(shader_ci);

	// Stage create info.
	shader_stage_.stage = stage_;
	shader_stage_.module = shader_module_;
	shader_stage_.pName = entry_point_.c_str();
}

const std::string & Shader::getId() const {
	return id_;
}

vk::ShaderStageFlagBits Shader::getStage() const {
	return stage_;
}

const std::vector<uint32_t>& Shader::getCode() const {
	return code_;
}

vk::PipelineShaderStageCreateInfo Shader::getVkHandle() const {
	return shader_stage_;
}

Shader::~Shader() {
	device_.destroyShaderModule(shader_module_);
}

} ///!	namespace vkr