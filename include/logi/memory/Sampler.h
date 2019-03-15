#ifndef MEMORY_SAMPLER_H
#define MEMORY_SAMPLER_H

#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/base/ManagedResource.h"

namespace logi {

struct SamplerConfiguration {
  SamplerConfiguration(vk::Filter mag_filter = vk::Filter::eNearest, vk::Filter min_filter = vk::Filter::eNearest,
                       vk::SamplerMipmapMode mipmap_mode = vk::SamplerMipmapMode::eNearest,
                       vk::SamplerAddressMode address_mode_u = vk::SamplerAddressMode::eRepeat,
                       vk::SamplerAddressMode address_mode_v = vk::SamplerAddressMode::eRepeat,
                       vk::SamplerAddressMode address_mode_w = vk::SamplerAddressMode::eRepeat,
                       float mip_lod_bias = 0.0f, bool anisotropy_enable = false, float max_anisotropy = 0.0f,
                       bool compare_enable = false, vk::CompareOp compare_op = vk::CompareOp::eNever,
                       float min_lod = 0.0f, float max_lod = 0.0f,
                       vk::BorderColor border_color = vk::BorderColor::eFloatTransparentBlack,
                       bool unnormalized_coordinates = false,
                       const vk::SamplerCreateFlags& flags = vk::SamplerCreateFlags());

  vk::SamplerCreateInfo buildVkConfig() const;

  vk::SamplerCreateFlags flags;
  vk::Filter mag_filter;
  vk::Filter min_filter;
  vk::SamplerMipmapMode mipmap_mode;
  vk::SamplerAddressMode address_mode_u;
  vk::SamplerAddressMode address_mode_v;
  vk::SamplerAddressMode address_mode_w;
  float mip_lod_bias;
  bool anisotropy_enable;
  float max_anisotropy;
  bool compare_enable;
  vk::CompareOp compare_op;
  float min_lod;
  float max_lod;
  vk::BorderColor border_color;
  bool unnormalized_coordinates;
};

class LogicalDevice;

class Sampler : public DestroyableOwnedHandle<LogicalDevice> {
 public:
  /**
   * Default placeholder constructor.
   */
  Sampler() = default;

  Sampler(const LogicalDevice& device, const SamplerConfiguration& config);

  const vk::Sampler& getVkHandle() const;

  operator vk::Sampler() const;

 protected:
  void free() override;

 private:
  using ManagedVkSampler =
    ManagedResource<vk::Device, vk::Sampler, vk::DispatchLoaderStatic, &vk::Device::destroySampler>;

  struct Data {
    Data(ManagedVkSampler vk_sampler, SamplerConfiguration config);

    ManagedVkSampler vk_sampler;
    SamplerConfiguration config;
  };

  std::shared_ptr<Data> data_;
};

} // namespace logi

#endif
