#include <fstream>
#include <gtest/gtest.h>
#include "logi/logi.hpp"
#include "logi/shader_reflection/shader_reflector.hpp"
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

std::vector<uint32_t> loadShader(const std::string& path) {
  std::ifstream file(path, std::ios::ate | std::ios::binary);

  // Check if the shader file was successfully opened.
  if (!file.is_open()) {
    throw std::runtime_error("Failed to load shader file: \"" + path + "\". Make sure that the file exists.");
  }

  const size_t file_size = static_cast<size_t>(file.tellg());
  std::vector<uint32_t> buffer(file_size / sizeof(uint32_t));

  file.seekg(0);
  file.read(reinterpret_cast<char*>(buffer.data()), file_size);
  file.close();

  return buffer;
}

TEST(Prototype, Vk) {
  DescriptorSetLayoutCreateInfoChain chain(true);
  chain.get<DescriptorSetLayoutCreateInfo>().bindings.resize(10);

  vk::DescriptorSetLayoutCreateInfo test = chain;

  ShaderReflector reflector;
  std::vector<uint32_t> vertex = loadShader("testdata/base.vert.spv");
  std::vector<uint32_t> frag = loadShader("testdata/base.frag.spv");
  std::vector<uint32_t> control = loadShader("testdata/pntriangles.tesc.spv");
  std::vector<uint32_t> eval = loadShader("testdata/pntriangles.tese.spv");

  reflector.reflectShader(vertex.data(), vertex.size(), "main");
  reflector.reflectShader(frag.data(), frag.size(), "main");
  reflector.reflectShader(control.data(), control.size(), "main");
  reflector.reflectShader(eval.data(), eval.size(), "main");

  int a = 0;

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