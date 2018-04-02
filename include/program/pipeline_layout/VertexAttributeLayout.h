#ifndef PROGRAM_PIPELINE_LAYOUT_VERTEX_ATTRIBUTE_LAYOUT_H
#define PROGRAM_PIPELINE_LAYOUT_VERTEX_ATTRIBUTE_LAYOUT_H

#include <vulkan/vulkan.hpp>

namespace vkr {

/**
 * @brief	Encapsulates vertex attribute layout data.
 */
class VertexAttributeLayout {
public:
	/**
	 * @brief	Default constructor. Initialize data members.
	 *
	 * @param	location	Attribute location index.
	 * @param	binding		Attribute binding index.
	 * @param	format		Attribute data format.
	 * @param	offset		Offset in bytes relative to the start of element.
	 * @param	stride		Distance in bytes between two consecutive elements.
	 * @param	input_rate	Per vertex or per instance.
	 */
	VertexAttributeLayout(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset, uint32_t stride, vk::VertexInputRate input_rate);

	/**
	 * @brief	Build VertexInputBindingDescription Vulkan structure.
	 *
	 * @return	VertexInputBindingDescription Vulkan structure
	 */
	vk::VertexInputBindingDescription getVkBindingDescription() const;

	/**
	 * @brief	Build VertexInputAttributeDescription Vulkan structure.
	 *
	 * @return	VertexInputAttributeDescription Vulkan structure
	 */
	vk::VertexInputAttributeDescription getVkAttributeDescription() const;

	/**
	 * @brief	Retrieve attribute location.
	 *
	 * @return	Attribute location.
	 */
	uint32_t getLocation() const;

	/**
	 * @brief	Retrieve attribute binding.
	 *
	 * @return	Attribute binding.
	 */
	uint32_t getBinding() const;

	/**
	 * @brief	Retrieve attribute data format.
	 *
	 * @return	Attribute data format.
	 */
	vk::Format getFormat() const;

	/**
	 * @brief	Retrieve attribute offset.
	 *
	 * @return	Attribute offset.
	 */
	uint32_t getOffset() const;

	/**
	 * @brief	Retrieve attribute stride.
	 *
	 * @return	Attribute stride.
	 */
	uint32_t getStride() const;

	/**
	 * @brief	Retrieve attribute input rate.
	 *
	 * @return	Attribute location.
	 */
	vk::VertexInputRate getInputRate() const;

private:
	uint32_t location_;					///< Attribute location index.
	uint32_t binding_;					///< Attribute binding index.
	vk::Format format_;					///< Attribute data format.
	uint32_t offset_;					///< Offset in bytes relative to the start of element.
	uint32_t stride_;					///< Distance in bytes between two consecutive elements.
	vk::VertexInputRate input_rate_;	///< Per vertex or per instance.
};

} ///!	namespace vkr

#endif ///!	HEADER_GUARD