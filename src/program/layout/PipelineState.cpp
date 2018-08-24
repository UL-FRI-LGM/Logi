#include <utility>
#include "program/layout/PipelineState.h"

namespace logi {

#pragma region InputAssemblyState

InputAssemblyState::InputAssemblyState(const vk::PrimitiveTopology topology, const bool primitive_restart_enable,
                                       const vk::PipelineInputAssemblyStateCreateFlags& flags)
    : topology(topology), primitive_restart_enable(primitive_restart_enable), flags(flags) { }


bool InputAssemblyState::operator==(const InputAssemblyState& rhs) const {
    return rhs.topology == topology && rhs.primitive_restart_enable == primitive_restart_enable;
}

const vk::PipelineInputAssemblyStateCreateInfo* InputAssemblyState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineInputAssemblyStateCreateInfo(flags, topology, primitive_restart_enable);
    vk_create_info_.pNext = buildExtensions();

    return &vk_create_info_;
}

#pragma endregion

#pragma region TesselationDomainOriginState

TessellationDomainOriginState::TessellationDomainOriginState(const vk::TessellationDomainOrigin domain_origin)
    : domain_origin(domain_origin) {}

std::unique_ptr<ExtensionObject> TessellationDomainOriginState::clone() const {
    return std::make_unique<TessellationDomainOriginState>(domain_origin);
}

bool TessellationDomainOriginState::operator==(const TessellationDomainOriginState& rhs) const {
    return domain_origin == rhs.domain_origin;
}

vk::PipelineTessellationDomainOriginStateCreateInfo* TessellationDomainOriginState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineTessellationDomainOriginStateCreateInfo(domain_origin);

    return &vk_create_info_;
}

void* TessellationDomainOriginState::build(void* next) {
    vk_create_info_ = vk::PipelineTessellationDomainOriginStateCreateInfo(domain_origin);
    vk_create_info_.pNext = next;

    return &vk_create_info_;
}

#pragma endregion

#pragma region TessellationState

TessellationState::TessellationState(const uint32_t patch_control_points,
                                     const vk::PipelineTessellationStateCreateFlags& flags)
    : patch_control_points(patch_control_points), flags(flags) {}

bool TessellationState::operator==(const TessellationState& rhs) const {
    return patch_control_points == rhs.patch_control_points && ExtendableObject::operator==(rhs);
}

const vk::PipelineTessellationStateCreateInfo* TessellationState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineTessellationStateCreateInfo(flags, patch_control_points);
    vk_create_info_.pNext = buildExtensions();

    return &vk_create_info_;
}

#pragma endregion

#pragma region NvidiaViewportSwizzleState
NvidiaViewportSwizzleState::NvidiaViewportSwizzleState(std::vector<vk::ViewportSwizzleNV> swizzle_states,
                                                       const vk::PipelineViewportSwizzleStateCreateFlagsNV& flags)
    : swizzle_states(std::move(swizzle_states)), flags(flags) {}

std::unique_ptr<ExtensionObject> NvidiaViewportSwizzleState::clone() const {
    return std::make_unique<NvidiaViewportSwizzleState>(swizzle_states, flags);
}

bool NvidiaViewportSwizzleState::operator==(const NvidiaViewportSwizzleState& rhs) const {
    return swizzle_states == rhs.swizzle_states;
}

vk::PipelineViewportSwizzleStateCreateInfoNV* NvidiaViewportSwizzleState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineViewportSwizzleStateCreateInfoNV(flags, swizzle_states.size(),
                                                                   (swizzle_states.empty()) ? nullptr : &swizzle_states[0]);

    return &vk_create_info_;
}

void* NvidiaViewportSwizzleState::build(void* next) {
    vk_create_info_ = vk::PipelineViewportSwizzleStateCreateInfoNV(flags, swizzle_states.size(),
                                                                   (swizzle_states.empty()) ? nullptr : &swizzle_states[0]);
    vk_create_info_.pNext = next;

    return &vk_create_info_;
}

#pragma endregion

#pragma region NvidiaViewportWScalingState

NvidiaViewportWScalingState::NvidiaViewportWScalingState(const bool enable_w_scaling,
                                                         std::vector<vk::ViewportWScalingNV> w_scalings)
    : enable_w_scaling(enable_w_scaling), w_scalings(std::move(w_scalings)) {}

std::unique_ptr<ExtensionObject> NvidiaViewportWScalingState::clone() const {
    return std::make_unique<NvidiaViewportWScalingState>(enable_w_scaling, w_scalings);
}

bool NvidiaViewportWScalingState::operator==(const NvidiaViewportWScalingState& rhs) const {
    return rhs.enable_w_scaling == enable_w_scaling && rhs.w_scalings == w_scalings;
}

vk::PipelineViewportWScalingStateCreateInfoNV* NvidiaViewportWScalingState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineViewportWScalingStateCreateInfoNV(enable_w_scaling, w_scalings.size(),
                                                                    (w_scalings.empty()) ? nullptr : &w_scalings[0]);

    return &vk_create_info_;
}

void* NvidiaViewportWScalingState::build(void* next) {
    vk_create_info_ = vk::PipelineViewportWScalingStateCreateInfoNV(enable_w_scaling, w_scalings.size(),
                                                                    (w_scalings.empty()) ? nullptr : &w_scalings[0]);
    vk_create_info_.pNext = next;

    return &vk_create_info_;
}

#pragma endregion

#pragma region ViewportState

ViewportState::ViewportState(std::vector<vk::Viewport> viewports, std::vector<vk::Rect2D> scissors,
                             const vk::PipelineViewportStateCreateFlags& flags)
    : viewports(std::move(viewports)), scissors(std::move(scissors)), flags(flags) {}

bool ViewportState::operator==(const ViewportState& rhs) const {
    return viewports == rhs.viewports && scissors == rhs.scissors && ExtendableObject::operator==(rhs);
}

vk::PipelineViewportStateCreateInfo* ViewportState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineViewportStateCreateInfo(flags, viewports.size(),
		                                                  (viewports.empty()) ? nullptr : &viewports[0],
                                                          scissors.size(), (scissors.empty()) ? nullptr : &scissors[0]);
    vk_create_info_.pNext = buildExtensions();

    return &vk_create_info_;
}

#pragma endregion

#pragma region ExtRasterizationConservativeState

ExtRasterizationConservativeState::ExtRasterizationConservativeState(
    const vk::ConservativeRasterizationModeEXT conservative_mode, const float extra_primitive_overestimation_size,
    const vk::PipelineRasterizationConservativeStateCreateFlagsEXT& flags)
    : conservative_mode(conservative_mode), extra_primitive_overestimation_size(extra_primitive_overestimation_size),
      flags(flags) {}

std::unique_ptr<ExtensionObject> ExtRasterizationConservativeState::clone() const {
    return std::make_unique<ExtRasterizationConservativeState>(conservative_mode, extra_primitive_overestimation_size,
                                                               flags);
}

bool ExtRasterizationConservativeState::operator==(const ExtRasterizationConservativeState& rhs) const {
    return conservative_mode == rhs.conservative_mode &&
           extra_primitive_overestimation_size == rhs.extra_primitive_overestimation_size;
}

vk::PipelineRasterizationConservativeStateCreateInfoEXT* ExtRasterizationConservativeState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineRasterizationConservativeStateCreateInfoEXT(flags, conservative_mode,
                                                                              extra_primitive_overestimation_size);

    return &vk_create_info_;
}

void* ExtRasterizationConservativeState::build(void* next) {
    vk_create_info_ = vk::PipelineRasterizationConservativeStateCreateInfoEXT(flags, conservative_mode,
                                                                              extra_primitive_overestimation_size);
    vk_create_info_.pNext = next;

    return &vk_create_info_;
}

#pragma endregion

#pragma region AMDRasterizationOrder

AMDRasterizationOrder::AMDRasterizationOrder(const vk::RasterizationOrderAMD rasterization_order)
    : rasterization_order(rasterization_order) {}

std::unique_ptr<ExtensionObject> AMDRasterizationOrder::clone() const {
    return std::make_unique<AMDRasterizationOrder>(rasterization_order);
}

bool AMDRasterizationOrder::operator==(const AMDRasterizationOrder& rhs) const {
    return rasterization_order == rhs.rasterization_order;
}

vk::PipelineRasterizationStateRasterizationOrderAMD* AMDRasterizationOrder::buildCreateInfo() {
    vk_create_info_ = vk::PipelineRasterizationStateRasterizationOrderAMD(rasterization_order);

    return &vk_create_info_;
}

void* AMDRasterizationOrder::build(void* next) {
    vk_create_info_ = vk::PipelineRasterizationStateRasterizationOrderAMD(rasterization_order);
    vk_create_info_.pNext = next;

    return &vk_create_info_;
}

#pragma endregion

#pragma region RasterizationState

RasterizationState::RasterizationState(const bool depth_clamp_enable, const bool rasterizer_discard_enable,
                                       const vk::PolygonMode polygon_mode, const vk::CullModeFlags& cull_mode,
                                       const vk::FrontFace front_face, const bool depth_bias_enable,
	                                   const float depth_bias_constant_factor, const float depth_bias_clamp,
	                                   const float depth_bias_slope_factor, const float line_width,
                                       const vk::PipelineRasterizationStateCreateFlags& flags)
    : depth_clamp_enable(depth_clamp_enable), rasterizer_discard_enable(rasterizer_discard_enable),
      polygon_mode(polygon_mode), cull_mode(cull_mode), front_face(front_face), depth_bias_enable(depth_bias_enable),
      depth_bias_constant_factor(depth_bias_constant_factor), depth_bias_clamp(depth_bias_clamp),
      depth_bias_slope_factor(depth_bias_slope_factor), line_width(line_width), flags(flags) {}

bool RasterizationState::operator==(const RasterizationState& rhs) const {
    return depth_clamp_enable == rhs.depth_clamp_enable && rasterizer_discard_enable == rhs.rasterizer_discard_enable &&
        polygon_mode == rhs.polygon_mode && cull_mode == rhs.cull_mode && front_face == rhs.front_face &&
		depth_bias_enable == rhs.depth_bias_enable && depth_bias_constant_factor == rhs.depth_bias_constant_factor &&
		depth_bias_clamp == rhs.depth_bias_clamp && depth_bias_slope_factor == rhs.depth_bias_slope_factor &&
        ExtendableObject::operator==(rhs);
}

vk::PipelineRasterizationStateCreateInfo* RasterizationState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineRasterizationStateCreateInfo(flags, depth_clamp_enable, rasterizer_discard_enable,
                                                               polygon_mode, cull_mode, front_face, depth_bias_enable,
                                                               depth_bias_constant_factor, depth_bias_clamp,
                                                               depth_bias_slope_factor, line_width);
    vk_create_info_.pNext = buildExtensions();

    return &vk_create_info_;
}

#pragma endregion

#pragma region NvidiaCoverageModulationState

NvidiaCoverageModulationState::NvidiaCoverageModulationState(const vk::CoverageModulationModeNV modulation_mode,
                                                             const bool enable_modulation_table,
                                                             std::vector<float> modulation_table,
                                                             const vk::PipelineCoverageModulationStateCreateFlagsNV& flags)
    : modulation_mode(modulation_mode), enable_modulation_table(enable_modulation_table),
      modulation_table(std::move(modulation_table)), flags(flags) {}

std::unique_ptr<ExtensionObject> NvidiaCoverageModulationState::clone() const {
    return std::make_unique<NvidiaCoverageModulationState>(modulation_mode, enable_modulation_table, modulation_table,
                                                           flags);
}

bool NvidiaCoverageModulationState::operator==(const NvidiaCoverageModulationState& rhs) const {
    return modulation_mode == rhs.modulation_mode && enable_modulation_table == rhs.enable_modulation_table &&
        modulation_table == rhs.modulation_table;
}

vk::PipelineCoverageModulationStateCreateInfoNV* NvidiaCoverageModulationState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineCoverageModulationStateCreateInfoNV(flags, modulation_mode, enable_modulation_table,
                                                                      modulation_table.size(),
                                                                      (modulation_table.empty())
                                                                          ? nullptr : &modulation_table[0]);

    return &vk_create_info_;
}

void* NvidiaCoverageModulationState::build(void* next) {
    vk_create_info_ = vk::PipelineCoverageModulationStateCreateInfoNV(flags, modulation_mode, enable_modulation_table,
                                                                      modulation_table.size(),
                                                                      (modulation_table.empty())
                                                                          ? nullptr : &modulation_table[0]);
    vk_create_info_.pNext = next;

    return &vk_create_info_;
}

#pragma endregion

#pragma region NvidiaCoverageToColor

NvidiaCoverageToColorState::NvidiaCoverageToColorState(const bool enable, const uint32_t location,
                                                       const vk::PipelineCoverageToColorStateCreateFlagsNV& flags)
    : enable(enable), location(location), flags(flags) {}

std::unique_ptr<ExtensionObject> NvidiaCoverageToColorState::clone() const {
    return std::make_unique<NvidiaCoverageToColorState>(enable, location, flags);
}

bool NvidiaCoverageToColorState::operator==(const NvidiaCoverageToColorState& rhs) const {
    return enable == rhs.enable && location == rhs.location;
}

vk::PipelineCoverageToColorStateCreateInfoNV* NvidiaCoverageToColorState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineCoverageToColorStateCreateInfoNV(flags, enable, location);

    return &vk_create_info_;
}

void* NvidiaCoverageToColorState::build(void* next) {
    vk_create_info_ = vk::PipelineCoverageToColorStateCreateInfoNV(flags, enable, location);
    vk_create_info_.pNext = next;

    return &vk_create_info_;
}

#pragma endregion

#pragma region ExtSampleLocationsState

ExtSampleLocationsState::ExtSampleLocationsState(const bool enable,
                                                 const vk::SampleLocationsInfoEXT& sample_locations_info)
    : enable(enable), sample_locations_info(sample_locations_info) {}

std::unique_ptr<ExtensionObject> ExtSampleLocationsState::clone() const {
    return std::make_unique<ExtSampleLocationsState>(enable, sample_locations_info);
}

bool ExtSampleLocationsState::operator==(const ExtSampleLocationsState& rhs) const {
    return enable == rhs.enable && sample_locations_info == rhs.sample_locations_info;
}

vk::PipelineSampleLocationsStateCreateInfoEXT* ExtSampleLocationsState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineSampleLocationsStateCreateInfoEXT(enable, sample_locations_info);

    return &vk_create_info_;
}

void* ExtSampleLocationsState::build(void* next) {
    vk_create_info_ = vk::PipelineSampleLocationsStateCreateInfoEXT(enable, sample_locations_info);
    vk_create_info_.pNext = next;

    return &vk_create_info_;
}

#pragma endregion

#pragma region MultisampleState

MultisampleState::MultisampleState(const vk::SampleCountFlagBits rasterization_samples,
                                   const bool enable_sample_shading,
                                   const float min_sample_shading, const vk::SampleMask sample_mask,
                                   const bool enable_alpha_to_coverage,
                                   const bool enable_alpha_to_one, const vk::PipelineMultisampleStateCreateFlags& flags)
    : rasterization_samples(rasterization_samples), enable_sample_shading(enable_sample_shading),
      min_sample_shading(min_sample_shading), sample_mask(sample_mask),
      enable_alpha_to_coverage(enable_alpha_to_coverage),
      enable_alpha_to_one(enable_alpha_to_one), flags(flags) {}

bool MultisampleState::operator==(const MultisampleState& rhs) const {
    return rasterization_samples == rhs.rasterization_samples && enable_sample_shading == rhs.enable_sample_shading &&
        min_sample_shading == rhs.min_sample_shading && sample_mask == rhs.sample_mask &&
		enable_alpha_to_coverage == rhs.enable_alpha_to_coverage && enable_alpha_to_one == rhs.enable_alpha_to_one &&
		ExtendableObject::operator==(rhs);
}

vk::PipelineMultisampleStateCreateInfo* MultisampleState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineMultisampleStateCreateInfo(vk::PipelineMultisampleStateCreateFlags(),
                                                             rasterization_samples, enable_sample_shading,
                                                             min_sample_shading, &sample_mask, enable_alpha_to_coverage,
                                                             enable_alpha_to_one);
    vk_create_info_.pNext = buildExtensions();

    return &vk_create_info_;
}

#pragma endregion

#pragma region DepthStencilState

DepthStencilState::DepthStencilState(const bool enable_depth_test, const bool enable_depth_write,
                                     const vk::CompareOp depth_compare_op, const bool enable_depth_bounds_test,
                                     const bool enable_stencil_test, const vk::StencilOpState& front,
                                     const vk::StencilOpState& back, const float min_depth_bounds,
                                     const float max_depth_bounds, const vk::PipelineDepthStencilStateCreateFlags& flags)
    : enable_depth_test(enable_depth_test), enable_depth_write(enable_depth_write), depth_compare_op(depth_compare_op),
      enable_depth_bounds_test(enable_depth_bounds_test), enable_stencil_test(enable_stencil_test), front(front),
      back(back), min_depth_bounds(min_depth_bounds), max_depth_bounds(max_depth_bounds), flags(flags) {}

bool DepthStencilState::operator==(const DepthStencilState& rhs) const {
    return enable_depth_test == rhs.enable_depth_test && enable_depth_write == rhs.enable_depth_write
        && depth_compare_op == rhs.depth_compare_op && enable_depth_bounds_test == rhs.enable_depth_bounds_test
        && enable_stencil_test == rhs.enable_stencil_test && front == rhs.front && back == rhs.back
        && min_depth_bounds == rhs.min_depth_bounds && max_depth_bounds == rhs.max_depth_bounds
        && ExtendableObject::operator==(rhs);
}

vk::PipelineDepthStencilStateCreateInfo* DepthStencilState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineDepthStencilStateCreateInfo(flags, enable_depth_test, enable_depth_write,
                                                              depth_compare_op, enable_depth_bounds_test,
                                                              enable_stencil_test, front, back, min_depth_bounds,
                                                              max_depth_bounds);
    vk_create_info_.pNext = buildExtensions();

    return &vk_create_info_;
}

#pragma endregion

#pragma region ColorBlendAdvancedState

ExtColorBlendAdvancedState::ExtColorBlendAdvancedState(const bool src_premultiplied, const bool dst_premultiplied,
                                                       const vk::BlendOverlapEXT blend_overlap)
    : src_premultiplied(src_premultiplied), dst_premultiplied(dst_premultiplied), blend_overlap(blend_overlap) {}

std::unique_ptr<ExtensionObject> ExtColorBlendAdvancedState::clone() const {
    return std::make_unique<ExtColorBlendAdvancedState>(src_premultiplied, dst_premultiplied, blend_overlap);
}

bool ExtColorBlendAdvancedState::operator==(const ExtColorBlendAdvancedState& rhs) const {
    return src_premultiplied == rhs.src_premultiplied && dst_premultiplied == rhs.dst_premultiplied && blend_overlap ==
        rhs.blend_overlap;
}

vk::PipelineColorBlendAdvancedStateCreateInfoEXT* ExtColorBlendAdvancedState::buildCreateInfo() {
    vk_create_info_ = vk::PipelineColorBlendAdvancedStateCreateInfoEXT(src_premultiplied, dst_premultiplied,
                                                                       blend_overlap);

    return &vk_create_info_;
}

void* ExtColorBlendAdvancedState::build(void* next) {
    vk_create_info_ = vk::PipelineColorBlendAdvancedStateCreateInfoEXT(src_premultiplied, dst_premultiplied,
                                                                       blend_overlap);
    vk_create_info_.pNext = next;

    return &vk_create_info_;
}

#pragma endregion

#pragma region ColorBlendState

ColorBlendState::ColorBlendState(const bool enable_logic_op, const vk::LogicOp logic_op,
                                 std::vector<vk::PipelineColorBlendAttachmentState> attachment_states,
                                 const std::array<float, 4>& blend_constants,
                                 const vk::PipelineColorBlendStateCreateFlags& flags)
    : enable_logic_op(enable_logic_op), logic_op(logic_op), attachment_states(std::move(attachment_states)),
      blend_constants(blend_constants), flags(flags) {}

bool ColorBlendState::operator==(const ColorBlendState& rhs) const {
    return enable_logic_op == rhs.enable_logic_op && logic_op == rhs.logic_op && attachment_states == rhs.
        attachment_states && blend_constants == rhs.blend_constants && ExtendableObject::operator==(rhs);
}

vk::PipelineColorBlendStateCreateInfo* ColorBlendState::buildCreateInfo() {
    vk_create_info_.logicOpEnable = enable_logic_op;
    vk_create_info_.logicOp = logic_op;
    vk_create_info_.attachmentCount = attachment_states.size();
    vk_create_info_.pAttachments = attachment_states.data();
    std::copy(blend_constants.begin(), blend_constants.end(), vk_create_info_.blendConstants);
    vk_create_info_.pNext = buildExtensions();

    return &vk_create_info_;
}

#pragma endregion

#pragma region DynamicState

DynamicState::DynamicState(std::set<vk::DynamicState> dynamic_states, const vk::PipelineDynamicStateCreateFlags& flags)
    : flags(flags), dynamic_states_set_(std::move(dynamic_states)) {}

bool DynamicState::operator==(const DynamicState& rhs) const {
    return dynamic_states_set_ == rhs.dynamic_states_set_ && ExtendableObject::operator==(rhs);
}

vk::PipelineDynamicStateCreateInfo* DynamicState::buildCreateInfo() {
    std::copy(dynamic_states_set_.begin(), dynamic_states_set_.end(), std::back_inserter(dynamic_states_));

    vk_create_info_ = vk::PipelineDynamicStateCreateInfo(flags, dynamic_states_.size(),
                                                         (dynamic_states_.empty()) ? nullptr : &dynamic_states_[0]);
    vk_create_info_.pNext = buildExtensions();

    if (vk_create_info_.dynamicStateCount == 0u && vk_create_info_.pNext == nullptr) {
		return nullptr;
	}
	
    return &vk_create_info_;
}

void DynamicState::set(const vk::DynamicState state, const bool enable) {
    if (enable) {
        dynamic_states_set_.insert(state);
    }
    else {
        dynamic_states_set_.erase(state);
    }
}

bool DynamicState::isSet(const vk::DynamicState state) {
    return dynamic_states_set_.find(state) != dynamic_states_set_.end();
}


#pragma endregion

PipelineState::PipelineState(InputAssemblyState input_assembly, TessellationState tessellation, ViewportState viewport,
                             RasterizationState rasterization, MultisampleState multisample,
                             DepthStencilState depth_stencil, ColorBlendState color_blend, DynamicState dynamic_states)
    : input_assembly(std::move(input_assembly)), tessellation(std::move(tessellation)), viewport(std::move(viewport)),
      rasterization(std::move(rasterization)), multisample(std::move(multisample)),
      depth_stencil(std::move(depth_stencil)), color_blend(std::move(color_blend)),
      dynamic_states(std::move(dynamic_states)) {}

bool PipelineState::operator==(const PipelineState& rhs) const {
    return input_assembly == rhs.input_assembly && tessellation == rhs.tessellation && viewport == rhs.viewport &&
        rasterization == rhs.rasterization && multisample == rhs.multisample && depth_stencil == rhs.depth_stencil &&
		color_blend == rhs.color_blend && dynamic_states == rhs.dynamic_states;
}

} ///!	namespace vkr
