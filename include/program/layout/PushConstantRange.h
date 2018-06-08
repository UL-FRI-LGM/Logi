#ifndef PROGRAM_LAYOUT_PUSH_CONSTANT_RANGE_H
#define PROGRAM_LAYOUT_PUSH_CONSTANT_RANGE_H

#include <vulkan/vulkan.hpp>

namespace logi {
namespace internal {

/**
 * @brief Structure used to accumulate the push constant data during shader reflection.
 */
struct PushConstantRangeInitializer {
	uint32_t offset;				///< Push constant offset.
	uint32_t size;					///< Push constant size.
	vk::ShaderStageFlags stages;	///< Stage in which the push constant will be available.

	/**
	 * @brief	Default constructor.
	 *
	 * @param	offset	Push constant offset.
	 * @param	size	Push constant size.
	 * @param	stages	Stage in which the push constant will be available.
	 */
	PushConstantRangeInitializer(uint32_t offset, uint32_t size, vk::ShaderStageFlags stages);
};

} ///!	namespace internal

/**
 * @brief Wraps Vulkan PushConstant data.
 */
class PushConstantRange {
 public:
	/**
	 * @brief Default constructor. Initializes push constant members.
	 *
	 * @param initializer Structure containing the stages in which the push constant will be available, push constant offset and size.
	 */
	PushConstantRange(const internal::PushConstantRangeInitializer& initializer);

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
	 * @brief Retrieve the stages in which the push constant will be available.
	 *
	 * @return Shader stage flags.
	 */
	vk::ShaderStageFlags getStages() const;

	/**
	 * @brief Build Vulkan push constant structure.
	 *
	 * @return Vulkan push constant structure.
	 */
	const vk::PushConstantRange& getVkHandle() const;

private:
	uint32_t offset_;					///< Push constant offset.
	uint32_t size_;						///< Push constant size.
	vk::ShaderStageFlags stages_;		///< Stages in which the push constant will be available.
	vk::PushConstantRange vk_handle_;	///< Vulkan handle.
};

} ///!	namespace logi 

#endif ///!	HEADER_GUARD