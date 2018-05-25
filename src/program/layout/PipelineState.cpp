#include "program/layout/PipelineState.h"

namespace vkr {

#pragma region InputAssemblyState

InputAssemblyState::InputAssemblyState(vk::PrimitiveTopology topology, bool primitive_restart_enable) : topology(topology), primitive_restart_enable(primitive_restart_enable), vk_create_info_() {
}


bool InputAssemblyState::operator==(const InputAssemblyState& other) const {
	return other.topology == topology &&
		other.primitive_restart_enable == primitive_restart_enable;
}

const vk::PipelineInputAssemblyStateCreateInfo* InputAssemblyState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineInputAssemblyStateCreateInfo(vk::PipelineInputAssemblyStateCreateFlags(), topology, primitive_restart_enable);

	return &vk_create_info_;
}

#pragma endregion

#pragma region TesselationDomainOriginState

TessellationDomainOriginState::TessellationDomainOriginState(vk::TessellationDomainOrigin domain_origin) : domain_origin(domain_origin) {}

bool TessellationDomainOriginState::operator==(const TessellationDomainOriginState& other) const {
	return domain_origin == other.domain_origin;
}

vk::PipelineTessellationDomainOriginStateCreateInfo* TessellationDomainOriginState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineTessellationDomainOriginStateCreateInfo(domain_origin);

	return &vk_create_info_;
}

#pragma endregion

#pragma region TessellationState

TessellationState::TessellationState(uint32_t patch_control_points, const std::optional<TessellationDomainOriginState>& domain_origin) : patch_control_points(patch_control_points), domain_origin(domain_origin), vk_create_info_() {
}

bool TessellationState::operator==(const TessellationState& other) const {
	return patch_control_points == other.patch_control_points && domain_origin == other.domain_origin;
}

const vk::PipelineTessellationStateCreateInfo* TessellationState::buildCreateInfo() {

	vk_create_info_ = vk::PipelineTessellationStateCreateInfo(vk::PipelineTessellationStateCreateFlags(), patch_control_points);

	// Add domain origin extension state.
	if (domain_origin.has_value()) {
		vk_create_info_.pNext = static_cast<void*>(domain_origin.value().buildCreateInfo());
	}

	return &vk_create_info_;
}

#pragma endregion

#pragma region NvidiaViewportSwizzleState
NvidiaViewportSwizzleState::NvidiaViewportSwizzleState(const std::vector<vk::ViewportSwizzleNV>& swizzle_states) : swizzle_states(swizzle_states) {}

bool NvidiaViewportSwizzleState::operator==(const NvidiaViewportSwizzleState& other) const {
	return swizzle_states == other.swizzle_states;
}

vk::PipelineViewportSwizzleStateCreateInfoNV* NvidiaViewportSwizzleState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineViewportSwizzleStateCreateInfoNV(vk::PipelineViewportSwizzleStateCreateFlagsNV(), swizzle_states.size(), (swizzle_states.empty()) ? nullptr : &swizzle_states[0]);

	return &vk_create_info_;
}
#pragma endregion

#pragma region NvidiaViewportWScalingState

NvidiaViewportWScalingState::NvidiaViewportWScalingState(bool enable_w_scaling, const std::vector<vk::ViewportWScalingNV>& w_scalings) : enable_w_scaling(enable_w_scaling), w_scalings(w_scalings), vk_create_info_() {}

bool NvidiaViewportWScalingState::operator==(const NvidiaViewportWScalingState& other) const {
	return other.enable_w_scaling == enable_w_scaling &&
		other.w_scalings == w_scalings;
}

vk::PipelineViewportWScalingStateCreateInfoNV* NvidiaViewportWScalingState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineViewportWScalingStateCreateInfoNV(enable_w_scaling, w_scalings.size(), (w_scalings.empty()) ? nullptr : &w_scalings[0]);
	
	return &vk_create_info_;
}

#pragma endregion

#pragma region ViewportState

ViewportState::ViewportState(const std::vector<vk::Viewport>& viewports, const std::vector<vk::Rect2D>& scissors, const std::optional<NvidiaViewportSwizzleState>& swizzle_state, const std::optional<NvidiaViewportWScalingState>& w_scaling_state)
	: viewports(viewports), scissors(scissors), swizzle_state(swizzle_state), w_scaling_state(w_scaling_state), vk_create_info_() {}

bool ViewportState::operator==(const ViewportState & other) const {
	return viewports == other.viewports && scissors == other.scissors 
		&& swizzle_state == other.swizzle_state && w_scaling_state == other.w_scaling_state;
}

vk::PipelineViewportStateCreateInfo* ViewportState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineViewportStateCreateInfo(vk::PipelineViewportStateCreateFlags(), viewports.size(), (viewports.empty()) ? nullptr : &viewports[0], scissors.size(), (scissors.empty()) ? nullptr : &scissors[0]);
	const void** next = &vk_create_info_.pNext;

	// Extension Nvidia viewport swizzle.
	if (swizzle_state.has_value()) {
		vk::PipelineViewportSwizzleStateCreateInfoNV* swizzle_p = swizzle_state.value().buildCreateInfo();
		*next = static_cast<const void*>(swizzle_p);
		next = &swizzle_p->pNext;
	}

	// Extension Nvidia viewport w scaling.
	if (w_scaling_state.has_value()) {
		vk::PipelineViewportWScalingStateCreateInfoNV* w_scaling_state_p = w_scaling_state.value().buildCreateInfo();
		*next = static_cast<const void*>(w_scaling_state_p);
		next = &w_scaling_state_p->pNext;
	}

	return &vk_create_info_;
}

#pragma endregion

#pragma region ExtRasterizationConservativeState

ExtRasterizationConservativeState::ExtRasterizationConservativeState(vk::ConservativeRasterizationModeEXT conservative_mode, float extra_primitive_overestimation_size) 
	: conservative_mode(conservative_mode), extra_primitive_overestimation_size(extra_primitive_overestimation_size), vk_create_info_() {}

bool ExtRasterizationConservativeState::operator==(const ExtRasterizationConservativeState& other) const {
	return conservative_mode == other.conservative_mode &&
		extra_primitive_overestimation_size == other.extra_primitive_overestimation_size;
}

vk::PipelineRasterizationConservativeStateCreateInfoEXT* ExtRasterizationConservativeState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineRasterizationConservativeStateCreateInfoEXT(vk::PipelineRasterizationConservativeStateCreateFlagsEXT(), conservative_mode, extra_primitive_overestimation_size);

	return &vk_create_info_;
}

#pragma endregion

#pragma region AMDRasterizationOrder

AMDRasterizationOrder::AMDRasterizationOrder(vk::RasterizationOrderAMD rasterization_order) : rasterization_order(rasterization_order), vk_create_info_() {}

bool AMDRasterizationOrder::operator==(const AMDRasterizationOrder& other) const {
	return rasterization_order == other.rasterization_order;
}

vk::PipelineRasterizationStateRasterizationOrderAMD* AMDRasterizationOrder::buildCreateInfo() {
	vk_create_info_ = vk::PipelineRasterizationStateRasterizationOrderAMD(rasterization_order);

	return &vk_create_info_;
}

#pragma endregion

#pragma region RasterizationState

RasterizationState::RasterizationState(bool depth_clamp_enable, bool rasterizer_discard_enable, vk::PolygonMode polygon_mode, vk::CullModeFlags cull_mode, 
									   vk::FrontFace front_face, bool depth_bias_enable, float depth_bias_constant_factor, float depth_bias_clamp, float depth_bias_slope_factor, 
									   float line_width, const std::optional<ExtRasterizationConservativeState>& conservative_state, const std::optional<AMDRasterizationOrder>& rasterization_order)
										: depth_clamp_enable(depth_clamp_enable), rasterizer_discard_enable(rasterizer_discard_enable), polygon_mode(polygon_mode), cull_mode(cull_mode), front_face(front_face), depth_bias_enable(depth_bias_enable),
										depth_bias_constant_factor(depth_bias_constant_factor), depth_bias_clamp(depth_bias_clamp), depth_bias_slope_factor(depth_bias_slope_factor), line_width(line_width), conservative_state(conservative_state), rasterization_order(rasterization_order), vk_create_info_() {}

bool RasterizationState::operator==(const RasterizationState& other) const {
	return depth_clamp_enable == other.depth_clamp_enable && rasterizer_discard_enable == other.rasterizer_discard_enable && polygon_mode == other.polygon_mode
		&& cull_mode == other.cull_mode && front_face == other.front_face && depth_bias_enable == other.depth_bias_enable && depth_bias_constant_factor == other.depth_bias_constant_factor
		&& depth_bias_clamp == other.depth_bias_clamp && depth_bias_slope_factor == other.depth_bias_slope_factor && conservative_state == other.conservative_state
		&& rasterization_order == other.rasterization_order;
}

vk::PipelineRasterizationStateCreateInfo* RasterizationState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineRasterizationStateCreateInfo(vk::PipelineRasterizationStateCreateFlags(), depth_clamp_enable, rasterizer_discard_enable, polygon_mode, cull_mode, front_face, depth_bias_enable, depth_bias_constant_factor, depth_bias_clamp, depth_bias_slope_factor, line_width);

	const void** next = &vk_create_info_.pNext;

	// Extension Nvidia viewport swizzle.
	if (conservative_state.has_value()) {
		vk::PipelineRasterizationConservativeStateCreateInfoEXT* conservative_state_p = conservative_state.value().buildCreateInfo();
		*next = static_cast<const void*>(conservative_state_p);
		next = &conservative_state_p->pNext;
	}

	// Extension Nvidia viewport w scaling.
	if (rasterization_order.has_value()) {
		vk::PipelineRasterizationStateRasterizationOrderAMD* rasterization_order_p = rasterization_order.value().buildCreateInfo();
		*next = static_cast<const void*>(rasterization_order_p);
		next = &rasterization_order_p->pNext;
	}

	return &vk_create_info_;
}

#pragma endregion

#pragma region NvidiaCoverageModulationState

NvidiaCoverageModulationState::NvidiaCoverageModulationState(vk::CoverageModulationModeNV modulation_mode, bool enable_modulation_table, const std::vector<float>& modulation_table)
	: modulation_mode(modulation_mode), enable_modulation_table(enable_modulation_table), modulation_table(modulation_table), vk_create_info_() {}

bool NvidiaCoverageModulationState::operator==(const NvidiaCoverageModulationState& other) const {
	return modulation_mode == other.modulation_mode && enable_modulation_table == other.enable_modulation_table && modulation_table == other.modulation_table;
}

vk::PipelineCoverageModulationStateCreateInfoNV* NvidiaCoverageModulationState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineCoverageModulationStateCreateInfoNV(vk::PipelineCoverageModulationStateCreateFlagsNV(), modulation_mode, enable_modulation_table, modulation_table.size(), (modulation_table.empty()) ? nullptr : &modulation_table[0]);

	return &vk_create_info_;
}

#pragma endregion

#pragma region NvidiaCoverageToColor

NvidiaCoverageToColorState::NvidiaCoverageToColorState(bool enable, uint32_t location) : enable(enable), location(location), vk_create_info_() {}

bool NvidiaCoverageToColorState::operator==(const NvidiaCoverageToColorState& other) const {
	return enable == other.enable && location == other.location;
}

vk::PipelineCoverageToColorStateCreateInfoNV* NvidiaCoverageToColorState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineCoverageToColorStateCreateInfoNV(vk::PipelineCoverageToColorStateCreateFlagsNV(), enable, location);

	return &vk_create_info_;
}

#pragma endregion

#pragma region ExtSampleLocationsState

ExtSampleLocationsState::ExtSampleLocationsState(bool enable, const vk::SampleLocationsInfoEXT& sample_locations_info) : enable(enable), sample_locations_info(sample_locations_info), vk_create_info_() {}

bool ExtSampleLocationsState::operator==(const ExtSampleLocationsState& other) const {
	return enable == other.enable && sample_locations_info == other.sample_locations_info;
}

vk::PipelineSampleLocationsStateCreateInfoEXT* ExtSampleLocationsState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineSampleLocationsStateCreateInfoEXT(enable, sample_locations_info);

	return &vk_create_info_;
}

#pragma endregion

#pragma region MultisampleState

MultisampleState::MultisampleState(vk::SampleCountFlagBits rasterization_samples, bool enable_sample_shading, float min_sample_shading, const vk::SampleMask& sample_mask, bool enable_alpha_to_coverage, bool enable_alpha_to_one, const std::optional<NvidiaCoverageModulationState>& coverage_modulation_state, const std::optional<NvidiaCoverageToColorState>& coverage_to_color_state, const std::optional<ExtSampleLocationsState>& sample_locations_state)
	: rasterization_samples(rasterization_samples), enable_sample_shading(enable_sample_shading), min_sample_shading(min_sample_shading), sample_mask(sample_mask), enable_alpha_to_coverage(enable_alpha_to_coverage), enable_alpha_to_one(enable_alpha_to_one), coverage_modulation_state(coverage_modulation_state), coverage_to_color_state(coverage_to_color_state), sample_locations_state(sample_locations_state), vk_create_info_() {}

bool MultisampleState::operator==(const MultisampleState& other) const {
	return rasterization_samples == other.rasterization_samples && enable_sample_shading == other.enable_sample_shading && min_sample_shading == other.min_sample_shading
		&& sample_mask == other.sample_mask && enable_alpha_to_coverage == other.enable_alpha_to_coverage && enable_alpha_to_one == other.enable_alpha_to_one
		&& coverage_modulation_state == other.coverage_modulation_state && coverage_to_color_state == other.coverage_to_color_state && sample_locations_state == other.sample_locations_state;
}

vk::PipelineMultisampleStateCreateInfo* MultisampleState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineMultisampleStateCreateInfo(vk::PipelineMultisampleStateCreateFlags(), rasterization_samples, enable_sample_shading, min_sample_shading, &sample_mask, enable_alpha_to_coverage, enable_alpha_to_one);
	const void** next = &vk_create_info_.pNext;

	// Extension Nvidia coverage modulation.
	if (coverage_modulation_state.has_value()) {
		vk::PipelineCoverageModulationStateCreateInfoNV* coverage_modulation_state_p = coverage_modulation_state.value().buildCreateInfo();
		*next = static_cast<const void*>(coverage_modulation_state_p);
		next = &coverage_modulation_state_p->pNext;
	}

	// Extension Nvidia coverage to color.
	if (coverage_to_color_state.has_value()) {
		vk::PipelineCoverageToColorStateCreateInfoNV* coverage_to_color_state_p = coverage_to_color_state.value().buildCreateInfo();
		*next = static_cast<const void*>(coverage_to_color_state_p);
		next = &coverage_to_color_state_p->pNext;
	}

	// Extension sample locations state.
	if (sample_locations_state.has_value()) {
		vk::PipelineSampleLocationsStateCreateInfoEXT* sample_locations_state_p = sample_locations_state.value().buildCreateInfo();
		*next = static_cast<const void*>(sample_locations_state_p);
		next = &sample_locations_state_p->pNext;
	}

	return &vk_create_info_;
}

#pragma endregion

#pragma region DepthStencilState

DepthStencilState::DepthStencilState(bool enable_depth_test, bool enable_depth_write, vk::CompareOp depth_compare_op, bool enable_depth_bounds_test, bool enable_stencil_test, const vk::StencilOpState& front, const vk::StencilOpState& back, float min_depth_bounds, float max_depth_bounds)
	: enable_depth_test(enable_depth_test), enable_depth_write(enable_depth_write), depth_compare_op(depth_compare_op), enable_depth_bounds_test(enable_depth_bounds_test), enable_stencil_test(enable_stencil_test), front(front), back(back), min_depth_bounds(min_depth_bounds), max_depth_bounds(max_depth_bounds), vk_create_info_() {}

bool DepthStencilState::operator==(const DepthStencilState& other) const {
	return enable_depth_test == other.enable_depth_test && enable_depth_write == other.enable_depth_write 
		&& depth_compare_op == other.depth_compare_op && enable_depth_bounds_test == other.enable_depth_bounds_test
		&& enable_stencil_test == other.enable_stencil_test && front == other.front && back == other.back
		&& min_depth_bounds == other.min_depth_bounds && max_depth_bounds == other.max_depth_bounds;
}

vk::PipelineDepthStencilStateCreateInfo* DepthStencilState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineDepthStencilStateCreateInfo(vk::PipelineDepthStencilStateCreateFlags(), enable_depth_test, enable_depth_write, depth_compare_op, enable_depth_bounds_test, enable_stencil_test, front, back, min_depth_bounds, max_depth_bounds);

	return &vk_create_info_;
}

#pragma endregion

#pragma region ColorBlendAdvancedState

ExtColorBlendAdvancedState::ExtColorBlendAdvancedState(bool src_premultiplied, bool dst_premultiplied, vk::BlendOverlapEXT blend_overlap)
	: src_premultiplied(src_premultiplied), dst_premultiplied(dst_premultiplied), blend_overlap(blend_overlap), vk_create_info_() {}

bool ExtColorBlendAdvancedState::operator==(const ExtColorBlendAdvancedState& other) const {
	return src_premultiplied == other.src_premultiplied && dst_premultiplied == other.dst_premultiplied && blend_overlap == other.blend_overlap;
}

vk::PipelineColorBlendAdvancedStateCreateInfoEXT* ExtColorBlendAdvancedState::buildCreateInfo() {
	vk_create_info_ = vk::PipelineColorBlendAdvancedStateCreateInfoEXT(src_premultiplied, dst_premultiplied, blend_overlap);

	return &vk_create_info_;
}

#pragma endregion

#pragma region ColorBlendState

ColorBlendState::ColorBlendState(bool enable_logic_op, vk::LogicOp logic_op, const std::vector<vk::PipelineColorBlendAttachmentState>& attachment_states, const std::array<float, 4>& blend_constants, const std::optional<ExtColorBlendAdvancedState>& advanced_state)
	: enable_logic_op(enable_logic_op), logic_op(logic_op), attachment_states(attachment_states), blend_constants(blend_constants), advanced_state(advanced_state), vk_create_info_() {}

bool ColorBlendState::operator==(const ColorBlendState& other) const {
	return enable_logic_op == other.enable_logic_op && logic_op == other.logic_op && attachment_states == other.attachment_states && blend_constants == other.blend_constants;
}

vk::PipelineColorBlendStateCreateInfo * ColorBlendState::buildCreateInfo() {
	return nullptr;
}

#pragma endregion

#pragma region DynamicState

DynamicState::DynamicState(bool viewport, bool scissors, bool line_width, bool depth_bias, bool blend_constants, bool depth_bounds, bool stencil_compare_mask, bool stencil_write_mask, bool stencil_reference, bool nvidia_viewport_w_scaling, bool ext_discard_rectangle, bool ext_sample_locations)
	: viewport(viewport), scissors(scissors), line_width(line_width), depth_bias(depth_bias), blend_constants(blend_constants), depth_bounds(depth_bounds),
	stencil_compare_mask(stencil_compare_mask), stencil_write_mask(stencil_write_mask), stencil_reference(stencil_reference), nvidia_viewport_w_scaling(nvidia_viewport_w_scaling), 
	ext_discard_rectangle(ext_discard_rectangle), ext_sample_locations(ext_sample_locations), dynamic_states_(), vk_create_info_() {}

bool DynamicState::operator==(const DynamicState & other) const {
	return viewport == other.viewport && scissors == other.scissors && line_width == other.line_width && depth_bias == other.depth_bias && blend_constants == other.blend_constants
		&& depth_bounds == other.depth_bounds && stencil_compare_mask == other.stencil_compare_mask && stencil_write_mask == other.stencil_write_mask && stencil_reference == other.stencil_reference
		&& nvidia_viewport_w_scaling == other.nvidia_viewport_w_scaling && ext_discard_rectangle == other.ext_discard_rectangle && ext_sample_locations == other.ext_sample_locations;
}

vk::PipelineDynamicStateCreateInfo* DynamicState::buildCreateInfo() {
	dynamic_states_.clear();

	if (viewport) { dynamic_states_.emplace_back(vk::DynamicState::eViewport); }
	if (scissors) { dynamic_states_.emplace_back(vk::DynamicState::eScissor); }
	if (line_width) { dynamic_states_.emplace_back(vk::DynamicState::eLineWidth); }
	if (depth_bias) { dynamic_states_.emplace_back(vk::DynamicState::eDepthBias); }
	if (blend_constants) { dynamic_states_.emplace_back(vk::DynamicState::eBlendConstants); }
	if (depth_bounds) { dynamic_states_.emplace_back(vk::DynamicState::eDepthBounds); }
	if (stencil_compare_mask) { dynamic_states_.emplace_back(vk::DynamicState::eStencilCompareMask); }
	if (stencil_write_mask) { dynamic_states_.emplace_back(vk::DynamicState::eStencilWriteMask); }
	if (stencil_reference) { dynamic_states_.emplace_back(vk::DynamicState::eStencilReference); }
	if (nvidia_viewport_w_scaling) { dynamic_states_.emplace_back(vk::DynamicState::eViewportWScalingNV); }
	if (ext_discard_rectangle) { dynamic_states_.emplace_back(vk::DynamicState::eDiscardRectangleEXT); }
	if (ext_sample_locations) { dynamic_states_.emplace_back(vk::DynamicState::eSampleLocationsEXT); }

	vk_create_info_ = vk::PipelineDynamicStateCreateInfo(vk::PipelineDynamicStateCreateFlags(), dynamic_states_.size(), (dynamic_states_.empty()) ? nullptr : &dynamic_states_[0]);
	return &vk_create_info_;
}

#pragma endregion

PipelineState::PipelineState(InputAssemblyState input_assembly, TessellationState tessellation, ViewportState viewport, RasterizationState rasterization, MultisampleState multisample, DepthStencilState depth_stencil, ColorBlendState color_blend, DynamicState dynamic_states) 
	: input_assembly(input_assembly), tessellation(tessellation), viewport(viewport), rasterization(rasterization), multisample(multisample), depth_stencil(depth_stencil), color_blend(color_blend), dynamic_states(dynamic_states) {}

bool PipelineState::operator==(const PipelineState& other) const {
	return input_assembly == other.input_assembly && tessellation == other.tessellation && viewport == other.viewport && rasterization == other.rasterization
		&& multisample == other.multisample && depth_stencil == other.depth_stencil && color_blend == other.color_blend && dynamic_states == other.dynamic_states;
}

} ///!	namespace vkr