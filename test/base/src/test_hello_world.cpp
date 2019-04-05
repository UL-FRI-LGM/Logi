#include <gtest/gtest.h>
#include "logi/logi.hpp"

template <typename VkStructureType>
class Structure {
  template <typename T, typename... E>
  friend class ExtendableStructure;

 public:
  virtual VkStructureType& build() {
    buildStructure();
    link();
    return structure_;
  };

  operator const VkStructureType&() {
    return build();
  }

 protected:
  virtual void buildStructure() = 0;

  VkStructureType structure_;

 private:
  virtual void link() {}
};

template <typename VkStructureType, typename... Extensions>
class ExtendableStructure : public Structure<VkStructureType> {
 public:
  template <typename T>
  void isEnabled() {
    return (!std::get<std::optional<T>>(extensions_).has_value());
  }

  template <typename T>
  T& get() {
    if (!std::get<std::optional<T>>(extensions_).has_value()) {
      return std::get<std::optional<T>>(extensions_).emplace();
    }

    return std::get<std::optional<T>>(extensions_).value();
  }

 private:
  void link() final {
    const void** p_next = &Structure<VkStructureType>::structure_.pNext;

    auto linker = [&p_next](auto& extension) {
      if (extension) {
        auto& vk_extension = extension.value().build();
        *p_next = static_cast<const void*>(&vk_extension);
        p_next = &vk_extension.pNext;
      }
    };

    std::apply([&linker](auto&... ext) { (..., linker(ext)); }, extensions_);
  }

  std::tuple<std::optional<Extensions>...> extensions_;
};

struct ValidationFlagsEXT : public Structure<vk::ValidationFlagsEXT> {
  std::vector<vk::ValidationCheckEXT> disabled_validation_checks;

 public:
  void buildStructure() override {
    structure_.disabledValidationCheckCount = disabled_validation_checks.size();
    structure_.pDisabledValidationChecks = disabled_validation_checks.data();
  }
};

/*
VkInstanceCreateFlags       flags;
const VkApplicationInfo*    pApplicationInfo;
uint32_t                    enabledLayerCount;
const char* const*          ppEnabledLayerNames;
uint32_t                    enabledExtensionCount;
const char* const*          ppEnabledExtensionNames;*/

struct InstanceCreateInfo : public ExtendableStructure<vk::InstanceCreateInfo, ValidationFlagsEXT> {
 public:
  vk::InstanceCreateFlags flags;
  vk::ApplicationInfo app_info;
  std::vector<const char*> enabled_layer_names;
  std::vector<const char*> enabled_extension_names;

 protected:
  void buildStructure() override {
    structure_.flags = flags;
    structure_.pApplicationInfo = &app_info;
    structure_.enabledLayerCount = enabled_layer_names.size();
    structure_.ppEnabledLayerNames = enabled_layer_names.data();
    structure_.enabledExtensionCount = enabled_extension_names.size();
    structure_.ppEnabledExtensionNames = enabled_extension_names.data();
  }
};

using namespace logi;

TEST(Instance, Vk) {
  InstanceCreateInfo ici;
  ValidationFlagsEXT& val_flags = ici.get<ValidationFlagsEXT>();
  val_flags.disabled_validation_checks.resize(1);
  auto& struc = val_flags.build();
  // val_flags.link();

  vk::InstanceCreateInfo create_info = ici;
  const VkValidationFlagsEXT* tst = reinterpret_cast<const VkValidationFlagsEXT*>(create_info.pNext);
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