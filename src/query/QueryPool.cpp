#include "logi/query/QueryPool.h"

#include "logi/base/LogicalDevice.h"

namespace logi {

QueryPoolCreateInfo::QueryPoolCreateInfo(const vk::QueryPoolCreateFlags& flags, const vk::QueryType query_type,
                                         const uint32_t query_count,
                                         const vk::QueryPipelineStatisticFlags& pipeline_statistics)
  : flags(flags), query_type(query_type), query_count(query_count), pipeline_statistics(pipeline_statistics) {}

vk::QueryPoolCreateInfo QueryPoolCreateInfo::build() const {
  vk::QueryPoolCreateInfo create_info = vk::QueryPoolCreateInfo(flags, query_type, query_count, pipeline_statistics);
  create_info.pNext = buildExtensions();

  return create_info;
}

QueryPool::QueryPool(const LogicalDevice& device, const QueryPoolCreateInfo& create_info)
  : DestroyableOwnedHandle(device, true), vk_query_pool_(nullptr) {
  const vk::Device& vk_device = device.getLogicalDeviceHandle();
  // Create descriptor pool.
  vk_query_pool_ = std::make_shared<ManagedVkQueryPool>(vk_device, vk_device.createQueryPool(create_info.build()));
}

const vk::QueryPool& QueryPool::getVkHandle() const {
  checkForNullHandleInvocation("QueryPool", "getVkHandle");

  return vk_query_pool_->get();
}

void QueryPool::free() {
  vk_query_pool_->destroy();
  DestroyableOwnedHandle::free();
}

} // namespace logi
