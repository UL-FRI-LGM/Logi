#include "logi/queues/QueueFamily.h"
#include <utility>
#include "logi/base/LogicalDevice.h"

namespace logi {

NvidiaQueueFamilyCheckpointProperties::NvidiaQueueFamilyCheckpointProperties(
  const vk::PipelineStageFlags& checkpoint_execution_stage_mask)
  : checkpoint_execution_stage_mask(checkpoint_execution_stage_mask) {}

std::unique_ptr<Extension> NvidiaQueueFamilyCheckpointProperties::clone() const {
  return std::make_unique<NvidiaQueueFamilyCheckpointProperties>(*this);
}

bool NvidiaQueueFamilyCheckpointProperties::operator==(const NvidiaQueueFamilyCheckpointProperties& rhs) const {
  return checkpoint_execution_stage_mask == rhs.checkpoint_execution_stage_mask;
}

QueueFamilyProperties::QueueFamilyProperties(const uint32_t family_index, const vk::QueueFamilyProperties& properties)
  : family_index(family_index), queue_flags(properties.queueFlags), queue_count(properties.queueCount),
    timestamp_valid_bits(properties.timestampValidBits),
    min_image_transfer_granularity(properties.minImageTransferGranularity) {}

QueueFamilyProperties::QueueFamilyProperties(const uint32_t family_index, const vk::QueueFamilyProperties2& properties)
  : family_index(family_index), queue_flags(properties.queueFamilyProperties.queueFlags),
    queue_count(properties.queueFamilyProperties.queueCount),
    timestamp_valid_bits(properties.queueFamilyProperties.timestampValidBits),
    min_image_transfer_granularity(properties.queueFamilyProperties.minImageTransferGranularity) {
  // Add extensions.
  void* next = properties.pNext;
  while (next != nullptr) {
    const auto* type = reinterpret_cast<const vk::StructureType*>(next);

    if (*type == vk::StructureType::eQueueFamilyCheckpointPropertiesNV) {
      const auto* ext = reinterpret_cast<vk::QueueFamilyCheckpointPropertiesNV*>(next);
      addExtensions(NvidiaQueueFamilyCheckpointProperties(ext->checkpointExecutionStageMask));
      next = ext->pNext;
    } else {
      throw InitializationError("Encountered unknown extension structure.");
    }
  }
}

QueueFamilyConfig::QueueFamilyConfig(QueueFamilyProperties properties, const uint32_t queue_count,
                                     std::vector<float> priorities, const vk::DeviceQueueCreateFlags& flags)
  : properties(std::move(properties)), queue_count(queue_count), priorities(std::move(priorities)), flags(flags) {
  // If there are too few priorities specified set missing priorities to lowest possible (0.0f).
  while (this->priorities.size() < queue_count) {
    this->priorities.emplace_back(0.0f);
  }
}

vk::DeviceQueueCreateInfo QueueFamilyConfig::build() const {
  vk::DeviceQueueCreateInfo create_info(flags, properties.family_index, queue_count, priorities.data());
  create_info.pNext = buildExtensions();

  return create_info;
}

QueueFamily::QueueFamily(const LogicalDevice& device, const QueueFamilyConfig& configuration)
  : OwnedHandle(device, true), data_(std::make_shared<Data>(device, configuration)) {
  // Create queues.
  data_->queues.reserve(configuration.queue_count);
  const vk::Device& vk_device = device.getLogicalDeviceHandle();

  for (uint32_t i = 0u; i < configuration.queue_count; i++) {
    data_->queues.emplace_back(HandleGenerator<QueueFamily, Queue>::createHandle(
      vk_device.getQueue(data_->configuration.properties.family_index, i)));
  }
}

CommandPool QueueFamily::createCommandPool(const vk::CommandPoolCreateFlags& flags) const {
  const vk::Device& vk_device = getOwner().getLogicalDeviceHandle();

  return HandleGenerator<QueueFamily, CommandPool>::createHandle(vk_device,
                                                                 data_->configuration.properties.family_index, flags);
}

const QueueFamilyConfig& QueueFamily::configuration() const {
  return data_->configuration;
}

Queue QueueFamily::getQueue(const size_t index) const {
  return data_->queues[index];
}

void QueueFamily::free() {
  HandleGenerator<QueueFamily, Queue>::destroyAllHandles();
  HandleGenerator<QueueFamily, CommandPool>::destroyAllHandles();
  OwnedHandle::free();
}

QueueFamily::Data::Data(QueueFamilyConfig config) : configuration(std::move(config)) {}

} // namespace logi
