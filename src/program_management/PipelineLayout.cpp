#include "program_management/PipelineLayout.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <vulkan/vulkan.hpp>
#include <spirv_cross.hpp>
#include "util/FormatConversion.h"
#include "base/VulkanDevice.h"

namespace vkr {

#pragma region DescriptorBindingLayout

DescriptorBindingLayout::DescriptorBindingLayout(std::string name, uint32_t binding, vk::DescriptorType type, uint32_t count, vk::ShaderStageFlags stages) : name_(name), binding_(binding), type_(type), count_(count), stages_(stages) {
}

void DescriptorBindingLayout::addStages(vk::ShaderStageFlags stages) {
	stages_ |= stages;
}

const std::string& DescriptorBindingLayout::getName() const {
	return name_;
}

uint32_t DescriptorBindingLayout::getBinding() const {
	return binding_;
}

vk::DescriptorType DescriptorBindingLayout::getDescriptorType() const {
	return type_;
}

uint32_t DescriptorBindingLayout::getCount() const {
	return count_;
}

vk::ShaderStageFlags DescriptorBindingLayout::getShaderStages() const {
	return stages_;
}

vk::DescriptorSetLayoutBinding DescriptorBindingLayout::getVkHandle() const {
	return vk::DescriptorSetLayoutBinding(binding_, type_, count_, stages_, nullptr);
}
#pragma endregion

#pragma region DescriptorSet

DescriptorSet::DescriptorSet() : bindings_(), device_to_res_(), descriptors_count_(1u) {
}



void DescriptorSet::addDescriptorBinding(const DescriptorBindingLayout& binding) {
	auto it = bindings_.begin();
	for (; it != bindings_.end(); it++) {
		// If an entry with the same binding is found add stage flags.
		if (it->getBinding() == binding.getBinding()) {
			it->addStages(binding.getShaderStages());
			break;
		}
		else if (it->getBinding() > binding.getBinding()) {
			break;
		}
	}

	incrementDescriptorCount(binding.getDescriptorType());
	// Add new binding entry.
	bindings_.insert(it, binding);
}

void DescriptorSet::incrementDescriptorCount(const vk::DescriptorType& type) {
	switch (type) {
	case vk::DescriptorType::eSampler:
		descriptors_count_.samplers++;
		break;
	case vk::DescriptorType::eCombinedImageSampler:
		descriptors_count_.combined_image_samplers++;
		break;
	case vk::DescriptorType::eSampledImage:
		descriptors_count_.sampled_images++;
		break;
	case vk::DescriptorType::eStorageImage:
		descriptors_count_.storage_images++;
		break;
	case vk::DescriptorType::eUniformTexelBuffer:
		descriptors_count_.uniform_texel_buffers++;
		break;
	case vk::DescriptorType::eStorageTexelBuffer:
		descriptors_count_.storage_texel_buffers++;
		break;
	case vk::DescriptorType::eUniformBuffer:
		descriptors_count_.uniform_buffers++;
		descriptors_count_.uniform_buffers_dynamic++;
		break;
	case vk::DescriptorType::eStorageBuffer:
		descriptors_count_.storage_buffers++;
		descriptors_count_.storage_buffers_dynamic++;
		break;
	case vk::DescriptorType::eInputAttachment:
		descriptors_count_.input_attachments++;
		break;
	}
}

DescriptorBindingLayout* DescriptorSet::getDescriptorBinding(uint32_t binding) {
	for (auto it = bindings_.begin(); it != bindings_.end(); it++) {
		if (it->getBinding() == binding) {
			return &(*it);
		}
		else if (it->getBinding() > binding) {
			break;
		}
	}

	return nullptr;
}

descset_id_t DescriptorSet::getResourceId(VulkanDevice* device) {
	auto it = device_to_res_.find(device);

	if (it != device_to_res_.end()) {
		return it->second;
	}
	else {
		static_cast<void>(getVkHandle(device));
		return device_to_res_[device];
	}
}

vk::DescriptorSetLayout DescriptorSet::getVkHandle(VulkanDevice* device) {
	PipelineResources* resources = device->getPipelineResources();
	auto it = device_to_res_.find(device);

	// Check if this descriptor set layout was already allocated for this device.
	if (it != device_to_res_.end()) {
		vk::DescriptorSetLayout descriptor_set = resources->getDescriptorSetLayout(it->second);
		if (!descriptor_set) {
			return descriptor_set;
		}
	}

	// Generate bindings.
	std::vector<vk::DescriptorSetLayoutBinding> vk_bindings{};
	vk_bindings.reserve(bindings_.size());

	for (auto jt = bindings_.begin(); jt != bindings_.end(); jt++) {
		vk_bindings.push_back(jt->getVkHandle());
	}

	// Build descriptor set layout.
	std::tuple<descset_id_t, vk::DescriptorSetLayout> descriptor_set = resources->createDescriptorSetLayout(vk_bindings);
	device_to_res_[device] = std::get<0>(descriptor_set);

	return std::get<1>(descriptor_set);
}

const DescriptorsCount& DescriptorSet::getDescriptorsCount() {
	return descriptors_count_;
}

#pragma endregion

#pragma region PushConstant

PushConstant::PushConstant(std::string name, vk::ShaderStageFlags stages, uint32_t offset, uint32_t size) : name_(name), stages_(stages), offset_(offset), size_(size) {
}

void PushConstant::addStages(vk::ShaderStageFlags stages) {
	stages_ |= stages;
}

const std::string& PushConstant::getName() const {
	return name_;
}

vk::ShaderStageFlags PushConstant::getStages() const {
	return stages_;
}

uint32_t PushConstant::getOffset() const {
	return offset_;
}

uint32_t PushConstant::getSize() const {
	return size_;
}

vk::PushConstantRange PushConstant::getVkHandle() {
	return vk::PushConstantRange(stages_, offset_, size_);
}

#pragma endregion

#pragma region VertexAttribute

VertexAttribute::VertexAttribute(std::string name, uint32_t location, uint32_t binding, vk::Format format, uint32_t offset, uint32_t stride, vk::VertexInputRate input_rate)
	: name_(name), location_(location), binding_(binding), format_(format), offset_(offset), stride_(stride), input_rate_(input_rate) {
}

vk::VertexInputBindingDescription VertexAttribute::getBindingDescription() {
	return vk::VertexInputBindingDescription(binding_, stride_, input_rate_);
}

vk::VertexInputAttributeDescription VertexAttribute::getAttributeDescription() {
	return vk::VertexInputAttributeDescription(location_, binding_, format_, offset_);
}

const std::string & VertexAttribute::getName() const {
	return name_;
}

uint32_t VertexAttribute::getLocation() const {
	return location_;
}

uint32_t VertexAttribute::getBinding() const {
	return binding_;
}

vk::Format VertexAttribute::getFormat() const {
	return format_;
}

uint32_t VertexAttribute::getOffset() const {
	return offset_;
}

uint32_t VertexAttribute::getStride() const {
	return stride_;
}

vk::VertexInputRate VertexAttribute::getInputRate() const {
	return input_rate_;
}

#pragma endregion

#pragma region PipelineLayout

PipelineLayout::PipelineLayout(const std::string& name, const std::vector<ShaderData*>& shaders) : name_(name), shaders_(shaders), descriptor_sets_(), push_constants_(), combined_descriptors_count_(), attributes_(), device_to_layout() {
	// Sort the shaders in the same order as they come in the pipeline.
	std::sort(shaders_.begin(), shaders_.end(), [](ShaderData* a, ShaderData* b) { return a->getStage() < b->getStage(); });

	// Validate
	vk::ShaderStageFlags stages{};
	for (auto it = shaders.begin(); it != shaders.end(); it++) {
		if (stages & (*it)->getStage()) {
			throw std::runtime_error("Multiple shaders with same stage.");
		}

		stages |= (*it)->getStage();
	}

	// Determine pipeline type.
	if (stages & vk::ShaderStageFlagBits::eCompute) {
		if (shaders.size() > 1) {
			throw std::runtime_error("Invalid shader configuration.");
		}

		type_ = PipelineType::COMPUTE;
	}
	else if ((stages & vk::ShaderStageFlagBits::eVertex) && (stages & vk::ShaderStageFlagBits::eFragment)) {
		type_ = PipelineType::GRAPHICAL;
	}
	else {
		throw std::runtime_error("Invalid shader configuration.");
	}

	// Perform shader reflection.
	for (auto it = shaders_.begin(); it != shaders_.end(); it++) {
		shaderReflection(*it);
	}

	// Update descriptor count.
	for (auto it = descriptor_sets_.begin(); it != descriptor_sets_.end(); it++) {
		combined_descriptors_count_ += it->getDescriptorsCount();
	}
}

const std::string& PipelineLayout::getName() {
	return name_;
}

PipelineType PipelineLayout::getPipelineType() {
	return type_;
}

std::vector<size_t> PipelineLayout::getShaderResourceIds(VulkanDevice* device) {
	std::vector<size_t> shader_res_ids{};
	shader_res_ids.reserve(shaders_.size());

	// Retrieve shader indices.
	for (ShaderData* shader : shaders_) {
		shader_res_ids.emplace_back(shader->getShaderResourceId(device));
	}

	return shader_res_ids;
}

std::vector<vk::PipelineShaderStageCreateInfo> PipelineLayout::getVkShaderHandles(VulkanDevice* device) {
	std::vector<vk::PipelineShaderStageCreateInfo> vk_shaders{};
	vk_shaders.reserve(shaders_.size());

	// Build or retrieve cached shader modules.
	for (ShaderData* shader : shaders_) {
		vk_shaders.emplace_back(shader->getVkHandle(device));
	}

	return vk_shaders;
}

layout_id_t PipelineLayout::getLayoutResourceId(VulkanDevice* device) {
	PipelineResources* resources = device->getPipelineResources();
	auto it = device_to_layout.find(device);

	// Resource already exists. Return its index.
	if (it != device_to_layout.end()) {
		return it->second;
	}

	// Create the resource.
	layout_id_t id{};
	std::tie(id, std::ignore) = buildVkPipelineLayout(device);
	return id;
}

vk::PipelineLayout PipelineLayout::getVkHandle(VulkanDevice* device) {
	PipelineResources* resources = device->getPipelineResources();
	auto it = device_to_layout.find(device);
	
	// If the pipeline was already built, return it.
	if (it != device_to_layout.end()) {
		return resources->getPipelineLayout(it->second);
	}

	vk::PipelineLayout vk_layout{};
	std::tie(std::ignore, vk_layout) = buildVkPipelineLayout(device);
	return vk_layout;
}

const DescriptorsCount& PipelineLayout::getDescriptorsCount() {
	return combined_descriptors_count_;
}

void vkr::PipelineLayout::shaderReflection(const ShaderData* shader) {
	spirv_cross::Compiler comp(shader->getCode());

	// The SPIR-V is now parsed, and we can perform reflection on it.
	spirv_cross::ShaderResources resources = comp.get_shader_resources();
	vk::ShaderStageFlagBits stage = shader->getStage();

#pragma region VertexAttributes
	// Parse attributes if it's a vertex shader.
	if (stage == vk::ShaderStageFlagBits::eVertex) {

		// Parse attribute configuration.
		for (auto& resource : resources.stage_inputs) {
			uint32_t binding = comp.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t location = comp.get_decoration(resource.id, spv::DecorationLocation);
			auto resource_type = comp.get_type_from_variable(resource.id);
			uint32_t offset = 0;
			vk::Format format = getVertexBufferFormat(resource_type);
			uint32_t stride = (resource_type.width * resource_type.vecsize * resource_type.columns) / 8; // Size of the single element in bytes.
			vk::VertexInputRate input_rate = vk::VertexInputRate::eVertex;

			// Add attribute description
			attributes_.emplace_back(VertexAttribute(resource.name, location, binding, format, offset, stride, input_rate));
		}
	}
#pragma endregion

#pragma region DescriptorBindings
	// Helper lambda used to add new DescriptorSetLayoutMeta entry to the shader meta.
	auto addDescriptorMeta = [this, &comp, &stage](const spirv_cross::Resource& resource, vk::DescriptorType type) {
		auto resource_type = comp.get_type_from_variable(resource.id);

		// Parse data.
		size_t set = static_cast<size_t>(comp.get_decoration(resource.id, spv::DecorationDescriptorSet));
		uint32_t binding = comp.get_decoration(resource.id, spv::DecorationBinding);
		uint32_t count = std::accumulate(resource_type.array.begin(), resource_type.array.end(), 1, std::multiplies<uint32_t>());

		// If the set does not yet exist add a new one.
		if (descriptor_sets_.size() <= set) {
			descriptor_sets_.resize(set + 1);
		}
		auto test = comp.get_storage_class(resource.id);

		descriptor_sets_[set].addDescriptorBinding(DescriptorBindingLayout(resource.name, binding, type, count, stage));
	};

	// Look at: http://vulkan-spec-chunked.ahcox.com/ch13s01.html
	
	// Add uniform buffers meta.
	for (auto& resource : resources.uniform_buffers) {
		addDescriptorMeta(resource, vk::DescriptorType::eUniformBuffer);
	}

	// Add storage buffers meta.
	for (auto& resource : resources.storage_buffers) {
		addDescriptorMeta(resource, vk::DescriptorType::eStorageBuffer);
	}

	// Add storage images meta.
	for (auto& resource : resources.storage_images) {
		if (comp.get_type_from_variable(resource.id).image.dim == spv::DimBuffer) {
			// If it's a buffer it is a storage texel buffer not storage image.
			addDescriptorMeta(resource, vk::DescriptorType::eStorageTexelBuffer);
		}
		else {
			addDescriptorMeta(resource, vk::DescriptorType::eStorageImage);
		}
	}

	// Add sampled images (separate) meta.
	for (auto& resource : resources.separate_images) {
		addDescriptorMeta(resource, vk::DescriptorType::eSampledImage);
	}

	// Add sampled images (combined with sampler) meta.
	for (auto& resource : resources.sampled_images) {
		// If it's a buffer it is a uniform texel buffer not combined image sampler.
		if (comp.get_type_from_variable(resource.id).image.dim == spv::DimBuffer) {
			addDescriptorMeta(resource, vk::DescriptorType::eUniformTexelBuffer);
		}
		else {
			addDescriptorMeta(resource, vk::DescriptorType::eCombinedImageSampler);
		}
	}

	// Add sampler meta (used in conjunction with a sampled image (separate).
	for (auto& resource : resources.separate_samplers) {
		addDescriptorMeta(resource, vk::DescriptorType::eSampler);
	}

	// Add subpass attachments
	for (auto& resource : resources.subpass_inputs) {
		addDescriptorMeta(resource, vk::DescriptorType::eInputAttachment);
	}

	// Add Push constants
	for (auto& resource : resources.push_constant_buffers) {
		std::string name = comp.get_name(resource.id);
		uint32_t size = comp.get_declared_struct_size(comp.get_type_from_variable(resource.id));

		addPushConstant(name, stage, size);
	}
#pragma endregion
}

void PipelineLayout::addPushConstant(const std::string& name, vk::ShaderStageFlagBits stage, uint32_t size) {
	// Check if the push constant already exists.
	for (auto it = push_constants_.begin(); it != push_constants_.end(); it++) {
		if (it->getName() == name) {
			// Validate push constant size.
			if (it->getSize() != size) {
				throw std::runtime_error("Push constant size miss match.");
			}

			// Add additional stage.
			it->addStages(stage);
			return;
		}
	}

	// Add new push constant.
	uint32_t offset = (push_constants_.empty()) ? 0 : push_constants_.back().getOffset() + push_constants_.back().getSize();
	push_constants_.emplace_back(PushConstant(name, stage, offset, size));
}

std::tuple<size_t, vk::PipelineLayout> PipelineLayout::buildVkPipelineLayout(VulkanDevice* device) {
	// Retrieve descriptor set resource ids
	std::vector<descset_id_t> vk_desc_set_ids{};
	vk_desc_set_ids.reserve(descriptor_sets_.size());

	for (DescriptorSet& desc_set : descriptor_sets_) {
		vk_desc_set_ids.emplace_back(desc_set.getResourceId(device));
	}

	// Build vulkan push constants.
	std::vector<vk::PushConstantRange> vk_push_constants{};
	vk_push_constants.reserve(push_constants_.size());

	for (PushConstant& push_constant : push_constants_) {
		vk_push_constants.emplace_back(push_constant.getVkHandle());
	}

	// Create new resource and cache it.
	std::tuple<size_t, vk::PipelineLayout> layout_res = device->getPipelineResources()->createPipelineLayout(vk_desc_set_ids, vk_push_constants);
	device_to_layout[device] = std::get<0>(layout_res);

	return layout_res;
}

#pragma endregion

} /// namespace vkr
