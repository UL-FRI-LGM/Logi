#ifndef PROGRAM_PIPELINE_LAYOUT_DESCRIPTOR_BINDING_LAYOUT_H
#define PROGRAM_PIPELINE_LAYOUT_DESCRIPTOR_BINDING_LAYOUT_H

#include <vulkan/vulkan.hpp>

namespace vkr {
namespace internal {

/**
 * @brief	Structure used to accumulate the descriptor binding data during shader reflection.
 */
struct DescriptorBindingInitializer {
	uint32_t binding;				///< Binding index.
	vk::DescriptorType type;		///< Binding descriptor type.	
	uint32_t count;					///< Number of array entries.
	vk::ShaderStageFlags stages;	///< Stages in which the binding should be available.

	/**
	 * @brief	Default constructor.
	 *
	 * @param	binding Binding index.
	 * @param	type	Binding descriptor type.
	 * @param	count	Number of array entries.
	 * @param	stages	Stages in which the binding should be available.
	 */
	DescriptorBindingInitializer(uint32_t binding, vk::DescriptorType type, uint32_t count, vk::ShaderStageFlags stages);
};

} ///! namespace internal

/**
 * @brief	Wraps Vulkan DescriptorBindingLayout data.
 */
class DescriptorBindingLayout {
public:
	/**
	 * @brief	Initializes members with the given values.
	 *
	 * @param	binding_initializer Structure containing binding index, descriptor type, number of array entries and stages in which the binding should be available.
	 */
	DescriptorBindingLayout(const internal::DescriptorBindingInitializer& binding_initializer);

	/**
	 * @brief	Retrieve binding index.
	 *
	 * @return	Binding index.
	 */
	uint32_t getBinding() const;

	/**
	 * @brief	Retrieve binding descriptor type.
	 *
	 * @return	Binding descriptor type.
	 */
	vk::DescriptorType getDescriptorType() const;

	/**
	 * @brief	Retrieve binding count.
	 *
	 * @return	Binding count.
	 */
	uint32_t getCount() const;

	/**
	 * @brief	Retrieve stages in which the binding will be available.
	 *
	 * @return	Binding stages.
	 */
	const vk::ShaderStageFlags& getShaderStages() const;

	/**
	 * @brief	Build Vulkan binding structure from the data.
	 *
	 * @return	Vulkan binding structure.
	 */
	const vk::DescriptorSetLayoutBinding& getVkHandle() const;

private:
	uint32_t binding_;							///< Binding index.
	vk::DescriptorType type_;					///< Binding descriptor type.
	uint32_t count_;							///< Number of array entries.
	vk::ShaderStageFlags stages_;				///< Stages in which the binding should be available.
	vk::DescriptorSetLayoutBinding vk_handle_;	///< Vulkan handle.
};

} ///!	namespace vkr

#endif ///!	HEADER_GUARD