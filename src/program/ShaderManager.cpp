#include "program/ShaderManager.h"
#include <tuple>

namespace vkr {

ShaderManager::ShaderManager(const vk::Device& device) : device_(device), shaders_() {
}

void ShaderManager::addShaderData(const std::string& id, vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code) {
	if (shaders_.find(id) != shaders_.end()) {
		throw std::runtime_error("Duplicate shader id.");
	}

	// Add new shader.
	shaders_[id] = std::make_unique<Shader>(device_, id, stage, code);
}

Shader* ShaderManager::getShaderData(const std::string& id) {
	return shaders_[id].get();
}

}