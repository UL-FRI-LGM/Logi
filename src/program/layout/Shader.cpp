#include "logi/program/layout/Shader.h"
#include "logi/base/LogicalDevice.h"

namespace logi {

ShaderModuleConfig::ShaderModuleConfig(std::vector<uint32_t> code, const vk::ShaderModuleCreateFlags& flags)
  : flags(flags), code(std::move(code)) {}

vk::ShaderModuleCreateInfo ShaderModuleConfig::build() const {
  vk::ShaderModuleCreateInfo create_info(flags, code.size() * 4, code.data());
  create_info.pNext = buildExtensions();

  return create_info;
}

ShaderModule::ShaderModule(const ProgramManager& program_manager, const ShaderModuleConfig& config)
  : DestroyableOwnedHandle<ProgramManager>(program_manager), data_(std::make_shared<Data>(config)) {
  vk::Device vk_device = getOwner<LogicalDevice>();
  data_->vk_shader_module = ManagedVkShaderModule(vk_device, vk_device.createShaderModule(data_->config.build()));
}

const ShaderModuleConfig& ShaderModule::config() const {
  checkForNullHandleInvocation("ShaderModule", "config");
  return data_->config;
}

const std::vector<uint32_t>& ShaderModule::code() const {
  checkForNullHandleInvocation("ShaderModule", "code");
  return data_->config.code;
}

const vk::ShaderModule& ShaderModule::getVkHandle() const {
  checkForNullHandleInvocation("ShaderModule", "getVkHandle");
  return data_->vk_shader_module.get();
}

void ShaderModule::free() {
  if (valid()) {
    data_->vk_shader_module.destroy();
    DestroyableOwnedHandle<ProgramManager>::free();
  }
}

ShaderModule::Data::Data(ShaderModuleConfig config) : config(std::move(config)) {}

ShaderSpecialization::ShaderSpecialization(std::vector<vk::SpecializationMapEntry> map_entries,
                                           std::vector<unsigned char> data)
  : map_entries(std::move(map_entries)), data(std::move(data)) {}

vk::SpecializationInfo* ShaderSpecialization::build() {
  vk_specialization_.mapEntryCount = static_cast<uint32_t>(map_entries.size());
  vk_specialization_.pMapEntries = map_entries.data();
  vk_specialization_.dataSize = data.size();
  vk_specialization_.pData = data.data();

  return &vk_specialization_;
}

PipelineShaderStage::PipelineShaderStage(const vk::ShaderStageFlagBits stage, ShaderModule module,
                                         std::string entry_point, const vk::PipelineShaderStageCreateFlags& flags,
                                         std::optional<ShaderSpecialization> specialization)
  : flags(flags), stage(stage), module(std::move(module)), entry_point(std::move(entry_point)),
    specialization(std::move(specialization)) {}

vk::PipelineShaderStageCreateInfo PipelineShaderStage::build() {
  vk::PipelineShaderStageCreateInfo create_info(flags, stage, module.getVkHandle(), entry_point.c_str(),
                                                specialization.has_value() ? specialization.value().build() : nullptr);
  create_info.pNext = buildExtensions();

  return create_info;
}

const std::vector<uint32_t>& PipelineShaderStage::code() const {
  return module.code();
}

} // namespace logi