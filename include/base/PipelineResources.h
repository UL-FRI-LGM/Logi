/*
* Shader manager.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef SHADER_RESOURCES_H
#define SHADER_RESOURCES_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include <tuple>
#include <unordered_map>
#include "base/Types.h"
#include <map>

namespace vkr {

class PipelineResources {
public:
	PipelineResources(vk::Device device);

	/**
	* @brief Create shader module and cache it.
	*
	* @param code Shader code
	* @return Tuple containing id of the shader module and the created shader module.
	*/
	std::tuple<shader_id_t, vk::PipelineShaderStageCreateInfo> createShaderModule(const std::vector<uint32_t>& code, vk::ShaderStageFlagBits stage);

	/**
	* @brief Retrieve the shader module with the given id. If the shader module is not found it returns a null handle.
	*
	* @param id Shader module id.
	* @return Shader module embedded in PipelineShaderStageCreateInfo
	*/
	vk::PipelineShaderStageCreateInfo getShaderModule(shader_id_t id);
	
	/**
	 * @brief Create descriptor set layout based and cache it.
	 * 
	 * @param bindings Descriptors of set layout bindings.
	 * @return Tuple containing id of the descriptor set layout and the created descriptor set layout.
	 */
	std::tuple<descset_id_t, vk::DescriptorSetLayout> createDescriptorSetLayout(const std::vector<vk::DescriptorSetLayoutBinding>& bindings);

	/**
	 * @brief Retrieve descriptor set layout with the given id.
	 *
	 * @param id Descriptor set layout id.
	 * @return Descriptor set layout.
	 */
	vk::DescriptorSetLayout getDescriptorSetLayout(descset_id_t id);

	/**
	 * @brief Create pipeline layout and cache it.
	 *
	 * @param dsl_indices Indices of descriptor set layouts used in this pipeline.
	 * @param push_constants Push constant ranges.
	 * @return Tuple containing id of the pipeline layout and the created pipeline layout.
	 */
	std::tuple<layout_id_t, vk::PipelineLayout> createPipelineLayout(const std::vector<descset_id_t>& dsl_ids, const std::vector<vk::PushConstantRange>& push_constants);

	/**
	 * @brief Retrieve pipeline layout with the given id.
	 *
	 * @param id Pipeline layout id.
	 * @return Pipeline layout.
	 */
	vk::PipelineLayout getPipelineLayout(layout_id_t id);

	/**
	* @brief Creates compute pipeline for the given shader.
	*
	* @param shader_id id of compute shader.
	* @param shader_id id of pipeline layout.
	*/
	void createComputePipeline(shader_id_t shader_id, layout_id_t layout_id, bool recreate = false);

	/**
	 * @brief Retrieve compute pipeline for the given shader.
	 *
	 * @param shader_id id of compute shader.
	 * @return Compute pipeline. If the pipeline does not exist null handle is returned.
	 */
	vk::Pipeline getComputePipeline(shader_id_t shader_id);

	/**
	* @brief Destroy all cached compute pipelines.
	*/
	void clearComputePipelines();

	~PipelineResources();
protected:
	/**
	* @brief Destroy all cached shader modules (this invalidates all indices previously outputted by createShaderModule).
	*/
	void clearShaderModules();

	/**
	* @brief Destroy all cached descriptor set layouts (this invalidates all indices previously outputted by createDescriptorSetLayout).
	*/
	void clearDescriptorSetLayouts();

	/**
	* @brief Destroy all cached pipeline layouts (this invalidates all indices previously outputted by createPipelineLayout).
	*/
	void clearPipelineLayouts();

private:
	vk::Device device_;

	std::vector<vk::PipelineShaderStageCreateInfo> shaders_; ///< Shader modules.
	std::vector<vk::DescriptorSetLayout> descriptor_sets_; ///< Descriptor set layouts
	std::vector<vk::PipelineLayout> pipeline_layouts_; ///< Pipeline layout
	
	std::unordered_map<shader_id_t, vk::Pipeline> compute_pipelines_;
	// TODO: Graphical pipelines.
};

}

#endif