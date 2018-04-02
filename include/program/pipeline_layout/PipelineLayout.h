#ifndef PROGRAM_PIPELINE_LAYOUT_PIPELINE_LAYOUT_H
#define PROGRAM_PIPELINE_LAYOUT_PIPELINE_LAYOUT_H

#include <vector>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "program/ShaderManager.h"
#include "base/Types.h"
#include "program/pipeline_layout/DescriptorSetLayout.h"
#include "program/pipeline_layout/VertexAttributeLayout.h"
#include "program/pipeline_layout/PushConstantRange.h"

namespace vkr {

/**
 * @brief	Used to specify pipeline type that can be either graphical or compute.
 */
enum class PipelineType {
	eGraphical = 0u,
	eCompute
};

/**
 * @brief	Wraps the data required to create VkPipelineLayout. It retrieves the data from the given shaders by performing shader reflection using spriv-cross.
 */
class PipelineLayout {
public:
	/**
	 * @brief Creates and initializes PipelineLayout from the given shaders.
	 *
	 * @param	device	Vulkan logical device handle.
	 * @param	name	Pipeline layout name.
	 * @param	shaders	Vector of shaders that form a pipeline.
	 */
	PipelineLayout(const vk::Device& device, const std::string& name, const std::vector<Shader*>& shaders);

	/**
	 * @brief	Retrieve PipelineLayout string identifier.
	 *
	 * @return	String identifier.
	 */
	const std::string& getName() const;

	/**
	 * @brief	Retrieve Pipeline type eCompute/eGraphical.
	 *
	 * @return	Pipeline type.
	 */
	PipelineType getPipelineType() const;

	/**
	 * @brief	Retrieve vector of Vulkan shader handles built for the given device.
	 *
	 * @return	Vector containing shader handles.
	 */
	const std::vector<vk::PipelineShaderStageCreateInfo>& getVkShaderHandles() const;

	/**
	 * @brief	Retrieve Vulkan layout handle.
	 *
	 * @return	Pipeline layout.
	 */
	const vk::PipelineLayout& getVkHandle() const;

	/**
	 * @brief	Retrieve descriptors count for the given pipeline layout.
	 *
	 * @return	DescriptorsCount object.
	 */
	const DescriptorsCount& getDescriptorsCount() const;

	/**
	 * @brief	Retrieve the descriptor set layout of the set with the given index.
	 *
	 * @param	set	Set index.
	 * @return	Pointer to DescriptorSetLayout.
	 */
	const DescriptorSetLayout* getDescriptorSetLayout(size_t set) const;

	/**
	 * @brief	Get number of descriptor sets.
	 *
	 * @return	Descriptor set count.
	 */
	size_t getDescriptorSetCount() const;

	/**
	 * @brief	Free resources.
	 */
	~PipelineLayout();

protected:
	/**
	* @brief	Perform shader reflection using spriv-cross.
	*/
	void shaderReflection();

	/**
	* @brief	Builds vulkan pipeline layout.
	*/
	void buildVkPipelineLayout();

private:
	vk::Device device_;													///< Logical device.
	vk::PipelineLayout vk_pipeline_layout_;								///< Pipeline layout handle.
	std::vector<vk::PipelineShaderStageCreateInfo> vk_shaders_;			///< Vulkan shader handles.

	std::string name_;													///< Layout name.
	PipelineType type_;													///< Pipeline type (graphical or compute).
	std::vector<Shader*> shaders_;										///< Pipeline shaders.
	std::vector<std::unique_ptr<DescriptorSetLayout>> descriptor_sets_;	///< Descriptor sets.
	std::vector<PushConstantRange> push_constants_;						///< Push constants.
	DescriptorsCount combined_descriptors_count_;						///< Combined number of descriptors.
	std::vector<VertexAttributeLayout> attributes_;						///< Only for graphical pipelines.
};

} ///!	namespace vkr

#endif ///! HEADER_GUARD