#ifndef PROGRAM_LAYOUT_SHADER_H
#define PROGRAM_LAYOUT_SHADER_H

#include "base/ManagedResource.h"
#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>
#include "base/Handle.h"

namespace logi {

/**
 * @brief	Structure that contains the data required to instantiate a Shader.
 */
struct ShaderData {
	/**
	 * @brief	Populates ShaderData structure.
	 *
	 * @param	stage			 Shader stage.
	 * @param	code			 Shader code.
	 * @param	entry_point		 Shader entry point.	
	 */
	ShaderData(vk::ShaderStageFlagBits stage, const std::vector<uint32_t>& code, const std::string& entry_point = "main");

	vk::ShaderStageFlagBits stage;		///< Shader stage.
	std::vector<uint32_t> code;			///< Shader code.
	std::string entry_point;			///< Shader entry point.
};

/**
 * @brief	Encapsulates shader data.
 */
class Shader : public DependentDestroyableHandle {
public:
	/**
	 * @brief	Creates Shader handle.
	 *
	 * @param	owner		Pointer to a HandleManager responsible for this handle.
	 * @param	device		Device to which the shader belongs.
	 * @param	shader_data	ShaderData structure
	 */
	Shader(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const ShaderData& shader_data);

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

protected:
	/**
	 * @brief	Destroys shader module.
	 */
	void free() override;

private:
	using ManagedVkShaderModule = ManagedResource<vk::Device, vk::ShaderModule, vk::DispatchLoaderStatic, &vk::Device::destroyShaderModule>;

	std::shared_ptr<ShaderData> shader_data_;
	std::shared_ptr<ManagedVkShaderModule> vk_shader_module_;
};

} ///!	namespace logi

#endif ///!	HEADER_GUARD