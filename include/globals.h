/*
* Project       : VulkanRenderer
*
* Author        : Primoz Lavric
*
* For the terms and conditions of redistribution or use of this software
* refer to the file LICENSE located in the root level directory
* of the project.
*/

#ifndef _VKR_GLOBALS_H_
#define _VKR_GLOBALS_H_

#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <vector>

namespace globals {

// Application meta data:
constexpr char* kEngineName{ "Vulkan Renderer" };
constexpr uint32_t kEngineVersion { VK_MAKE_VERSION(0, 0, 1) };

constexpr char* kAppName{ "Med3D" };
constexpr uint32_t kAppVersion{ VK_MAKE_VERSION(0, 0, 1) };

// Validation constants:
constexpr bool kEnableValidation{ true };
const std::vector<char *> kValidationLayers {
	"VK_LAYER_LUNARG_standard_validation"
};

constexpr VkDebugReportFlagBitsEXT kDebugFlags = static_cast<VkDebugReportFlagBitsEXT>(VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT); ///< Types of messages that will be handled by the debug callback.

}

#endif  // _VKR_GLOBALS_H_