#include "vk_texture.h"
#include <vulkan/vulkan.h>

namespace vkr {

VulkanTexture::VulkanTexture(VkImage image, VulkanDevice *owner, bool manage_dealloc): image_(image), owner_(owner), manage_dealloc_(manage_dealloc) {

}

VulkanTexture::~VulkanTexture() {

}

}