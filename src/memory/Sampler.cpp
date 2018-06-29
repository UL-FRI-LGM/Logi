
#include "memory/Sampler.h"

namespace logi {

SamplerConfiguration::SamplerConfiguration(const vk::Filter mag_filter, const vk::Filter min_filter,
                                           const vk::SamplerMipmapMode mipmap_mode, 
										   const vk::SamplerAddressMode address_mode_u,
                                           const vk::SamplerAddressMode address_mode_v,
                                           const vk::SamplerAddressMode address_mode_w, const float mip_lod_bias,
                                           const bool anisotropy_enable, const float max_anisotropy,
                                           const bool compare_enable, const vk::CompareOp compare_op,
                                           const float min_lod, const float max_lod, const vk::BorderColor border_color,
                                           const bool unnormalized_coordinates,
                                           const vk::SamplerCreateFlags& flags)
	: flags(flags), mag_filter(mag_filter), min_filter(min_filter), mipmap_mode(mipmap_mode),
	  address_mode_u(address_mode_u), address_mode_v(address_mode_v), address_mode_w(address_mode_w),
	  mip_lod_bias(mip_lod_bias), anisotropy_enable(anisotropy_enable), max_anisotropy(max_anisotropy),
	  compare_enable(compare_enable), compare_op(compare_op), min_lod(min_lod), max_lod(max_lod),
	  border_color(border_color), unnormalized_coordinates(unnormalized_coordinates) {
}

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

Sampler::Sampler() : DependentDestroyableHandle({}, false), data_(nullptr) {
}

Sampler::Sampler(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const SamplerConfiguration& config)
	: DependentDestroyableHandle(owner), data_(std::make_shared<SamplerData>(device, config)) {
	data_->vk_sampler = device.createSampler(config.buildVkConfig());
}

const vk::Sampler& Sampler::getVkHandle() const {
	return data_->vk_sampler;
}

void Sampler::free() {
	data_->vk_device.destroySampler(data_->vk_sampler);
}

Sampler::SamplerData::SamplerData(const vk::Device& vk_device, const SamplerConfiguration& configuration)
	: vk_device(vk_device), configuration(configuration) {
}

}
