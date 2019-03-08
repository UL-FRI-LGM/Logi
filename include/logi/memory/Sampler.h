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
	                     float mip_lod_bias = 0.0f, bool anisotropy_enable = false,
	                     float max_anisotropy = 0.0f, bool compare_enable = false,
	                     vk::CompareOp compare_op = vk::CompareOp::eNever, float min_lod = 0.0f,
	                     float max_lod = 0.0f, vk::BorderColor border_color = vk::BorderColor::eFloatTransparentBlack,
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


class Sampler : public DependentDestroyableHandle {
public:
	Sampler();

	Sampler(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const SamplerConfiguration& config);

	const vk::Sampler& getVkHandle() const;

protected:
	void free() override;

private:
	struct SamplerData {
		SamplerData(const vk::Device& vk_device, const SamplerConfiguration& configuration);

		vk::Device vk_device;
		vk::Sampler vk_sampler{nullptr};
		SamplerConfiguration configuration;
	};

	std::shared_ptr<SamplerData> data_;
};

}

#endif
