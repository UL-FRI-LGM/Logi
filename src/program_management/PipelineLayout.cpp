#include "program_management/PipelineLayout.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <vulkan/vulkan.hpp>
#include <spirv_cross.hpp>
#include "util/FormatConversion.h"

namespace vkr {

#pragma region DescriptorsCount
DescriptorsCount::DescriptorsCount(uint32_t num_sets, uint32_t samplers, uint32_t combined_image_samplers, uint32_t sampled_images, uint32_t storage_images, uint32_t uniform_texel_buffers,
	uint32_t storage_texel_buffers, uint32_t uniform_buffers, uint32_t storage_buffers, uint32_t uniform_buffers_dynamic, uint32_t storage_buffers_dynamic, uint32_t input_attachments)
	: num_sets(num_sets), samplers(samplers), combined_image_samplers(combined_image_samplers), sampled_images(sampled_images), storage_images(storage_images),
	uniform_texel_buffers(uniform_texel_buffers), storage_texel_buffers(storage_texel_buffers), uniform_buffers(uniform_buffers), storage_buffers(storage_buffers),
	uniform_buffers_dynamic(uniform_buffers_dynamic), storage_buffers_dynamic(storage_buffers_dynamic), input_attachments(input_attachments) {
}

DescriptorsCount DescriptorsCount::operator+(const DescriptorsCount& other) const {
	return DescriptorsCount(num_sets + other.num_sets, samplers + other.samplers, combined_image_samplers + other.combined_image_samplers, sampled_images + other.sampled_images, storage_images + other.storage_images, uniform_texel_buffers + other.uniform_texel_buffers,
		storage_texel_buffers + other.storage_texel_buffers, uniform_buffers + other.uniform_buffers, storage_buffers + other.storage_buffers, uniform_buffers_dynamic + other.uniform_buffers_dynamic, storage_buffers_dynamic + other.storage_buffers_dynamic, input_attachments + other.input_attachments);
}

DescriptorsCount& DescriptorsCount::operator+=(const DescriptorsCount& other) {
	num_sets += other.num_sets;
	samplers += other.samplers;
	combined_image_samplers += other.combined_image_samplers;
	sampled_images += other.sampled_images;
	storage_images += other.storage_images;
	uniform_texel_buffers += other.uniform_texel_buffers;
	storage_texel_buffers += other.storage_texel_buffers;
	uniform_buffers += other.uniform_buffers;
	storage_buffers += other.storage_buffers;
	uniform_buffers_dynamic += other.uniform_buffers_dynamic;
	storage_buffers_dynamic += other.storage_buffers_dynamic;
	input_attachments += other.input_attachments;

	return *this;
}

DescriptorsCount DescriptorsCount::operator*(uint32_t multipler) const {
	return DescriptorsCount(num_sets * multipler, samplers * multipler, combined_image_samplers * multipler, sampled_images * multipler, storage_images * multipler, uniform_texel_buffers * multipler,
		storage_texel_buffers * multipler, uniform_buffers * multipler, storage_buffers * multipler, uniform_buffers_dynamic * multipler, storage_buffers_dynamic * multipler, input_attachments * multipler);
}

DescriptorsCount DescriptorsCount::operator*=(uint32_t multipler) {
	num_sets *= multipler;
	samplers *= multipler;
	combined_image_samplers *= multipler;
	sampled_images *= multipler;
	storage_images *= multipler;
	uniform_texel_buffers *= multipler;
	storage_texel_buffers *= multipler;
	uniform_buffers *= multipler;
	storage_buffers *= multipler;
	uniform_buffers_dynamic *= multipler;
	storage_buffers_dynamic *= multipler;
	input_attachments *= multipler;

	return *this;
}

std::vector<vk::DescriptorPoolSize> DescriptorsCount::getVkDescriptorPoolSizes() const {
	std::vector<vk::DescriptorPoolSize> vk_pool_sizes{};

	if (samplers > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eSampler, samplers));
	}
	if (combined_image_samplers > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, combined_image_samplers));
	}
	if (sampled_images > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eSampledImage, sampled_images));
	}
	if (storage_images > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eStorageImage, storage_images));
	}
	if (uniform_texel_buffers > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eUniformTexelBuffer, uniform_texel_buffers));
	}
	if (storage_texel_buffers > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eStorageTexelBuffer, storage_texel_buffers));
	}
	if (uniform_buffers > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, uniform_buffers));
	}
	if (storage_buffers > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eStorageBuffer, storage_buffers));
	}
	if (uniform_buffers_dynamic > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eUniformBufferDynamic, uniform_buffers_dynamic));
	}
	if (storage_buffers_dynamic > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eStorageBufferDynamic, storage_buffers_dynamic));
	}
	if (input_attachments > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eInputAttachment, input_attachments));
	}

	return vk_pool_sizes;
}
#pragma endregion

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

#pragma region DescriptorSetLayout

DescriptorSetLayout::DescriptorSetLayout(const vk::Device& device) : device_(device), descriptor_set_layout_(nullptr), bindings_(), descriptors_count_(1u) {
}

void DescriptorSetLayout::addDescriptorBinding(const DescriptorBindingLayout& binding) {
	// Check if the descriptor set layout is already baked.
	if (descriptor_set_layout_) {
		throw std::runtime_error("Descriptor set layout is already baked.");
	}

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

void DescriptorSetLayout::bake() {
	// Check if the descriptor set layout is already baked.
	if (descriptor_set_layout_) {
		throw std::runtime_error("Descriptor set layout is already baked.");
	}

	// Generate bindings.
	std::vector<vk::DescriptorSetLayoutBinding> vk_bindings{};
	vk_bindings.reserve(bindings_.size());

	for (auto it = bindings_.begin(); it != bindings_.end(); it++) {
		vk_bindings.push_back(it->getVkHandle());
	}

	// Create descriptor set layout.
	vk::DescriptorSetLayoutCreateInfo dsl_ci;
	dsl_ci.bindingCount = vk_bindings.size();
	dsl_ci.pBindings = vk_bindings.data();

	descriptor_set_layout_ = device_.createDescriptorSetLayout(dsl_ci);
}

void DescriptorSetLayout::incrementDescriptorCount(const vk::DescriptorType& type) {
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

const DescriptorBindingLayout* DescriptorSetLayout::getDescriptorBinding(uint32_t binding) const {
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


const vk::DescriptorSetLayout& DescriptorSetLayout::getVkHandle() const {
	return descriptor_set_layout_;
}

const DescriptorsCount& DescriptorSetLayout::getDescriptorsCount() {
	return descriptors_count_;
}

DescriptorSetLayout::~DescriptorSetLayout() {
	if (descriptor_set_layout_) {
		device_.destroyDescriptorSetLayout(descriptor_set_layout_);
	}
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

PipelineLayout::PipelineLayout(const vk::Device& device, const std::string& name, const std::vector<ShaderData*>& shaders) : device_(device), name_(name), shaders_(shaders), descriptor_sets_(), push_constants_(), combined_descriptors_count_(), attributes_() {
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

	// Bake descriptor sets.
	for (auto& desc_set : descriptor_sets_) {
		desc_set->bake();
	}

	// Update descriptor count.
	for (auto it = descriptor_sets_.begin(); it != descriptor_sets_.end(); it++) {
		combined_descriptors_count_ += (*it)->getDescriptorsCount();
	}

	// Build Vulkan pipeline layout.
	buildVkPipelineLayout();
}

const std::string& PipelineLayout::getName() const {
	return name_;
}

PipelineType PipelineLayout::getPipelineType() const {
	return type_;
}

std::vector<vk::PipelineShaderStageCreateInfo> PipelineLayout::getVkShaderHandles() {
	std::vector<vk::PipelineShaderStageCreateInfo> vk_shaders{};
	vk_shaders.reserve(shaders_.size());

	// Build or retrieve cached shader modules.
	for (ShaderData* shader : shaders_) {
		vk_shaders.emplace_back(shader->getVkHandle());
	}

	return vk_shaders;
}

const vk::PipelineLayout& PipelineLayout::getVkHandle() const {
	return vk_pipeline_layout_;
}

const DescriptorsCount& PipelineLayout::getDescriptorsCount() const {
	return combined_descriptors_count_;
}

const DescriptorSetLayout* PipelineLayout::getDescriptorSetLayout(size_t set) const {
	return descriptor_sets_[set].get();
}

size_t PipelineLayout::getDescriptorSetCount() const {
	return descriptor_sets_.size();
}

PipelineLayout::~PipelineLayout() {
	device_.destroyPipelineLayout(vk_pipeline_layout_);
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
		while (descriptor_sets_.size() <= set) {
			descriptor_sets_.emplace_back(std::make_unique<DescriptorSetLayout>(device_));
		}
		auto test = comp.get_storage_class(resource.id);

		descriptor_sets_[set]->addDescriptorBinding(DescriptorBindingLayout(resource.name, binding, type, count, stage));
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

void PipelineLayout::buildVkPipelineLayout() {
	// Retrieve descriptor set vulkan handles
	std::vector<vk::DescriptorSetLayout> vk_desc_set_layouts{};
	vk_desc_set_layouts.reserve(descriptor_sets_.size());

	for (std::unique_ptr<DescriptorSetLayout>& desc_set : descriptor_sets_) {
		vk_desc_set_layouts.emplace_back(desc_set->getVkHandle());
	}

	// Build vulkan push constants.
	std::vector<vk::PushConstantRange> vk_push_constants{};
	vk_push_constants.reserve(push_constants_.size());

	for (PushConstant& push_constant : push_constants_) {
		vk_push_constants.emplace_back(push_constant.getVkHandle());
	}

	vk::PipelineLayoutCreateInfo layout_ci{};
	layout_ci.pSetLayouts = vk_desc_set_layouts.data();
	layout_ci.setLayoutCount = vk_desc_set_layouts.size();
	layout_ci.pPushConstantRanges = vk_push_constants.data();
	layout_ci.pushConstantRangeCount = vk_push_constants.size();

	vk_pipeline_layout_ = device_.createPipelineLayout(layout_ci);
}

#pragma endregion

} /// namespace vkr
