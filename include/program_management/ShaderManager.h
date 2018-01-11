#ifndef PROGRAM_MANAGEMENT_SHADER_MANAGER_H
#define PROGRAM_MANAGEMENT_SHADER_MANAGER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan.hpp>

namespace vkr {

/**
 * @brief Encapsulates shader data.
 */
class ShaderData {
public:
	/**
	 * @brief Create new shader data with the given id from the given code and stage.
	 *
	 * @param device logical device
	 * @param id Shader id.
	 * @param stage Shader stage.
	 * @param code Shader code.
	 */
	ShaderData(const vk::Device& device, const std::string& id, vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code);

	/**
	 * @brief Get shader unique identifier.
	 *
	 * @return Shader unique identifier.
	 */
	const std::string& getId() const;

	/**
	 * @brief Get shader stage.
	 *
	 * @return Shader stage flag.
	 */
	vk::ShaderStageFlagBits getStage() const;

	/**
	 * @brief Get shader code.
	 *
	 * @return Reference to the shader code.
	 */
	const std::vector<uint32_t>& getCode() const;

	/**
	 * @brief Return Vulkan shader handle.
	 *
	 * @return Vulkan shader handle.
	 */
	vk::PipelineShaderStageCreateInfo getVkHandle();

	/**
	 * @brief Free resources.
	 */
	~ShaderData();

private:
	vk::Device device_;								///< Logical device.
	vk::ShaderModule shader_module_;	///< Vulkan shader module.

	std::string id_;									///< Shader identifier.
	vk::ShaderStageFlagBits stage_;		///< Shader stage.
	std::vector<uint32_t> code_;			///< Shader code.
};

/**
 * @brief Class used to manage ShaderData.
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
	 * @brief Find the index of the ShaderData with the given ID.
	 *
	 * @param id Shader identifier.
	 */
	ShaderData* getShaderData(const std::string& id);

private:
	vk::Device device_; ///< Logical device.
	std::unordered_map<std::string, std::unique_ptr<ShaderData>> shaders_; ///< Maps shader identifier to its index.
};

}

#endif /// PROGRAM_MANAGEMENT_SHADER_MANAGER_H