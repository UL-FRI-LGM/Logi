#ifndef BASE_PIPELINE_H
#define BASE_PIPELINE_H

#include <vector>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "base/VulkanDevice.h"
#include "program_management/ShaderManager.h"
#include "base/Types.h"

namespace vkr {

#pragma region DescriptorBindingLayout

/**
 * @brief Wraps Vulkan DescriptorBindingLayout data.
 */
class DescriptorBindingLayout {
public:
	/**
	 * @brief Initializes members with the given values.
	 *
	 * @param name Binding name.
	 * @param binding Binding index.
	 * @param type Binding descriptor type.
	 * @param count Number of array entries.
	 * @param stages Stages to which the binding should be available.
	 */
	DescriptorBindingLayout(std::string name, uint32_t binding, vk::DescriptorType type, uint32_t count, vk::ShaderStageFlags stages);

	/**
	 * @brief Add additional stages.
	 *
	 * @param stage Stages to which the binding should be available.
	 */
	void addStages(vk::ShaderStageFlags stages);

	/**
	 * @brief Retrieve binding name.
	 *
	 * @return Binding name.
	 */
	const std::string& getName() const;

	/**
	 * @brief Retrieve binding index.
	 *
	 * @return Binding index.
	 */
	uint32_t getBinding() const;

	/**
	 * @brief Retrieve binding descriptor type.
	 *
	 * @return Binding descriptor type.
	 */
	vk::DescriptorType getDescriptorType() const;

	/**
	 * @brief Retrieve binding count.
	 *
	 * @return Binding count.
	 */
	uint32_t getCount() const;

	/**
	* @brief Retrieve stage to which the binding should be available.
	*
	* @return Binding stages.
	*/
	vk::ShaderStageFlags getShaderStages() const;

	/**
	 * @brief Build Vulkan binding structure from the data.
	 *
	 * @return Vulkan binding structure.
	 */
	vk::DescriptorSetLayoutBinding getVkHandle() const;

private:
	std::string name_; ///< Binding name.
	uint32_t binding_; ///< Binding index.
	vk::DescriptorType type_; ///< Binding descriptor type.
	uint32_t count_; ///< Number of array entries.
	vk::ShaderStageFlags stages_; ///< Stages to which the binding should be available.
};

#pragma endregion

#pragma region DescriptorSetLayout

/**
* @brief Wraps Vulkan DescriptorBindingLayout data.
*/
class DescriptorSetLayout {
friend class PipelineLayout;
public:
	/**
	 * @brief Default constructor.
	 */
	DescriptorSetLayout();

	/**
	 * @brief Retrieve DescriptorBindingLayout with the given binding index.
	 *
	 * @param binding Binding index.
	 * @return Pointer to the DescriptorBindingLayout or nullptr if the DescriptorBindingLayout with the give binding index does not exist.
	 */
	DescriptorBindingLayout* getDescriptorBinding(uint32_t binding);

	/**
	* @brief Get DescriptorSetLayout resource index.
	*/
	descset_id_t getResourceId(VulkanDevice* device);

	/**
	 * @brief Get DescriptorSetLayout
	 */
	vk::DescriptorSetLayout getVkHandle(VulkanDevice* device);

	/**
	 * @brief Returns DescriptorCount object that contains descriptor type counters.
	 *
	 * @return DescriptorCount object.
	 */
	const DescriptorsCount& getDescriptorsCount();

protected:
	/**
	 * @brief Add new DescriptorBindingLayout to the descriptor set.
	 *
	 * @param Descriptor binding.
	 */
	void addDescriptorBinding(const DescriptorBindingLayout& binding);

	/**
	 * @brief Increment descriptor count of a given type.
	 * 
	 * @param type Descriptor type.
	 */
	void incrementDescriptorCount(const vk::DescriptorType& type);

private:
	std::vector<DescriptorBindingLayout> bindings_; ///< Descriptor binding belonging to this set (bindings may be defined sparsely).
	std::unordered_map<VulkanDevice*, descset_id_t> device_to_res_; ///< Maps device to resource id.
	DescriptorsCount descriptors_count_;
};

#pragma endregion

#pragma region PushConstant

/**
 * @brief Wraps Vulkan PushConstant data.
 */
class PushConstant {
public:
	/**
	 * @brief Default constructor. Initializes push constant members.
	 *
	 * @param name Push constant name.
	 * @param stages Stages to which the push constant will be available.
	 * @param offset Push constant offset.
	 * @param size Push constant size.
	 */
	PushConstant(std::string name, vk::ShaderStageFlags stages, uint32_t offset, uint32_t size);

	/**
	* @brief Add additional stages.
	*
	* @param stage Stages to which the push constant should be available.
	*/
	void addStages(vk::ShaderStageFlags stages);

	/**
	 * @brief Retrieve push constant name.
	 *
	 * @return Push constant name.
	 */
	const std::string& getName() const;

	/**
	 * @brief Retrieve the stages to which the push constant will be available.
	 *
	 * @return Shader stage flags.
	 */
	vk::ShaderStageFlags getStages() const;

	/**
	 * @brief Retrieve push constant offset.
	 *
	 * @return Push constant offset.
	 */
	uint32_t getOffset() const;

	/**
	 * @brief Retrieve push constant size.
	 *
	 * @return Return push constant size.
	 */
	uint32_t getSize() const;

	/**
	 * @brief Build Vulkan push constant structure.
	 *
	 * @return Vulkan push constant structure.
	 */
	vk::PushConstantRange getVkHandle();

private:
	std::string name_; ///< Push constant name.
	vk::ShaderStageFlags stages_; ///< Stage to which the push constant will be available.
	uint32_t offset_; ///< Push constant offset.
	uint32_t size_; ///< Push constant size.
};

#pragma endregion

#pragma region VertexAttribute

/**
 * @brief Encapsulates vertex attribute data.
 */
class VertexAttribute {
public:
	/**
	 * @brief Default constructor. Initialize data members.
	 */
	VertexAttribute(std::string name, uint32_t location, uint32_t binding, vk::Format format, uint32_t offset, uint32_t stride, vk::VertexInputRate input_rate);

	/**
	 * @brief Build VertexInputBindingDescription Vulkan structure.
	 *
	 * @return VertexInputBindingDescription Vulkan structure
	 */
	vk::VertexInputBindingDescription getBindingDescription();

	/**
	 * @brief Build VertexInputAttributeDescription Vulkan structure.
	 *
	 * @return VertexInputAttributeDescription Vulkan structure
	 */
	vk::VertexInputAttributeDescription getAttributeDescription();

	/**
	* @brief Retrieve attribute name.
	*
	* @return Attribute name.
	*/
	const std::string& getName() const;

	/**
	 * @brief Retrieve attribute location.
	 *
	 * @return Attribute location.
	 */
	uint32_t getLocation() const;

	/**
	 * @brief Retrieve attribute binding.
	 *
	 * @return Attribute binding.
	 */
	uint32_t getBinding() const;

	/**
	 * @brief Retrieve attribute format.
	 *
	 * @return Attribute format.
	 */
	vk::Format getFormat() const;

	/**
	 * @brief Retrieve attribute offset.
	 *
	 * @return Attribute offset.
	 */
	uint32_t getOffset() const;

	/**
	 * @brief Retrieve attribute stride.
	 *
	 * @return Attribute stride.
	 */
	uint32_t getStride() const;

	/**
	 * @brief Retrieve attribute input rate.
	 *
	 * @return Attribute location.
	 */
	vk::VertexInputRate getInputRate() const;

private:
	std::string name_; ///< Attribute name.
	uint32_t location_; ///< Attribute location index.
	uint32_t binding_; ///< Attribute binding index.
	vk::Format format_; ///< Attribute elements format.
	uint32_t offset_; ///< Offset in bytes relative to the start of element.
	uint32_t stride_; ///< Distance in bytes between two consecutive elements.
	vk::VertexInputRate input_rate_; ///< Per vertex or per instance.
};

#pragma endregion

enum class PipelineType {
	GRAPHICAL,
	COMPUTE
};

class PipelineLayout {
public:
	/**
	 * @brief Creates and initializes PipelineLayout from the given shaders.
	 */
	PipelineLayout(const std::string& name, const std::vector<ShaderData*>& shaders);

	/**
	 * @brief Retrieve PipelineLayout string identifier.
	 *
	 * @return String identifier.
	 */
	const std::string& getName();

	/**
	 * @brief Retrieve Pipeline type COMPUTE/GRAPHICAL.
	 *
	 * @return Pipeline type.
	 */
	PipelineType getPipelineType();

	/**
	 * @brief Retrieves shader resource ids for the given device.
	 *
	 * @param device Device for which the shader ids will be retrieved.
	 * @return Vector of shader resource ids.
	 */
	std::vector<size_t> getShaderResourceIds(VulkanDevice* device);

	/**
	 * @brief Retrieve vector of Vulkan shader handles built for the given device.
	 *
	 * @param Device for which the shaders shall be retrieved.
	 * @return Vector containing shader handles.
	 */
	std::vector<vk::PipelineShaderStageCreateInfo> getVkShaderHandles(VulkanDevice* device);

	/**
	 * @brief Retrieves layout resource id for the given device.
	 *
	 * @param device Device for which the layout resource will be retrieved.
	 * @return Id of the layout resource.
	 */
	layout_id_t getLayoutResourceId(VulkanDevice* device);

	/**
	 * @brief Retrieve Vulkan layout handle build for the given device.
	 *
	 * @param Vulkan device for which the layout shall be retrieved.
	 * @return Pipeline layout.
	 */
	vk::PipelineLayout getVkHandle(VulkanDevice* device);

	/**
	 * @brief Retrieves descriptors count for the given pipeline layout.
	 *
	 * @return DescriptorsCount object.
	 */
	const DescriptorsCount& getDescriptorsCount();

private:
	std::string name_; ///< Layout name.
	PipelineType type_; ///< Pipeline type (graphical or compute).
	std::vector<ShaderData*> shaders_; ///< Pipeline shaders.
	std::vector<DescriptorSetLayout> descriptor_sets_; ///< Descriptor sets.
	std::vector<PushConstant> push_constants_; ///< Push constants.
	DescriptorsCount combined_descriptors_count_;

	std::vector<VertexAttribute> attributes_; ///< Only for graphical pipelines.
	std::unordered_map<VulkanDevice*, layout_id_t> device_to_layout;

	/**
	 * @brief Perform shader reflection using spriv-cross.
	 *
	 * @param shader Shader data.
	 */
	void shaderReflection(const ShaderData* shader);

	void addPushConstant(const std::string& name, vk::ShaderStageFlagBits stage, uint32_t size);

	/**
	 * @brief Builds vulkan pipeline layout for the given device.
	 *
	 */
	std::tuple<size_t, vk::PipelineLayout> buildVkPipelineLayout(VulkanDevice* device);
};

}

#endif