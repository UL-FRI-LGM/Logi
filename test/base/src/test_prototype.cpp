#include <gtest/gtest.h>
#include "logi/logi.hpp"
#include "logi/structures/structure.hpp"
#include "logi/structures/vk_structures.hpp"

using namespace logi;

struct DebugReportCallbackCreateInfoEXT : public Structure<vk::DebugReportCallbackCreateInfoEXT> {
  using vk::DebugReportCallbackCreateInfoEXT::flags;
  using vk::DebugReportCallbackCreateInfoEXT::pfnCallback;
  using vk::DebugReportCallbackCreateInfoEXT::pUserData;
};

struct ValidationFeaturesEXT : public Structure<vk::ValidationFeaturesEXT> {
  std::vector<vk::ValidationFeatureEnableEXT> enabledValidationFeatures;
  std::vector<vk::ValidationFeatureDisableEXT> disabledValidationFeatures;

  void updateVkStructure() override {
    vecToCArr(enabledValidationFeatures, pEnabledValidationFeatures, enabledValidationFeatureCount);
    vecToCArr(disabledValidationFeatures, pDisabledValidationFeatures, disabledValidationFeatureCount);
  }
};

struct InstanceCreateInfo : public Structure<vk::InstanceCreateInfo> {
 public:
  using vk::InstanceCreateInfo::flags;
  vk::ApplicationInfo appInfo;
  std::vector<const char*> enabledLayerNames;
  std::vector<const char*> enabledExtensionNames;

  void updateVkStructure() override {
    pApplicationInfo = &appInfo;
    vecToCArr(enabledLayerNames, ppEnabledLayerNames, enabledLayerCount);
    vecToCArr(enabledExtensionNames, ppEnabledExtensionNames, enabledExtensionCount);
  }
};

TEST(Prototype, Vk) {
  DescriptorSetLayoutCreateInfoChain chain(true);
  chain.get<DescriptorSetLayoutCreateInfo>().bindings.resize(10);

  vk::DescriptorSetLayoutCreateInfo test = chain;
  int a = 0;

  vk::Device device;
  vk::CommandBuffer cbuf;

  /*
  InstanceCreateInfo ici;
  ici.enable<DebugReportCallbackCreateInfoEXT, ValidationFeaturesEXT>();
  ici.get<ValidationFeaturesEXT>().enabledValidationFeatures.emplace_back();



  vk::InstanceCreateInfo& vk_ici = ici.build();
  const vk::DebugReportCallbackCreateInfoEXT* test = reinterpret_cast<const
  vk::DebugReportCallbackCreateInfoEXT*>(vk_ici.pNext); const vk::ValidationFeaturesEXT* test2 = reinterpret_cast<const
  vk::ValidationFeaturesEXT*>(test->pNext); int a = 3;
   */
}