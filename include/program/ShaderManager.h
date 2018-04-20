#ifndef PROGRAM_MANAGEMENT_SHADER_MANAGER_H
#define PROGRAM_MANAGEMENT_SHADER_MANAGER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "program/layout/Shader.h"

namespace vkr {

/**
 * @brief Class used to manage Shader.
 */
class ShaderManager {
public:
	/**
	 * @brief Default constructor.
	 */
	ShaderManager(const vk::Device& device);

	/**
	 * @brief Add new shader data.
	 *
	 * @param id Shader identifier (required to be unique).
	 * @param stage Shader stage.
	 * @param code Shader code.
	 */
	void addShaderData(const std::string& id, vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code);
	
	/**
	 * @brief Find the index of the Shader with the given ID.
	 *
	 * @param id Shader identifier.
	 */
	Shader* getShaderData(const std::string& id);

private:
	vk::Device device_; ///< Logical device.
	std::unordered_map<std::string, std::unique_ptr<Shader>> shaders_; ///< Maps shader identifier to its index.
};

}

#endif /// PROGRAM_MANAGEMENT_SHADER_MANAGER_H