#ifndef PROGRAM_LAYOUT_VERTEX_ATTRIBUTE_LAYOUT_H
#define PROGRAM_LAYOUT_VERTEX_ATTRIBUTE_LAYOUT_H

#include <vulkan/vulkan.hpp>

namespace logi {

/**
 * @brief	Encapsulates vertex attribute layout data.
 */
struct VertexAttributeLayout {
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

	uint32_t location;				///< Attribute location index.
	uint32_t binding;				///< Attribute binding index.
	vk::Format format;				///< Attribute data format.
	uint32_t offset;				///< Offset in bytes relative to the start of element.
	uint32_t stride;				///< Distance in bytes between two consecutive elements.
	vk::VertexInputRate input_rate;	///< Per vertex or per instance.
};

} ///!	namespace logi

#endif ///!	HEADER_GUARD