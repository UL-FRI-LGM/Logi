#ifndef PROGRAM_LAYOUT_SHADER_H
#define PROGRAM_LAYOUT_SHADER_H

#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>
#include "logi/base/ExtensionObject.h"
#include "logi/base/Handle.h"
#include "logi/base/ManagedResource.h"

namespace logi {

/**
 * @brief   Structure that contains shader module code and configuration.
 */
struct ShaderModuleConfig : public BuildableExtendable {
  /**
   * @brief   Initialize ShaderModuleConfig members.
   *
   * @param	code	Shader module code.
   * @param	flags	Additional configuration flags.
   */
  explicit ShaderModuleConfig(std::vector<uint32_t> code, const vk::ShaderModuleCreateFlags& flags = {});

  /**
   * @brief	Builds Vulkan ShaderModule create info structure.
   *
   * @return	Vulkan ShaderModule create info structure.
   */
  vk::ShaderModuleCreateInfo build() const;

  /**
   * Additional configuration flags.
   */
  vk::ShaderModuleCreateFlags flags;

  /**
   * Shader code.
   */
  std::vector<uint32_t> code;
};

class ProgramManager;

/**
 * @brief   ShaderModule handle that manages Vulkan ShaderModule object.
 */
class ShaderModule : public DestroyableOwnedHandle<ProgramManager> {
 public:
  /**
   * @brief   Placeholder ShaderModule handle constructor.
   */
  ShaderModule() = default;

  /**
   * @brief	Creates ShaderModule handle.
   *
   * @param	program_manager	Program manager that owns this shader module.
   * @param	config	        Shader data and configuration.
   */
  ShaderModule(const ProgramManager& program_manager, const ShaderModuleConfig& config);

  /**
   * @brief	Get shader configuration.
   *
   * @return	Reference to the shader configuration.
   */
  const ShaderModuleConfig& config() const;

  /**
   * @brief   Get shader code.
   *
   * @return  Reference to the shader code.
   */
  const std::vector<uint32_t>& code() const;

  /**
   * @brief	Get Vulkan shader module handle.
   *
   * @return	Vulkan shader module handle.
   */
  const vk::ShaderModule& getVkHandle() const;

 protected:
  /**
   * @brief   Frees Vulkan ShaderModule.
   */
  void free() override;

 private:
  using ManagedVkShaderModule =
    ManagedResource<vk::Device, vk::ShaderModule, vk::DispatchLoaderStatic, &vk::Device::destroyShaderModule>;

  /**
   * @brief   Contains ShaderModule handle internal data.
   */
  struct Data {
    /**
     * @brief   Inizialize shader module configuration.
     *
     * @param	config  Shader module configuration.
     */
    explicit Data(ShaderModuleConfig config);

    /**
     * Shader module configuration.
     */
    ShaderModuleConfig config;

    /**
     * Shader module handle.
     */
    ManagedVkShaderModule vk_shader_module;
  };

  /**
   * ShaderModule handle internal data.
   */
  std::shared_ptr<Data> data_;
};

/**
 * @brief   Structure that contains shader specialization data.
 */
struct ShaderSpecialization {
  /**
   * @brief   Initialize ShaderSpecialization members.
   *
   * @param	map_entries	Array of vk::SpecializationMapEntry which maps constant IDs to offsets in data.
   * @param	data	    Contains the constant values to specialize with.
   */
  explicit ShaderSpecialization(std::vector<vk::SpecializationMapEntry> map_entries = {},
                                std::vector<unsigned char> data = {});

  /**
   * @brief   Builds Vulkan SpecializationInfo structure using the member values.
   *
   * @return	Vulkan SpecializationInfo structure
   */
  vk::SpecializationInfo* build();

  /**
   * Array of vk::SpecializationMapEntry which maps constant IDs to offsets in data.
   */
  std::vector<vk::SpecializationMapEntry> map_entries;

  /**
   * Contains the constant values to specialize with.
   */
  std::vector<unsigned char> data;

 private:
  /**
   * Vulkan specialization info structure.
   */
  vk::SpecializationInfo vk_specialization_;
};

/**
 * @brief   Structure that contains pipeline shader stage configuration.
 */
struct PipelineShaderStage : public BuildableExtendable {
  /**
   * @brief	Initialize PipelineShaderStage members.
   *
   * @param	stage			Value specifying a single pipeline stage.
   * @param	module			Handle that holds the shader for this stage.
   * @param	entry_point		Entry point name of the shader for this stage.
   * @param	flags			Additional configuration flags.
   * @param	specialization	Optional specialization constants.
   */
  explicit PipelineShaderStage(vk::ShaderStageFlagBits stage = {}, ShaderModule module = {},
                               std::string entry_point = "main", const vk::PipelineShaderStageCreateFlags& flags = {},
                               std::optional<ShaderSpecialization> specialization = std::nullopt);

  /**
   * @brief	Builds Vulkan PipelineShaderStageCreateInfo structure using the member values.
   *
   * @return	Vulkan PipelineShaderStageCreateInfo structure.
   */
  vk::PipelineShaderStageCreateInfo build();

  /**
   * @brief   Get shader code.
   *
   * @return  Reference to the shader code.
   */
  const std::vector<uint32_t>& code() const;

  /**
   * Additional configuration flags.
   */
  vk::PipelineShaderStageCreateFlags flags;

  /**
   * Value specifying a single pipeline stage.
   */
  vk::ShaderStageFlagBits stage;

  /**
   * Handle that holds the shader for this stage.
   */
  ShaderModule module;

  /**
   * Entry point name of the shader for this stage.
   */
  std::string entry_point;

  /**
   * Optional specialization constants.
   */
  std::optional<ShaderSpecialization> specialization;
};

} // namespace logi

#endif ///!	HEADER_GUARD
