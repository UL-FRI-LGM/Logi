#include <gtest/gtest.h>
#include "logi/logi.hpp"

struct InstanceCreateInfo {
 private:
  vk::InstanceCreateInfo create_info;
};

struct DebugReportCallbackCreateInfoEXT {};

using namespace logi;

TEST(Instance, Vk) {
  vk::ApplicationInfo app_info;
  app_info.pApplicationName = "app";
  app_info.pEngineName = "engine";

  vk::InstanceCreateInfo ci;
  ci.pApplicationInfo = &app_info;

  VulkanInstance instance = createInstance(ci);
  VulkanInstance second = instance;
  std::vector<PhysicalDevice> devices = second.enumeratePhysicalDevices();
  int a = 0;
  // devices[0].getExternalBufferProperties();

  // std::vector<PhysicalDevice> devices = instance->getPhysicalDevices();
}