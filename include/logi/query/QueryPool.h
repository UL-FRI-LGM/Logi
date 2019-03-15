#include "logi/base/ExtensionObject.h"
#include "logi/base/Handle.h"
#include "logi/base/ManagedResource.h"
#include <vulkan/vulkan.hpp>

namespace logi {

struct QueryPoolCreateInfo : public BuildableExtendable {
  explicit QueryPoolCreateInfo(const vk::QueryPoolCreateFlags& flags = {},
                               vk::QueryType query_type = vk::QueryType::eOcclusion, uint32_t query_count = 0,
                               const vk::QueryPipelineStatisticFlags& pipeline_statistics = {});

  vk::QueryPoolCreateInfo build() const;

  vk::QueryPoolCreateFlags flags;
  vk::QueryType query_type;
  uint32_t query_count;
  vk::QueryPipelineStatisticFlags pipeline_statistics;
};

class LogicalDevice;

class QueryPool : DestroyableOwnedHandle<LogicalDevice> {
 public:
  QueryPool() = default;

  QueryPool(const LogicalDevice& device, const QueryPoolCreateInfo& create_info);

  template <typename T>
  void getQueryPoolResults(uint32_t first_query, uint32_t query_count, vk::ArrayProxy<T> data, uint64_t stride,
                           const vk::QueryResultFlags& flags) const;

  const vk::QueryPool& getVkHandle() const;

 protected:
  void free() override;

 private:
  using ManagedVkQueryPool =
    ManagedResource<vk::Device, vk::QueryPool, vk::DispatchLoaderStatic, &vk::Device::destroyQueryPool>;

  std::shared_ptr<ManagedVkQueryPool> vk_query_pool_;
};

template <typename T>
void QueryPool::getQueryPoolResults(const uint32_t first_query, const uint32_t query_count, vk::ArrayProxy<T> data,
                                    const uint64_t stride, const vk::QueryResultFlags& flags) const {
  checkForNullHandleInvocation("QueryPool", "getQueryPoolResults");

  vk_query_pool_->getOwner().getQueryPoolResults(vk_query_pool_->get(), first_query, query_count, data, stride, flags);
}

} // namespace logi
