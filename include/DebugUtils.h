/*
* Vulkan Renderer debug configuration variables
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef _VKR_DEBUG_CONFIG_H_
#define _VKR_DEBUG_CONFIG_H_

#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <vector>

namespace vkr {
namespace debug_utils {

// Validation constants:
const bool kEnableValidation = true;
const std::vector<char *> kValidationLayers{
	"VK_LAYER_LUNARG_standard_validation"
};

constexpr VkDebugReportFlagBitsEXT kDebugFlags = static_cast<VkDebugReportFlagBitsEXT>(VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT); ///< Types of messages that will be handled by the debug callback.

}
}

#endif  // _VKR_DEBUG_CONFIG_H_