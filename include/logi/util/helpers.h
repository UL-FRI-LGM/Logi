/*
* Vulkan Renderer base class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef UTIL_HELPERS_H_
#define UTIL_HELPERS_H_

#include <vulkan/vulkan.hpp>

namespace vkr {
bool hasRequestedFeatures(const vk::PhysicalDeviceFeatures& super, const vk::PhysicalDeviceFeatures& sub);

}

#endif