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

  vk::PhysicalDeviceFeatures features;
  vk::DeviceCreateInfo dci;
  vk::DeviceQueueCreateInfo qci;
  qci.queueFamilyIndex = 0;
  qci.queueCount = 1;
  float priority = 0.1;
  qci.pQueuePriorities = &priority;

  dci.pQueueCreateInfos = &qci;
  dci.queueCreateInfoCount = 1;
  dci.pEnabledFeatures = &features;

  LogicalDevice log_device = devices[0].createLogicalDevice(dci);
  int a = 0;
  second.destroy();
}