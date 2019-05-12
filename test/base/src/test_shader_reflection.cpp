#include <gtest/gtest.h>
#include "logi/logi.hpp"
#include "logi/structures/vk_structures.hpp"

template <typename VkStructureType>
class Structure {
 protected:
  VkStructureType structure_;

  virtual void updateVkStructure() {}

 public:
  operator VkStructureType() {
    updateVkStructure();
    return structure_;
  }
};

class InstanceCreateInfo : public Structure<vk::InstanceCreateInfo> {
 private:
  vk::InstanceCreateFlags& flags = structure_.flags;
};

TEST(ShaderReflection, Basic) {
  vk::ApplicationInfo appInfo;
  appInfo.pApplicationName = "TestApp";
  appInfo.pEngineName = "TestEngine";
  appInfo.apiVersion = VK_API_VERSION_1_1;
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

  vk::InstanceCreateInfo instanceCI;
  instanceCI.pApplicationInfo = &appInfo;
  instanceCI.enabledExtensionCount = 0;
  std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
  instanceCI.enabledLayerCount = validationLayers.size();
  instanceCI.ppEnabledLayerNames = validationLayers.data();

  logi::VulkanInstance instance = logi::createInstance(instanceCI);
}
