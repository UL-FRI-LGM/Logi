
#include "logi/memory/Sampler.h"
#include "logi/base/LogicalDevice.h"

namespace logi {

SamplerConfiguration::SamplerConfiguration(const vk::Filter mag_filter, const vk::Filter min_filter,
                                           const vk::SamplerMipmapMode mipmap_mode,
                                           const vk::SamplerAddressMode address_mode_u,
                                           const vk::SamplerAddressMode address_mode_v,
                                           const vk::SamplerAddressMode address_mode_w, const float mip_lod_bias,
                                           const bool anisotropy_enable, const float max_anisotropy,
                                           const bool compare_enable, const vk::CompareOp compare_op,
                                           const float min_lod, const float max_lod, const vk::BorderColor border_color,
                                           const bool unnormalized_coordinates, const vk::SamplerCreateFlags& flags)
  : flags(flags), mag_filter(mag_filter), min_filter(min_filter), mipmap_mode(mipmap_mode),
    address_mode_u(address_mode_u), address_mode_v(address_mode_v), address_mode_w(address_mode_w),
    mip_lod_bias(mip_lod_bias), anisotropy_enable(anisotropy_enable), max_anisotropy(max_anisotropy),
    compare_enable(compare_enable), compare_op(compare_op), min_lod(min_lod), max_lod(max_lod),
    border_color(border_color), unnormalized_coordinates(unnormalized_coordinates) {}

vk::SamplerCreateInfo SamplerConfiguration::buildVkConfig() const {
  vk::SamplerCreateInfo ci;
  ci.magFilter = mag_filter;
  ci.minFilter = min_filter;
  ci.mipmapMode = mipmap_mode;
  ci.addressModeU = address_mode_u;
  ci.addressModeV = address_mode_v;
  ci.addressModeW = address_mode_w;
  ci.mipLodBias = mip_lod_bias;
  ci.anisotropyEnable = anisotropy_enable;
  ci.maxAnisotropy = max_anisotropy;
  ci.compareEnable = compare_enable;
  ci.compareOp = compare_op;
  ci.minLod = min_lod;
  ci.maxLod = max_lod;
  ci.borderColor = border_color;
  ci.unnormalizedCoordinates = unnormalized_coordinates;
  ci.flags = flags;

  return ci;
}

Sampler::Sampler(const LogicalDevice& device, const SamplerConfiguration& config)
  : DestroyableOwnedHandle<LogicalDevice>(device, true), data_(nullptr) {
  vk::Device vk_device = device;
  data_ = std::make_shared<Data>(ManagedVkSampler(vk_device, vk_device.createSampler(config.buildVkConfig())), config);
}

const vk::Sampler& Sampler::getVkHandle() const {
  checkForNullHandleInvocation("Sampler", "getVkHandle");
  return data_->vk_sampler.get();
}

Sampler::operator vk::Sampler() const {
  checkForNullHandleInvocation("Sampler", "operator vk::Sampler(");
  return data_->vk_sampler.get();
}

void Sampler::free() {
  if (valid()) {
    data_->vk_sampler.destroy();
  }
}

Sampler::Data::Data(ManagedVkSampler vk_sampler, SamplerConfiguration config)
  : vk_sampler(vk_sampler), config(std::move(config)) {}

} // namespace logi
