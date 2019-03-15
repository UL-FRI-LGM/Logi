#include "logi/memory/AllocationManager.h"
#include "logi/base/LogicalDevice.h"

namespace logi {

AllocationManager::AllocationManager(const LogicalDevice& device)
  : OwnedHandle<LogicalDevice>(device, true), allocator_(nullptr) {
  // Create VMA for the given device.
  VmaAllocatorCreateInfo allocator_info;
  allocator_info.physicalDevice = static_cast<VkPhysicalDevice>(device.getPhysicalDeviceHandle());
  allocator_info.device = static_cast<VkDevice>(device.getLogicalDeviceHandle());

  vmaCreateAllocator(&allocator_info, &allocator_);
}

Buffer AllocationManager::createBuffer(const BufferConfiguration& configuration) {
  checkForNullHandleInvocation("AllocationManager", "createBuffer");

  // Build buffer create info.
  vk::BufferCreateInfo buffer_ci{};
  buffer_ci.flags = configuration.flags;
  buffer_ci.size = configuration.size;
  buffer_ci.usage = configuration.usage;

  // If the buffer is required to be concurrent add concurrent queue families.
  if (configuration.concurrent_queue_families.size() <= 1u) {
    buffer_ci.sharingMode = vk::SharingMode::eExclusive;
  } else {
    buffer_ci.sharingMode = vk::SharingMode::eConcurrent;
    buffer_ci.pQueueFamilyIndices = configuration.concurrent_queue_families.data();
    buffer_ci.queueFamilyIndexCount = static_cast<uint32_t>(configuration.concurrent_queue_families.size());
  }

  // High level usage type.
  VmaAllocationCreateInfo alloc_info = {};
  alloc_info.usage = static_cast<VmaMemoryUsage>(configuration.memory_usage);

  // Allocate Image.
  vk::Buffer buffer;
  VmaAllocation allocation;
  if (vmaCreateBuffer(allocator_, reinterpret_cast<const VkBufferCreateInfo*>(&buffer_ci), &alloc_info,
                      reinterpret_cast<VkBuffer*>(&buffer), &allocation, nullptr) != VK_SUCCESS) {
    throw std::runtime_error("vmaCreateBuffer failed. TODO: Handle this.");
  }

  // Create and register Buffer handle.
  return HandleGenerator<AllocationManager, Buffer>::createHandle(getOwner<LogicalDevice>(), buffer, allocator_,
                                                                  allocation, configuration);
}

Image AllocationManager::createImage(const ImageConfiguration& configuration) {
  // Make sure that AllocationManager is still alive.
  checkForNullHandleInvocation("AllocationManager", "createImage");

  // Build image create info.
  vk::ImageCreateInfo image_ci{};
  image_ci.flags = configuration.create_flags;
  image_ci.imageType = configuration.type;
  image_ci.format = configuration.format;
  image_ci.extent = configuration.extent;
  image_ci.mipLevels = configuration.mip_levels;
  image_ci.arrayLayers = configuration.array_layers;
  image_ci.samples = configuration.samples;
  image_ci.tiling = configuration.tiling;
  image_ci.usage = configuration.usage;
  image_ci.initialLayout = configuration.initial_layout;

  // If the image is required to be concurrent add concurrent queue families.
  if (configuration.concurrent_queue_families.size() <= 1u) {
    image_ci.sharingMode = vk::SharingMode::eExclusive;
  } else {
    image_ci.sharingMode = vk::SharingMode::eConcurrent;
    image_ci.pQueueFamilyIndices = configuration.concurrent_queue_families.data();
    image_ci.queueFamilyIndexCount = static_cast<uint32_t>(configuration.concurrent_queue_families.size());
  }

  // High level usage type.
  VmaAllocationCreateInfo alloc_info = {};
  alloc_info.usage = static_cast<VmaMemoryUsage>(configuration.memory_usage);

  // Allocate Image.
  vk::Image image;
  VmaAllocation allocation;
  if (vmaCreateImage(allocator_, reinterpret_cast<const VkImageCreateInfo*>(&image_ci), &alloc_info,
                     reinterpret_cast<VkImage*>(&image), &allocation, nullptr) != VK_SUCCESS) {
    throw std::runtime_error("vmaCreateImage failed. TODO: Handle this.");
  }

  // Create and register Image handle.
  return HandleGenerator<AllocationManager, Image>::createHandle(getOwner<LogicalDevice>(), image, allocator_,
                                                                 allocation, configuration);
}

void AllocationManager::free() {
  if (valid()) {
    HandleGenerator<AllocationManager, Buffer>::destroyAllHandles();
    HandleGenerator<AllocationManager, Image>::destroyAllHandles();
    OwnedHandle<LogicalDevice>::free();
  }
}

} // namespace logi