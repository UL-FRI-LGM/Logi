#ifndef PROGRAM_LAYOUT_SHADER_H
#define PROGRAM_LAYOUT_SHADER_H

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>

namespace vkr {

/**
 * @brief	Encapsulates shader data.
 */
class Shader {
public:
	/**
	 * @brief	Create new shader data with the given id from the given code and stage.
	 *
	 * @param	device	Logical device.
	 * @param	id		Shader identifier.
	 * @param	stage	Shader stage.
	 * @param	code	Shader code.
	 */
	Shader(const vk::Device& device, const std::string& id, vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code, const std::string& entry_point = "main");

	/**
	 * @brief	Get shader unique identifier.
	 *
	 * @return	Shader unique identifier.
	 */
	const std::string& getId() const;

	/**
	 * @brief	Get shader stage.
	 *
	 * @return	Shader stage flag.
	 */
	vk::ShaderStageFlagBits getStage() const;

	/**
	 * @brief	Get shader code.
	 *
	 * @return	Reference to the shader code.
	 */
	const std::vector<uint32_t>& getCode() const;

	/**
	 * @brief	Get Vulkan shader handle.
	 *
	 * @return	Vulkan shader handle.
	 */
	vk::PipelineShaderStageCreateInfo getVkHandle() const;

	/**
	 * @brief	Free resources.
	 */
	~Shader();

private:
	vk::Device device_;									///< Logical device.
	std::string id_;									///< Shader identifier.
	vk::ShaderStageFlagBits stage_;						///< Shader stage.
	std::vector<uint32_t> code_;						///< Shader code.
	std::string entry_point_;							///< Shader entry point.
	vk::ShaderModule shader_module_;					///< Vulkan shader module.
	vk::PipelineShaderStageCreateInfo shader_stage_;	///< Vulkan shader stage handle
};

} ///!	namespace vkr

#endif ///!	HEADER_GUARD