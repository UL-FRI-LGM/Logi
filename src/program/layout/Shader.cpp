#include "program/layout/Shader.h"

namespace logi {

ShaderData::ShaderData(vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code, const std::string& entry_point)
	:  stage(stage), code(code), entry_point(entry_point) {}

Shader::Shader(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const ShaderData& shader_data)
	: DependentDestroyableHandle(owner), shader_data_(std::make_shared<ShaderData>(shader_data)), vk_shader_module_(nullptr) {
	// Create shader
	vk::ShaderModuleCreateInfo shader_ci{};
	shader_ci.pCode = shader_data_->code.data();
	shader_ci.codeSize = shader_data_->code.size() * 4;

	vk_shader_module_ = std::make_shared<ManagedVkShaderModule>(device, device.createShaderModule(shader_ci));
}

vk::ShaderStageFlagBits Shader::getStage() const {
	return shader_data_->stage;
}

const std::vector<uint32_t>& Shader::getCode() const {
	return shader_data_->code;
}

vk::PipelineShaderStageCreateInfo Shader::getVkHandle() const {
	return vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), shader_data_->stage, vk_shader_module_->get(), shader_data_->entry_point.c_str());
}

void Shader::free() {
	vk_shader_module_->destroy();
}

} ///!	namespace logi