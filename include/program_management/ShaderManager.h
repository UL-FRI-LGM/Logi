#ifndef PROGRAM_MANAGEMENT_SHADER_MANAGER_H
#define PROGRAM_MANAGEMENT_SHADER_MANAGER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "base/VulkanDevice.h"

namespace vkr {

/**
 * @brief Encapsulates shader data.
 */
class ShaderData {
public:
	/**
	 * @brief Create new shader data with the given id from the given code and stage.
	 *
	 * @param id Shader id.
	 * @param stage Shader stage.
	 * @param code Shader code.
	 */
	ShaderData(const std::string& id, vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code);

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
	 * @brief Retrieves shader resource index for the given device. If the shader was not already created for this device, create it.
	 *
	 * @param device Vulkan device for which the shader will be retrieved.
	 * @return Index of the shader resource for the given device.
	 */
	shader_id_t getShaderResourceId(VulkanDevice* device);

	/**
	 * @brief Return Vulkan shader handle for the given device.
	 *
	 * @param Vulkan device for which the shader will be retrieved.
	 * @return Vulkan shader handle.
	 */
	vk::PipelineShaderStageCreateInfo getVkHandle(VulkanDevice* device);

private:
	std::string id_; ///< Shader identifier.
	vk::ShaderStageFlagBits stage_; ///< Shader stage.
	std::vector<uint32_t> code_; ///< Shader code.

	std::unordered_map<VulkanDevice*, shader_id_t> device_to_res_;  ///< Maps Device identifier to Vulkan handle identifier.
};

/**
 * @brief Class used to manage ShaderData.
 */
class ShaderManager {
public:
	/**
	 * @brief Default constructor.
	 */
	ShaderManager();

	/**
	 * @brief Add new shader data.
	 *
	 * @param id Shader identifier (required to be unique).
	 * @param stage Shader stage.
	 * @param code Shader code.
	 * @return Index of the newly created ShaderData.
	 */
	shader_id_t addShaderData(const std::string& id, vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code);
	
	/**
	 * @brief Find the index of the ShaderData with the given ID.
	 *
	 * @param id Shader identifier.
	 * @return Shader index (if shader could not be found max(size_t) will be returned).
	 */
	ShaderData* getShaderData(const std::string& id);


private:
	std::vector<std::unique_ptr<ShaderData>> shaders_; ///< Stores shader data.
	std::unordered_map<std::string, size_t> id_to_idx; ///< Maps shader identifier to its index.
};

}

#endif /// PROGRAM_MANAGEMENT_SHADER_MANAGER_H