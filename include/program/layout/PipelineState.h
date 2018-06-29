#ifndef PROGRAM_LAYOUT_PIPELINE_STATE_H
#define PROGRAM_LAYOUT_PIPELINE_STATE_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include <array>
#include <memory>
#include <optional>

namespace logi {

/**
 * @brief	Used to build the pipeline assembly state configuration.
 */
struct InputAssemblyState {
	/**
	 * @brief	Default constructor that initializes members with either default or given values.
	 *
	 * @param	topology					(Optional) Defines primitive topology.
	 * @param	primitive_restart_enable	(Optional) If true a special vertex index value will be treated as restarting the assembly of primitives.
	 */
	InputAssemblyState(vk::PrimitiveTopology topology = vk::PrimitiveTopology::eTriangleList, bool primitive_restart_enable = false);

	/**
	 * @brief	Compares InputAssemblyStates if they are contain equivalent configuration.
	 *
	 * @param	other	Other InputAsseblyState object.
	 * @return	True if the InputAssamblyState objects contain equivalent configuration.
	 */
	bool operator==(const InputAssemblyState& other) const;

	/**
	 * @brief	Build Vulkan vk::PipelineInputAssemblyStateCreateInfo using the member values.
	 *
	 * @return	Pointer to vk::PipelineInputAssemblyStateCreateInfo structure.
	 */
	const vk::PipelineInputAssemblyStateCreateInfo* buildCreateInfo();

	vk::PrimitiveTopology	topology;					///< Defines primitive topology.
	bool					primitive_restart_enable;	///< Controls whether a special vertex index value is treated as restarting the assembly of primitives.

private:
	vk::PipelineInputAssemblyStateCreateInfo vk_create_info_;	///< Vulkan create info structure.
};


/**
 * @brief	Used to build the tessellation domain origin state configuration.
 */
struct TessellationDomainOriginState {

	/**
	 * @brief	Default constructor that initializes members with either default or given values.
	 *
	 * @param	domain_origin	The domain origin.
	 */
	TessellationDomainOriginState(vk::TessellationDomainOrigin domain_origin);

	/**
	 * @brief	Compares TessellationDomainOriginStates if they are contain equivalent configuration.
	 *
	 * @param	other	Other TessellationDomainOriginState object.
	 *
	 * @return	True if the TessellationDomainOriginState objects contain equivalent configuration.
	 */
	bool operator==(const TessellationDomainOriginState& other) const;

	/**
	 * @brief	Build Vulkan vk::PipelineTessellationDomainOriginStateCreateInfo using the member values.
	 *
	 * @return	Pointer to vk::PipelineTessellationDomainOriginStateCreateInfo structure.
	 */
	vk::PipelineTessellationDomainOriginStateCreateInfo* buildCreateInfo();

	vk::TessellationDomainOrigin domain_origin;	///< Controls the origin of the tessellation domain space.

private:
	vk::PipelineTessellationDomainOriginStateCreateInfo vk_create_info_;	///< Vulkan create info structure.
};

/**
 * @brief	Structure containing the pipeline tessellation state configuration.
 */
struct TessellationState {
	/**
	 * @brief	Default constructor that initializes members with either default or given values.
	 *
	 * @param	patch_control_points		Number of control points per patch.
	 * @param	domain_origin				Controls the origin of the tessellation domain space.
	 */
	TessellationState::TessellationState(uint32_t patch_control_points = 0, const std::optional<TessellationDomainOriginState>& domain_origin = {});

	/**
	 * @brief	Compare TessellationStates if they are contain equivalent configuration.
	 *
	 * @param	other	Other TessellationState object.
	 *
	 * @return	True if the TessellationState objects contain equivalent configuration.
	 */
	bool operator==(const TessellationState& other) const;

	/**
	 * @brief	Build Vulkan PipelineTessellationStateCreateInfo using the member values.
	 *
	 * @return	PipelineTessellationStateCreateInfo structure.
	 */
	const vk::PipelineTessellationStateCreateInfo* buildCreateInfo();

	uint32_t									patch_control_points;	///< Number of control points per patch.
	std::optional<TessellationDomainOriginState>	domain_origin;			///< Controls the origin of the tessellation domain space.

private:
	vk::PipelineTessellationStateCreateInfo vk_create_info_;	///< Vulkan create info structure.
};

struct NvidiaViewportSwizzleState {
	NvidiaViewportSwizzleState::NvidiaViewportSwizzleState(const std::vector<vk::ViewportSwizzleNV>& swizzle_states = {});

	bool operator==(const NvidiaViewportSwizzleState& other) const;

	vk::PipelineViewportSwizzleStateCreateInfoNV* buildCreateInfo();

	std::vector<vk::ViewportSwizzleNV> swizzle_states;

private:
	vk::PipelineViewportSwizzleStateCreateInfoNV vk_create_info_;	///< Vulkan create info structure.
};

struct NvidiaViewportWScalingState {

	NvidiaViewportWScalingState::NvidiaViewportWScalingState(bool enable_w_scaling = true, const std::vector<vk::ViewportWScalingNV>& w_scalings = {});
	
	bool operator==(const NvidiaViewportWScalingState& other) const;

	vk::PipelineViewportWScalingStateCreateInfoNV* buildCreateInfo();

	bool								enable_w_scaling;	///< True if w scaling should be enabled.
	std::vector<vk::ViewportWScalingNV>	w_scalings;			///< Specifies w scaling for each viewport.

private:
	vk::PipelineViewportWScalingStateCreateInfoNV vk_create_info_;	///< Vulkan create info structure.
};

/**
 * @brief	Structure containing the pipeline viewport state configuration.
 */
struct ViewportState {
	/**
	 * @brief	Default constructor that initializes members with either default or given values.
	 *
	 * @param	viewports	   	(Optional) Vector containing the viewport transforms. If the viewport state is dynamic, this member is ignored.
	 * @param	scissors	   	(Optional) Vector containing rectangular bounds of the scissor for the corresponding viewport. If the scissor state is dynamic, this member is ignored.
	 * @param	swizzle_state  	(Optional) Optional swizzle state.
	 * @param	w_scaling_state	(Optional) Optional w scaling state.
	 */
	ViewportState(const std::vector<vk::Viewport>& viewports = {}, const std::vector<vk::Rect2D>& scissors = {}, const std::optional<NvidiaViewportSwizzleState>& swizzle_state = {}, const std::optional<NvidiaViewportWScalingState>& w_scaling_state = {});

	bool operator==(const ViewportState& other) const;

	/**
	 * @brief	Build Vulkan PipelineViewportStateCreateInfo using the member values.
	 *
	 * @return	PipelineViewportStateCreateInfo structure.
	 */
	vk::PipelineViewportStateCreateInfo*	 buildCreateInfo();

	std::vector<vk::Viewport>					viewports;			///< Vector containing the viewport transforms. If the viewport state is dynamic, this member is ignored.
	std::vector<vk::Rect2D>						scissors;			///< Vector containing rectangular bounds of the scissor for the corresponding viewport. If the scissor state is dynamic, this member is ignored.
	std::optional<NvidiaViewportSwizzleState>	swizzle_state;		///< Optional swizzle state.
	std::optional<NvidiaViewportWScalingState>	w_scaling_state;	///< Optional w scaling state.

private:
	vk::PipelineViewportStateCreateInfo			vk_create_info_;	///< Vulkan create info structure.
};

struct ExtRasterizationConservativeState {
	ExtRasterizationConservativeState(vk::ConservativeRasterizationModeEXT conservative_mode = vk::ConservativeRasterizationModeEXT::eDisabled, float extra_primitive_overestimation_size = 0);

	bool operator==(const ExtRasterizationConservativeState& other) const;

	vk::PipelineRasterizationConservativeStateCreateInfoEXT* buildCreateInfo();

	vk::ConservativeRasterizationModeEXT conservative_mode;
	float extra_primitive_overestimation_size;
private:
	vk::PipelineRasterizationConservativeStateCreateInfoEXT vk_create_info_;
};

struct AMDRasterizationOrder {
	AMDRasterizationOrder(vk::RasterizationOrderAMD rasterization_order = vk::RasterizationOrderAMD::eStrict);

	bool operator==(const AMDRasterizationOrder& other) const;

	vk::PipelineRasterizationStateRasterizationOrderAMD* buildCreateInfo();

	vk::RasterizationOrderAMD rasterization_order;
private:
	vk::PipelineRasterizationStateRasterizationOrderAMD vk_create_info_;
};

struct RasterizationState {
	
	RasterizationState(bool depth_clamp_enable = false, bool rasterizer_discard_enable = false, vk::PolygonMode polygon_mode = vk::PolygonMode::eFill, vk::CullModeFlags cull_mode = vk::CullModeFlags(), 
					   vk::FrontFace front_face = vk::FrontFace::eCounterClockwise, bool depth_bias_enable = false, float depth_bias_constant_factor = 0, float depth_bias_clamp = 0, float depth_bias_slope_factor = 0, 
					   float line_width = 0, const std::optional<ExtRasterizationConservativeState>& conservative_state = {}, const std::optional<AMDRasterizationOrder>& rasterization_order = {});

	bool operator==(const RasterizationState& other) const;

	vk::PipelineRasterizationStateCreateInfo* buildCreateInfo();

	bool depth_clamp_enable;
	bool rasterizer_discard_enable;
	vk::PolygonMode polygon_mode;
	vk::CullModeFlags cull_mode;
	vk::FrontFace front_face;
	bool depth_bias_enable;
	float depth_bias_constant_factor;
	float depth_bias_clamp;
	float depth_bias_slope_factor;
	float line_width;
	std::optional<ExtRasterizationConservativeState> conservative_state;
	std::optional<AMDRasterizationOrder> rasterization_order;
private:
	vk::PipelineRasterizationStateCreateInfo vk_create_info_;
};

struct NvidiaCoverageModulationState {
	NvidiaCoverageModulationState(vk::CoverageModulationModeNV modulation_mode = vk::CoverageModulationModeNV::eNone, bool enable_modulation_table = false, const std::vector<float>& modulation_table = {});

	bool operator==(const NvidiaCoverageModulationState& other) const;

	vk::PipelineCoverageModulationStateCreateInfoNV* buildCreateInfo();

	vk::CoverageModulationModeNV modulation_mode;
	bool enable_modulation_table;
	std::vector<float> modulation_table;
private:
	vk::PipelineCoverageModulationStateCreateInfoNV vk_create_info_;
};

struct NvidiaCoverageToColorState {

	NvidiaCoverageToColorState(bool enable = false, uint32_t location = 0u);

	bool operator==(const NvidiaCoverageToColorState& other) const;

	vk::PipelineCoverageToColorStateCreateInfoNV* buildCreateInfo();

	bool enable;
	uint32_t location;
private:
	vk::PipelineCoverageToColorStateCreateInfoNV vk_create_info_;
};

struct ExtSampleLocationsState {

	ExtSampleLocationsState(bool enable = false, const vk::SampleLocationsInfoEXT& sample_locations_info = vk::SampleLocationsInfoEXT());

	bool operator==(const ExtSampleLocationsState& other) const;

	vk::PipelineSampleLocationsStateCreateInfoEXT* buildCreateInfo();

	bool enable;
	vk::SampleLocationsInfoEXT sample_locations_info;
private:
	vk::PipelineSampleLocationsStateCreateInfoEXT vk_create_info_;
};

struct MultisampleState {
	MultisampleState(vk::SampleCountFlagBits rasterization_samples = vk::SampleCountFlagBits::e1,
		bool enable_sample_shading = false, float min_sample_shading = 0,
		const vk::SampleMask& sample_mask = {0xFFFFFFFF}, bool enable_alpha_to_coverage = false,
		bool enable_alpha_to_one = false, 
		const std::optional<NvidiaCoverageModulationState>& coverage_modulation_state = {},
		const std::optional<NvidiaCoverageToColorState>& coverage_to_color_state = {},
		const std::optional<ExtSampleLocationsState>& sample_locations_state = {});

	bool operator==(const MultisampleState& other) const;

	vk::PipelineMultisampleStateCreateInfo* buildCreateInfo();

	vk::SampleCountFlagBits rasterization_samples;
	bool enable_sample_shading;
	float min_sample_shading;
	vk::SampleMask sample_mask;
	bool enable_alpha_to_coverage;
	bool enable_alpha_to_one;

	std::optional<NvidiaCoverageModulationState> coverage_modulation_state;
	std::optional<NvidiaCoverageToColorState> coverage_to_color_state;
	std::optional<ExtSampleLocationsState> sample_locations_state;
private:
	vk::PipelineMultisampleStateCreateInfo vk_create_info_;
};

struct DepthStencilState {
	DepthStencilState(bool enable_depth_test = false, bool enable_depth_write = false, 
					  vk::CompareOp depth_compare_op = vk::CompareOp::eNever, bool enable_depth_bounds_test = false,
					  bool enable_stencil_test = false, const vk::StencilOpState& front = {},
					  const vk::StencilOpState& back = {}, float min_depth_bounds = false,
					  float max_depth_bounds = false);

	bool operator==(const DepthStencilState& other) const;

	vk::PipelineDepthStencilStateCreateInfo* buildCreateInfo();

	bool enable_depth_test;
	bool enable_depth_write;
	vk::CompareOp depth_compare_op;
	bool enable_depth_bounds_test;
	bool enable_stencil_test;
	vk::StencilOpState front;
	vk::StencilOpState back;
	float min_depth_bounds;
	float max_depth_bounds;
private:
	vk::PipelineDepthStencilStateCreateInfo vk_create_info_;
};

struct ExtColorBlendAdvancedState {
	ExtColorBlendAdvancedState(bool src_premultiplied = false, bool dst_premultiplied = false, vk::BlendOverlapEXT blend_overlap = vk::BlendOverlapEXT::eUncorrelated);

	bool operator==(const ExtColorBlendAdvancedState& other) const;

	vk::PipelineColorBlendAdvancedStateCreateInfoEXT* buildCreateInfo();

	bool src_premultiplied;
	bool dst_premultiplied;
	vk::BlendOverlapEXT blend_overlap;
private:
	vk::PipelineColorBlendAdvancedStateCreateInfoEXT vk_create_info_;
};

struct ColorBlendState {
	ColorBlendState(bool enable_logic_op = false, vk::LogicOp logic_op = vk::LogicOp::eClear, const std::vector<vk::PipelineColorBlendAttachmentState>& attachment_states = {}, const std::array<float, 4>& blend_constants = { { 0, 0, 0, 0 } }, const std::optional<ExtColorBlendAdvancedState>&  advanced_state = {});

	bool operator==(const ColorBlendState& other) const;

	vk::PipelineColorBlendStateCreateInfo* buildCreateInfo();
	
	bool enable_logic_op;
	vk::LogicOp logic_op;
	std::vector<vk::PipelineColorBlendAttachmentState> attachment_states;
	std::array<float, 4> blend_constants;
	std::optional<ExtColorBlendAdvancedState> advanced_state;
private:
	vk::PipelineColorBlendStateCreateInfo vk_create_info_;
};

struct DynamicState {

	DynamicState(bool viewport = false, bool scissors = false, bool line_width = false, bool depth_bias = false, bool blend_constants = false,
				 bool depth_bounds = false, bool stencil_compare_mask = false, bool stencil_write_mask = false, bool stencil_reference = false,
				 bool nvidia_viewport_w_scaling = false, bool ext_discard_rectangle = false, bool ext_sample_locations = false);

	bool operator==(const DynamicState& other) const;

	vk::PipelineDynamicStateCreateInfo* buildCreateInfo();

	bool viewport;
	bool scissors;
	bool line_width;
	bool depth_bias;
	bool blend_constants;
	bool depth_bounds;
	bool stencil_compare_mask;
	bool stencil_write_mask;
	bool stencil_reference;
	bool nvidia_viewport_w_scaling;
	bool ext_discard_rectangle;
	bool ext_sample_locations;
private:
	std::vector<vk::DynamicState> dynamic_states_;
	vk::PipelineDynamicStateCreateInfo vk_create_info_;
};

struct PipelineState {
	PipelineState(InputAssemblyState input_assembly = {}, TessellationState tessellation = {}, ViewportState viewport = {}, RasterizationState rasterization = {},
				  MultisampleState multisample = {}, DepthStencilState depth_stencil = {}, ColorBlendState color_blend = {}, DynamicState dynamic_states = {});

	bool operator==(const PipelineState& other) const;

	InputAssemblyState	input_assembly;
	TessellationState	tessellation;
	ViewportState		viewport;
	RasterizationState	rasterization;
	MultisampleState	multisample;
	DepthStencilState	depth_stencil;
	ColorBlendState		color_blend;
	DynamicState		dynamic_states;
};

} ///!	namespace vkr

#endif ///!	HEADER_GUARD