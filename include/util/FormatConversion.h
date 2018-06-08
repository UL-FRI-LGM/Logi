/*
* Vulkan Renderer base class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef UTIL_FORMAT_CONVERSION_H_
#define UTIL_FORMAT_CONVERSION_H_

#include <vulkan/vulkan.hpp>
#include <spirv_cross.hpp>

namespace logi {

/**
 * @brief Determines VkFormat based on SPIRType retrieved from the vertex shader code.
 *
 * @param format_info SPIRType retrieved from the vertex shader code.
 * @return VkFromat.
 */
vk::Format getVertexBufferFormat(const spirv_cross::SPIRType& format_info);

}

#endif // UTIL_FORMAT_CONVERSION_H_