#include "program_management/ShaderManager.h"
#include <tuple>

namespace vkr {

ShaderData::ShaderData(const std::string& id, vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code) : id_(id), stage_(stage), code_(code), device_to_res_() {
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

shader_id_t ShaderData::getShaderResourceId(VulkanDevice* device) {
	PipelineResources* resources = device->getPipelineResources();
	auto it = device_to_res_.find(device);

	// Resource index was found.
	if (it != device_to_res_.end()) {
		return it->second;
	}

	// Resource does not exist. Create it.
	shader_id_t id{};
	std::tie(id, std::ignore) = resources->createShaderModule(code_, stage_);
	device_to_res_[device] = id;

	return id;
}

vk::PipelineShaderStageCreateInfo ShaderData::getVkHandle(VulkanDevice* device) {
	PipelineResources* resources = device->getPipelineResources();
	auto it = device_to_res_.find(device);

	if (it == device_to_res_.end()) {
		// Create shader module if it does not exist yet.
		std::tuple<shader_id_t, vk::PipelineShaderStageCreateInfo> new_res = resources->createShaderModule(code_, stage_);
		device_to_res_[device] = std::get<0>(new_res);

		return std::get<1>(new_res);
	}
	else {
		vk::PipelineShaderStageCreateInfo res = resources->getShaderModule(it->second);

		// Check if the shader module is still valid (device did not destroy it).
		if (!res.module) {
			std::tuple<shader_id_t, vk::PipelineShaderStageCreateInfo> new_res = resources->createShaderModule(code_, stage_);
			device_to_res_[device] = std::get<0>(new_res);
			return std::get<1>(new_res);
		}

		return res;
	}
}

ShaderManager::ShaderManager() : shaders_() {
}

shader_id_t ShaderManager::addShaderData(const std::string& id, vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code) {
	if (id_to_idx.find(id) != id_to_idx.end()) {
		throw std::runtime_error("Duplicate shader id.");
	}

	// Add new shader.
	shaders_.emplace_back(std::make_unique<ShaderData>(id, stage, code));
	id_to_idx[id] = shaders_.size() - 1;
	return shaders_.size() - 1;
}

ShaderData* ShaderManager::getShaderData(const std::string& id) {
	auto it = id_to_idx.find(id);

	if (it == id_to_idx.end()) {
		return nullptr;
	}
	else {
		return shaders_[it->second].get();
	}
}

}