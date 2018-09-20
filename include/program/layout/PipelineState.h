#ifndef PROGRAM_LAYOUT_PIPELINE_STATE_H
#define PROGRAM_LAYOUT_PIPELINE_STATE_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include <array>

#include "base/ExtensionObject.h"
#include <set>

namespace logi {

/**
 * @brief	Convinience structure used to build the Vulkan PipelineInputAssemblyStateCreateInfo.
 */
struct InputAssemblyState : public BuildableExtendable {

    /**
     * @brief	Default constructor that initializes members with either default or the given values.
     *
     * @param	topology					Defines primitive topology.
     * @param	primitive_restart_enable	If true a special vertex index value will be treated as
     *										restarting the assembly of primitives.
     * @param	flags						Additional configuration flags.
     */
    explicit InputAssemblyState(vk::PrimitiveTopology topology = vk::PrimitiveTopology::eTriangleList,
                                bool primitive_restart_enable = false,
                                const vk::PipelineInputAssemblyStateCreateFlags& flags = {});

    /**
     * @brief	Compares InputAssemblyStates if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side InputAsseblyState object.
     * @return	True if the InputAssamblyState objects contain equivalent configuration.
     */
    bool operator==(const InputAssemblyState& rhs) const;

    /**
     * @brief	Build Vulkan vk::PipelineInputAssemblyStateCreateInfo using the member values.
     *
     * @return	Pointer to vk::PipelineInputAssemblyStateCreateInfo structure.
     */
    const vk::PipelineInputAssemblyStateCreateInfo* buildCreateInfo();

    /**
     * Defines primitive topology.
     */
    vk::PrimitiveTopology topology;

    /**
     * Controls whether a special vertex index value is treated as restarting the assembly of primitives.
     */
    bool primitive_restart_enable;

    /**
     * Additional configuration flags.
     */
    vk::PipelineInputAssemblyStateCreateFlags flags;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineInputAssemblyStateCreateInfo vk_create_info_;
};


/**
 * @brief	Convinience structure used to build the Vulkan PipelineTessellationDomainOriginStateCreateInfo.
 */
struct TessellationDomainOriginState : public BuildableExtension {

    /**
     * @brief	Default constructor that initializes members with either default or given values.
     *
     * @param	domain_origin	The domain origin.
     */
    explicit TessellationDomainOriginState(
        vk::TessellationDomainOrigin domain_origin = vk::TessellationDomainOrigin::eUpperLeft);

    /**
     * @brief	Deep copy the object.
     *
     * @return	Unique pointer to the copy.
     */
    std::unique_ptr<BuildableExtension> clone() const override;

    /**
     * @brief	Compares TessellationDomainOriginStates if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side TessellationDomainOriginState object.
     * @return	True if the TessellationDomainOriginState objects contain equivalent configuration.
     */
    bool operator==(const TessellationDomainOriginState& rhs) const;

    /**
     * @brief	Build Vulkan vk::PipelineTessellationDomainOriginStateCreateInfo using the member values.
     *
     * @return	Pointer to vk::PipelineTessellationDomainOriginStateCreateInfo structure.
     */
    vk::PipelineTessellationDomainOriginStateCreateInfo* buildCreateInfo();

    /**
     * @brief	Build PipelineTessellationDomainOriginStateCreateInfo, set its next pointer to the given pointer and
     *			return void pointer to the generated create info.
     *
     * @param	next	Pointer that will be used to set create info next.
     * @return	Pointer to the generated create info structure.
     */
    void* build(void* next) override;

    /**
     * Controls the origin of the tessellation domain space.
     */
    vk::TessellationDomainOrigin domain_origin;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineTessellationDomainOriginStateCreateInfo vk_create_info_;
};


/**
 * @brief	Convinience structure used to build the Vulkan PipelineTessellationStateCreateInfo.
 */
struct TessellationState : public BuildableExtendable {

    /**
     * @brief	Default constructor that initializes members with either default or given values.
     *
     * @param	patch_control_points	Number of control points per patch.
     * @param   flags                   Additional configuration flags.
     */
    explicit TessellationState(uint32_t patch_control_points = 0, const vk::PipelineTessellationStateCreateFlags& flags = {});

    /**
     * @brief	Compare TessellationStates if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side TessellationState object.
     * @return	True if the TessellationState objects contain equivalent configuration.
     */
    bool operator==(const TessellationState& rhs) const;

    /**
     * @brief	Build Vulkan PipelineTessellationStateCreateInfo using the member values.
     *
     * @return	PipelineTessellationStateCreateInfo structure.
     */
    const vk::PipelineTessellationStateCreateInfo* buildCreateInfo();

    /**
     * Number of control points per patch.
     */
    uint32_t patch_control_points;

    /**
     * Additional configuration flags.
     */
    vk::PipelineTessellationStateCreateFlags flags;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineTessellationStateCreateInfo vk_create_info_;
};

/**
 * @brief	Convinience structure used to build the Vulkan PipelineViewportSwizzleStateCreateInfoNV.
 */
struct NvidiaViewportSwizzleState : public BuildableExtension {

    /**
     * @brief	Default constructor that initializes members with either default or given values.
     *
     * @param	swizzle_states	Vector of viewport swizzles.
     * @param	flags			Additional configuration flags.
     */
    explicit NvidiaViewportSwizzleState(std::vector<vk::ViewportSwizzleNV> swizzle_states = {},
                                        const vk::PipelineViewportSwizzleStateCreateFlagsNV& flags = {});

    /**
     * @brief	Deep copy the object.
     *
     * @return	Unique pointer to the copy.
     */
    std::unique_ptr<BuildableExtension> clone() const override;

    /**
     * @brief	Compare NvidiaViewportSwizzleStates if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side NvidiaViewportSwizzleStates object.
     * @return	True if the NvidiaViewportSwizzleState objects contain equivalent configuration.
     */
    bool operator==(const NvidiaViewportSwizzleState& rhs) const;

    /**
     * @brief	Build Vulkan PipelineViewportSwizzleStateCreateInfoNV using the member values.
     *
     * @return	PipelineViewportSwizzleStateCreateInfoNV structure.
     */
    vk::PipelineViewportSwizzleStateCreateInfoNV* buildCreateInfo();

    /**
     * @brief	Build PipelineViewportSwizzleStateCreateInfoNV, set its next pointer to the given pointer and return
     *			void pointer to the generated create info.
     *
     * @param	next	Pointer that will be used to set create info next.
     * @return	Pointer to the generated create info structure.
     */
    void* build(void* next) override;

    /**
     * Vector of viewport swizzles.
     */
    std::vector<vk::ViewportSwizzleNV> swizzle_states;

    /**
     * Additional configuration flags.
     */
    vk::PipelineViewportSwizzleStateCreateFlagsNV flags;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineViewportSwizzleStateCreateInfoNV vk_create_info_;
};

/**
 * @brief	Convinience structure used to build the Vulkan PipelineViewportWScalingStateCreateInfoNV.
 */
struct NvidiaViewportWScalingState : public BuildableExtension {

    /**
     * @brief	Default constructor that initializes members with either default or given values.
     *
     * @param	enable_w_scaling	Enable w scaling.
     * @param	w_scalings			Vector of ViewportWScalingNVs.
     */
    explicit NvidiaViewportWScalingState(bool enable_w_scaling = true,
                                         std::vector<vk::ViewportWScalingNV> w_scalings = {});

    /**
     * @brief	Deep copy the object.
     *
     * @return	Unique pointer to the copy.
     */
    std::unique_ptr<BuildableExtension> clone() const override;

    /**
    ¸* @brief	Compare NvidiaViewportWScalingState if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side NvidiaViewportWScalingState object.
     * @return	True if the NvidiaViewportWScalingState objects contain equivalent configuration.
     */
    bool operator==(const NvidiaViewportWScalingState& rhs) const;

    /**
     * @brief	Build Vulkan PipelineViewportWScalingStateCreateInfoNV using the member values.
     *
     * @return	PipelineViewportWScalingStateCreateInfoNV structure.
     */
    vk::PipelineViewportWScalingStateCreateInfoNV* buildCreateInfo();

    /**
     * @brief	Build PipelineViewportWScalingStateCreateInfoNV, set its next pointer to the given pointer and return
     *			void pointer to the generated create info.
      *
     * @param	next	Pointer that will be used to set create info next.
     * @return	Pointer to the generated create info structure.
     */
    void* build(void* next) override;

    /**
     * True if w scaling should be enabled.
     */
    bool enable_w_scaling;

    /**
     * Specifies w scaling for each viewport.
     */
    std::vector<vk::ViewportWScalingNV> w_scalings;

private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineViewportWScalingStateCreateInfoNV vk_create_info_;
};

/**
 * @brief	Convinience structure used to build the Vulkan PipelineViewportStateCreateInfo.
 */
struct ViewportState : public BuildableExtendable {
    /**
     * @brief	Default constructor that initializes members with either default or given values.
     *
     * @param	viewports   Vector containing the viewport transforms. If the viewport state is dynamic, this member is 
     *                      ignored.
     * @param	scissors    Vector containing rectangular bounds of the scissor for the corresponding viewport. If the 
     *                      scissor state is dynamic, this member is ignored.
     * @param   flags       Additional configuration flags.
     */
    explicit ViewportState(std::vector<vk::Viewport> viewports = {}, std::vector<vk::Rect2D> scissors = {},
                           const vk::PipelineViewportStateCreateFlags& flags = {});

    /**
     * @brief	Compare ViewportState if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side ViewportState object.
     * @return	True if the ViewportState objects contain equivalent configuration.
     */
    bool operator==(const ViewportState& rhs) const;

    /**
     * @brief	Build Vulkan PipelineViewportStateCreateInfo using the member values.
     *
     * @return	PipelineViewportStateCreateInfo structure.
     */
    vk::PipelineViewportStateCreateInfo* buildCreateInfo();

    /**
     * Vector containing the viewport transforms. If the viewport state is dynamic, this member is ignored.
     */
    std::vector<vk::Viewport> viewports;

    /**
     * Vector containing rectangular bounds of the scissor for the corresponding viewport. If the scissor state is dynamic, this member is ignored.
     */
    std::vector<vk::Rect2D> scissors;

    /**
     * Additional configuration flags.
     */
    vk::PipelineViewportStateCreateFlags flags;
private:
    vk::PipelineViewportStateCreateInfo vk_create_info_; ///< Vulkan create info structure.
};

/**
 * @brief	Convinience structure used to build the Vulkan PipelineRasterizationConservativeStateCreateInfoEXT.
 */
struct ExtRasterizationConservativeState : public BuildableExtension {
    /**
     * @brief	Default constructor that initializes members with either default or given values.
     *
     * @param	flags	                            Additional configuration flags.
     * @param	conservative_mode	                Conservative rasterization mode.
     * @param	extra_primitive_overestimation_size	Extra primitive overestimation size.
     */
    explicit ExtRasterizationConservativeState(
        vk::ConservativeRasterizationModeEXT conservative_mode = vk::ConservativeRasterizationModeEXT::eDisabled,
        float extra_primitive_overestimation_size = 0,
        const vk::PipelineRasterizationConservativeStateCreateFlagsEXT& flags = {});

    /**
     * @brief	Deep copy the object.
     *
     * @return	Unique pointer to the copy.
     */
    std::unique_ptr<BuildableExtension> clone() const override;

    /**
     * @brief	Compare ExtRasterizationConservativeState if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side ExtRasterizationConservativeState object.
     * @return	True if the ExtRasterizationConservativeState objects contain equivalent configuration.
     */
    bool operator==(const ExtRasterizationConservativeState& rhs) const;

    /**
     * @brief	Build Vulkan PipelineRasterizationConservativeStateCreateInfoEXT using the member values.
     *
     * @return	PipelineRasterizationConservativeStateCreateInfoEXT structure.
     */
    vk::PipelineRasterizationConservativeStateCreateInfoEXT* buildCreateInfo();

    /**
     * @brief	Build PipelineRasterizationConservativeStateCreateInfoEXT, set its next pointer to the given pointer and
     *          return void pointer to the generated create info.
     *
     * @param	next	Pointer that will be used to set create info next.
     * @return	Pointer to the generated create info structure.
     */
    void* build(void* next) override;

    /**
     * Conservative rasterization mode.
     */
    vk::ConservativeRasterizationModeEXT conservative_mode;

    /**
     * Extra primitive overestimation size.
     */
    float extra_primitive_overestimation_size;

    /**
     * Additional configuration flags.
     */
    vk::PipelineRasterizationConservativeStateCreateFlagsEXT flags;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineRasterizationConservativeStateCreateInfoEXT vk_create_info_;
};


/**
 * @brief	Convinience structure used to build the Vulkan PipelineRasterizationStateRasterizationOrderAMD.
 */
struct AMDRasterizationOrder : public BuildableExtension {
    /**
     * @brief 	Default constructor that initializes members with either default or given values.
     *
     * @param	rasterization_order Rasterization order.
     */
    explicit AMDRasterizationOrder(vk::RasterizationOrderAMD rasterization_order = vk::RasterizationOrderAMD::eStrict);

    /**
     * @brief	Deep copy the object.
     *
     * @return	Unique pointer to the copy.
     */
    std::unique_ptr<BuildableExtension> clone() const override;

    /**
     * @brief	Compare AMDRasterizationOrder if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side AMDRasterizationOrder object.
     * @return	True if the AMDRasterizationOrder objects contain equivalent configuration.
     */
    bool operator==(const AMDRasterizationOrder& rhs) const;

    /**
     * @brief	Build Vulkan PipelineRasterizationStateRasterizationOrderAMD using the member values.
     *
     * @return	PipelineRasterizationStateRasterizationOrderAMD structure.
     */
    vk::PipelineRasterizationStateRasterizationOrderAMD* buildCreateInfo();

    /**
     * @brief	Build PipelineRasterizationStateRasterizationOrderAMD, set its next pointer to the given pointer and
     *          return void pointer to the generated create info.
     *
     * @param	next	Pointer that will be used to set create info next.
     * @return	Pointer to the generated create info structure.
     */
    void* build(void* next) override;

    /**
     * Rasterization order.
     */
    vk::RasterizationOrderAMD rasterization_order;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineRasterizationStateRasterizationOrderAMD vk_create_info_;
};


/**
 * @brief	Convinience structure used to build the Vulkan PipelineRasterizationStateCreateInfo.
 */
struct RasterizationState : public BuildableExtendable {

    /**
     * @brief 
     *
     * @param	depth_clamp_enable          Controls whether to clamp the fragment’s depth values instead of clipping 
     *                                      primitives to the z planes of the frustum, as described in Primitive Clipping.
     * @param	rasterizer_discard_enable	Controls whether primitives are discarded immediately before the
     *                                      rasterization stage.
     * @param	polygon_mode	            Triangle rendering mode.
     * @param	cull_mode	                Triangle facing direction used for primitive culling.
     * @param	front_face	                Specifies the front-facing triangle orientation to be used for culling.
     * @param	depth_bias_enable	        Whether to bias fragment depth.
     * @param	depth_bias_constant_factor	Scalar factor controlling the constant depth value added to each fragment.
     * @param	depth_bias_clamp	        Maximum (or minimum) depth bias of a fragment.
     * @param	depth_bias_slope_factor	    Scalar factor applied to a fragment’s slope in depth bias calculations.
     * @param	line_width	                Width of rasterized line segments.
     * @param   flags                       Additional configuration flags.
     */
    explicit RasterizationState(bool depth_clamp_enable = false,
                                bool rasterizer_discard_enable = false,
                                vk::PolygonMode polygon_mode = vk::PolygonMode::eFill,
                                const vk::CullModeFlags& cull_mode = vk::CullModeFlags(),
                                vk::FrontFace front_face = vk::FrontFace::eCounterClockwise,
                                bool depth_bias_enable = false,
                                float depth_bias_constant_factor = 0,
                                float depth_bias_clamp = 0,
                                float depth_bias_slope_factor = 0,
                                float line_width = 0,
                                const vk::PipelineRasterizationStateCreateFlags& flags = {});


    /**
     * @brief	Compare RasterizationState if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side RasterizationState object.
     * @return	True if the RasterizationState objects contain equivalent configuration.
     */
    bool operator==(const RasterizationState& rhs) const;

    /**
     * @brief	Build Vulkan RasterizationState using the member values.
     *
     * @return	RasterizationState structure.
     */
    vk::PipelineRasterizationStateCreateInfo* buildCreateInfo();

    /**
     * Controls whether to clamp the fragment’s depth values instead of clipping primitives to the z planes of the
     * frustum, as described in Primitive Clipping.
     */
    bool depth_clamp_enable;

    /**
     * Controls whether primitives are discarded immediately before the rasterization stage.
     */
    bool rasterizer_discard_enable;

    /**
     * Triangle rendering mode.
     */
    vk::PolygonMode polygon_mode;

    /**
     * Triangle facing direction used for primitive culling.
     */
    vk::CullModeFlags cull_mode;

    /**
     * Specifies the front-facing triangle orientation to be used for culling.
     */
    vk::FrontFace front_face;

    /**
     * Whether to bias fragment depth.
     */
    bool depth_bias_enable;

    /**
     * Scalar factor controlling the constant depth value added to each fragment.
     */
    float depth_bias_constant_factor;

    /**
     * Maximum (or minimum) depth bias of a fragment.
     */
    float depth_bias_clamp;

    /**
     * Scalar factor applied to a fragment’s slope in depth bias calculations.
     */
    float depth_bias_slope_factor;

    /**
     * Width of rasterized line segments.
     */
    float line_width;

    /**
     * Additional configuration flags.
     */
    vk::PipelineRasterizationStateCreateFlags flags;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineRasterizationStateCreateInfo vk_create_info_;
};


/**
 * @brief   Convinience structure used to build the Vulkan PipelineCoverageModulationStateCreateInfoNV.
 */
struct NvidiaCoverageModulationState : public BuildableExtension {
    /**
     * @brief  	Default constructor that initializes members with either default or given values.
     *
     * @param	modulation_mode	        Controls which color components are modulated and is of type.
     * @param	enable_modulation_table Controls whether the modulation factor is looked up from a table in. 
     * @param	modulation_table	    Table of modulation factors containing a value for each number of covered samples.
     * @param	flags	                Additional configuration flags.
     */
    explicit NvidiaCoverageModulationState(
        vk::CoverageModulationModeNV modulation_mode = vk::CoverageModulationModeNV::eNone,
        bool enable_modulation_table = false,
        std::vector<float> modulation_table = {},
        const vk::PipelineCoverageModulationStateCreateFlagsNV& flags = {});

    /**
     * @brief	Deep copy the object.
     *
     * @return	Unique pointer to the copy.
     */
    std::unique_ptr<BuildableExtension> clone() const override;

    /**
     * @brief	Compare NvidiaCoverageModulationState if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side NvidiaCoverageModulationState object.
     * @return	True if the NvidiaCoverageModulationState objects contain equivalent configuration.
     */
    bool operator==(const NvidiaCoverageModulationState& rhs) const;

    /**
     * @brief	Build Vulkan PipelineCoverageModulationStateCreateInfoNV using the member values.
     *
     * @return	PipelineCoverageModulationStateCreateInfoNV structure.
     */
    vk::PipelineCoverageModulationStateCreateInfoNV* buildCreateInfo();

    /**
     * @brief	Build PipelineCoverageModulationStateCreateInfoNV, set its next pointer to the given pointer and
     *          return void pointer to the generated create info.
     *
     * @param	next	Pointer that will be used to set create info next.
     * @return	Pointer to the generated create info structure.
     */
    void* build(void* next) override;

    /**
     * Controls which color components are modulated and is of type.
     */
    vk::CoverageModulationModeNV modulation_mode;

    /**
     * Controls whether the modulation factor is looked up from a table in. 
     */
    bool enable_modulation_table;

    /**
     * Table of modulation factors containing a value for each number of covered samples.
     */
    std::vector<float> modulation_table;

    /**
     * Additional configuration flags.
     */
    vk::PipelineCoverageModulationStateCreateFlagsNV flags;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineCoverageModulationStateCreateInfoNV vk_create_info_;
};


/**
 * @brief   Convinience structure used to build the Vulkan PipelineCoverageToColorStateCreateInfoNV.
 */
struct NvidiaCoverageToColorState : public BuildableExtension {

    /**
	 * @brief   Default constructor that initializes members with either default or given values.
	 *
	 * @param	enable      Controls whether the fragment coverage value replaces a fragment color output.
	 * @param	location    Controls which fragment shader color output value is replaced.
	 * @param   flags	    Additional configuration flags.
	 */
    explicit NvidiaCoverageToColorState(bool enable = false, uint32_t location = 0u,
                                        const vk::PipelineCoverageToColorStateCreateFlagsNV& flags = {});

    /**
     * @brief	Deep copy the object.
     *
     * @return	Unique pointer to the copy.
     */
    std::unique_ptr<BuildableExtension> clone() const override;

    /**
     * @brief	Compare NvidiaCoverageToColorState if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side NvidiaCoverageToColorState object.
     * @return	True if the NvidiaCoverageToColorState objects contain equivalent configuration.
     */
    bool operator==(const NvidiaCoverageToColorState& rhs) const;

    /**
     * @brief	Build Vulkan PipelineCoverageToColorStateCreateInfoNV using the member values.
     *
     * @return	PipelineCoverageModulationStateCreateInfoNV structure.
     */
    vk::PipelineCoverageToColorStateCreateInfoNV* buildCreateInfo();

    /**
     * @brief	Build PipelineCoverageToColorStateCreateInfoNV, set its next pointer to the given pointer and
     *          return void pointer to the generated create info.
     *
     * @param	next	Pointer that will be used to set create info next.
     * @return	Pointer to the generated create info structure.
     */
    void* build(void* next) override;

    /**
     * Controls whether the fragment coverage value replaces a fragment color output.
     */
    bool enable;

    /**
     * Controls which fragment shader color output value is replaced.
     */
    uint32_t location;

    /**
     * Additional configuration flags.
     */
    vk::PipelineCoverageToColorStateCreateFlagsNV flags;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineCoverageToColorStateCreateInfoNV vk_create_info_;
};


/**
 * @brief   Convinience structure used to build the Vulkan PipelineSampleLocationsStateCreateInfoEXT.
 */
struct ExtSampleLocationsState : public BuildableExtension {

    /**
     * @brief   Default constructor that initializes members with either default or given values.
     *
     * @param	enable	                Controls whether custom sample locations are used.
     * @param	sample_locations_info   Sample locations to use during rasterization.
     */
    explicit ExtSampleLocationsState(bool enable = false,
                                     const vk::SampleLocationsInfoEXT& sample_locations_info = vk::
                                         SampleLocationsInfoEXT());

    /**
     * @brief	Deep copy the object.
     *
     * @return	Unique pointer to the copy.
     */
    std::unique_ptr<BuildableExtension> clone() const override;

    /**
     * @brief	Compare ExtSampleLocationsState if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side ExtSampleLocationsState object.
     * @return	True if the ExtSampleLocationsState objects contain equivalent configuration.
     */
    bool operator==(const ExtSampleLocationsState& rhs) const;

    /**
     * @brief	Build Vulkan PipelineSampleLocationsStateCreateInfoEXT using the member values.
     *
     * @return	PipelineSampleLocationsStateCreateInfoEXT structure.
     */
    vk::PipelineSampleLocationsStateCreateInfoEXT* buildCreateInfo();

    /**
     * @brief	Build PipelineSampleLocationsStateCreateInfoEXT, set its next pointer to the given pointer and
     *          return void pointer to the generated create info.
     *
     * @param	next	Pointer that will be used to set create info next.
     * @return	Pointer to the generated create info structure.
     */
    void* build(void* next) override;

    /**
     * Controls whether custom sample locations are used.
     */
    bool enable;

    /**
     * Sample locations to use during rasterization.
     */
    vk::SampleLocationsInfoEXT sample_locations_info;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineSampleLocationsStateCreateInfoEXT vk_create_info_;
};


/**
* @brief   Convinience structure used to build the Vulkan PipelineMultisampleStateCreateInfo.
*/
struct MultisampleState : public BuildableExtendable {
    /**
     * @brief   Default constructor that initializes members with either default or given values.
     *
     * @param	rasterization_samples	    Specifies the number of samples per pixel used in rasterization.
     * @param	enable_sample_shading	    Controls whether the sample shading is enabled.
     * @param	min_sample_shading	        Controls whether the sample shading is enabled.
     * @param	sample_mask	                Specifies a minimum fraction of sample shading.
     * @param	enable_alpha_to_coverage	A bitmask of static coverage information that is ANDed with the coverage 
     *                                      information generated during rasterization.
     * @param	enable_alpha_to_one	        Controls whether the alpha component of the fragment’s first color output is 
     *                                      replaced.
     * @param	flags	                    Additional configuration flags.
     */
    explicit MultisampleState(vk::SampleCountFlagBits rasterization_samples = vk::SampleCountFlagBits::e1,
                              bool enable_sample_shading = false, float min_sample_shading = 0,
                              vk::SampleMask sample_mask = 0xFFFFFFFF, bool enable_alpha_to_coverage = false,
                              bool enable_alpha_to_one = false, const vk::PipelineMultisampleStateCreateFlags& flags = {});

    /**
     * @brief	Compare MultisampleState if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side MultisampleState object.
     * @return	True if the MultisampleState objects contain equivalent configuration.
     */
    bool operator==(const MultisampleState& rhs) const;

    /**
     * @brief	Build Vulkan PipelineMultisampleStateCreateInfo using the member values.
     *
     * @return	PipelineMultisampleStateCreateInfo structure.
     */
    vk::PipelineMultisampleStateCreateInfo* buildCreateInfo();

    /**
     * Specifies the number of samples per pixel used in rasterization.
     */
    vk::SampleCountFlagBits rasterization_samples;

    /**
     * Controls whether the sample shading is enabled.
     */
    bool enable_sample_shading;

    /**
     * Specifies a minimum fraction of sample shading.
     */
    float min_sample_shading;

    /**
     * A bitmask of static coverage information that is ANDed with the coverage information generated during
     * rasterization.
     */
    vk::SampleMask sample_mask;

    /**
     * Controls whether a temporary coverage value is generated based on the alpha component.
     */
    bool enable_alpha_to_coverage;

    /**
     * Controls whether the alpha component of the fragment’s first color output is replaced.
     */
    bool enable_alpha_to_one;

    /**
     * Additional configuration flags.
     */
    vk::PipelineMultisampleStateCreateFlags flags;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineMultisampleStateCreateInfo vk_create_info_;
};


/**
 * @brief   Convinience structure used to build the Vulkan PipelineDepthStencilStateCreateInfo.
 */
struct DepthStencilState : public BuildableExtendable {

    /**
	 * @brief   Default constructor that initializes members with either default or given values.
	 *
	 * @param	enable_depth_test	        Controls whether depth testing is enabled.
	 * @param	enable_depth_write	        Controls whether depth writes are enabled.
	 * @param	depth_compare_op	        Comparison operator used in the depth test.
	 * @param	enable_depth_bounds_test	Controls whether depth bounds testing is enabled.
	 * @param	enable_stencil_test	        Controls whether stencil testing is enabled.
	 * @param	front	                    Stencil test control parameter.
	 * @param	back	                    Stencil test control parameter.
	 * @param	min_depth_bounds	        Minimum bound of depth range.
	 * @param	max_depth_bounds	        Minimum bound of depth range.
	 * @param	flags	                    Additional configuration flags.
	 */
    explicit DepthStencilState(bool enable_depth_test = false, bool enable_depth_write = false,
                               vk::CompareOp depth_compare_op = vk::CompareOp::eNever,
                               bool enable_depth_bounds_test = false,
                               bool enable_stencil_test = false, const vk::StencilOpState& front = {},
                               const vk::StencilOpState& back = {}, float min_depth_bounds = false,
                               float max_depth_bounds = false, const vk::PipelineDepthStencilStateCreateFlags& flags = {});

    /**
     * @brief	Compare DepthStencilState if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side DepthStencilState object.
     * @return	True if the DepthStencilState objects contain equivalent configuration.
     */
    bool operator==(const DepthStencilState& rhs) const;


    /**
     * @brief	Build Vulkan PipelineDepthStencilStateCreateInfo using the member values.
     *
     * @return	PipelineDepthStencilStateCreateInfo structure.
     */
    vk::PipelineDepthStencilStateCreateInfo* buildCreateInfo();

    /**
     * Controls whether depth testing is enabled.
     */
    bool enable_depth_test;

    /**
     * Controls whether depth writes are enabled.
     */
    bool enable_depth_write;

    /**
     * Comparison operator used in the depth test.
     */
    vk::CompareOp depth_compare_op;

    /**
     * Controls whether depth bounds testing is enabled.
     */
    bool enable_depth_bounds_test;

    /**
     * Controls whether stencil testing is enabled.
     */
    bool enable_stencil_test;

    /**
     * Stencil test control parameter.
     */
    vk::StencilOpState front;

    /**
     * Stencil test control parameter.
     */
    vk::StencilOpState back;

    /**
     * Minimum bound of depth range.
     */
    float min_depth_bounds;

    /**
     * Maxiumum bound of depth range.
     */
    float max_depth_bounds;

    /**
     * Additional configuration flags.
     */
    vk::PipelineDepthStencilStateCreateFlags flags;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineDepthStencilStateCreateInfo vk_create_info_;
};

/**
 * @brief   Convinience structure used to build the Vulkan PipelineColorBlendAdvancedStateCreateInfoEXT.
 */
struct ExtColorBlendAdvancedState : public BuildableExtension {

    /**
	 * @brief   Default constructor that initializes members with either default or given values.
	 *
	 * @param	src_premultiplied   Specifies whether the source color of the blend operation is treated as
	 *                              premultiplied.
	 * @param	dst_premultiplied	Specifies whether the destination color of the blend operation is treated as
	 *                              premultiplied.
	 * @param	blend_overlap	    Specifies how the source and destination samples coverage is correlated.
	 */
    explicit ExtColorBlendAdvancedState(bool src_premultiplied = false, bool dst_premultiplied = false,
                                        vk::BlendOverlapEXT blend_overlap = vk::BlendOverlapEXT::eUncorrelated);

    /**
     * @brief	Deep copy the object.
     *
     * @return	Unique pointer to the copy.
     */
    std::unique_ptr<BuildableExtension> clone() const override;

    /**
     * @brief	Compare PipelineColorBlendAdvancedStateCreateInfoEXT if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side PipelineColorBlendAdvancedStateCreateInfoEXT object.
     * @return	True if the PipelineColorBlendAdvancedStateCreateInfoEXT objects contain equivalent configuration.
     */
    bool operator==(const ExtColorBlendAdvancedState& rhs) const;

    /**
     * @brief	Build Vulkan PipelineColorBlendAdvancedStateCreateInfoEXT using the member values.
     *
     * @return	PipelineColorBlendAdvancedStateCreateInfoEXT structure.
     */
    vk::PipelineColorBlendAdvancedStateCreateInfoEXT* buildCreateInfo();

    /**
     * @brief	Build PipelineColorBlendAdvancedStateCreateInfoEXT, set its next pointer to the given pointer and
     *          return void pointer to the generated create info.
     *
     * @param	next	Pointer that will be used to set create info next.
     * @return	Pointer to the generated create info structure.
     */
    void* build(void* next) override;

    /**
     * Specifies whether the source color of the blend operation is treated as premultiplied.
     */
    bool src_premultiplied;

    /**
     * Specifies whether the destination color of the blend operation is treated as premultiplied.
     */
    bool dst_premultiplied;

    /**
     * Specifies how the source and destination samples coverage is correlated.
     */
    vk::BlendOverlapEXT blend_overlap;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineColorBlendAdvancedStateCreateInfoEXT vk_create_info_;
};

/**
 * @brief   Convinience structure used to build the Vulkan PipelineColorBlendStateCreateInfo.
 */
struct ColorBlendState : public BuildableExtendable {
    /**
	 * @brief   Default constructor that initializes members with either default or given values.
	 *
	 * @param	enable_logic_op     Controls whether to apply logical operations.
	 * @param	logic_op	        Selects which logical operation to apply.
	 * @param	attachment_states	Vector of per target attachment states.
	 * @param	blend_constants	    Four values used as the R, G, B, and A components of the blend constant that are 
	 *                              used in blending, depending on the blend factor.
	 * @param	flags	            Additional configuration flags.
	 */
    explicit ColorBlendState(bool enable_logic_op = false, vk::LogicOp logic_op = vk::LogicOp::eClear,
                             std::vector<vk::PipelineColorBlendAttachmentState> attachment_states = {},
                             const std::array<float, 4>& blend_constants = { 0.0f, 0.0f, 0.0f, 0.0f },
                             const vk::PipelineColorBlendStateCreateFlags& flags = {});


    /**
     * @brief	Compare ColorBlendState if they are contain equivalent configuration.
     *
     * @param	rhs Right hand side ColorBlendState object.
     * @return	True if the ColorBlendState objects contain equivalent configuration.
     */
    bool operator==(const ColorBlendState& rhs) const;

    /**
     * @brief	Build Vulkan PipelineColorBlendStateCreateInfo using the member values.
     *
     * @return	PipelineColorBlendStateCreateInfo structure.
     */
    vk::PipelineColorBlendStateCreateInfo* buildCreateInfo();

    /**
     * Controls whether to apply logical operations.
     */
    bool enable_logic_op;

    /**
     * Selects which logical operation to apply.
     */
    vk::LogicOp logic_op;

    /**
     * Vector of per target attachment states.
     */
    std::vector<vk::PipelineColorBlendAttachmentState> attachment_states;

    /**
     * Four values used as the R, G, B, and A components of the blend constant that are used in blending, depending on 
     * the blend factor.
     */
    std::array<float, 4> blend_constants;

    /**
     * Additional configuration flags.
     */
    vk::PipelineColorBlendStateCreateFlags flags;
private:
    /**
     * Vulkan create info structure.
     */
    vk::PipelineColorBlendStateCreateInfo vk_create_info_;
};

/**
 * @brief   Convinience structure used to build the Vulkan PipelineDynamicStateCreateInfo.
 */
struct DynamicState : public BuildableExtendable {

    /**
     * @brief   Default constructor that initializes members with either default or given values.
     *
     * @param	dynamic_states	
     * @param	flags	
     */
    explicit DynamicState(std::set<vk::DynamicState> dynamic_states = {},
                          const vk::PipelineDynamicStateCreateFlags& flags = {});

    /**
	 * @brief	Compare DynamicState if they are contain equivalent configuration.
	 *
	 * @param	rhs Right hand side DynamicState object.
	 * @return	True if the DynamicState objects contain equivalent configuration.
	 */
    bool operator==(const DynamicState& rhs) const;

    /**
     * @brief	Build Vulkan PipelineColorBlendStateCreateInfo using the member values.
     *
     * @return	PipelineColorBlendStateCreateInfo structure.
     */
    vk::PipelineDynamicStateCreateInfo* buildCreateInfo();

    /**
	 * @brief   Enable or disable the dynamic state.
	 *
	 * @param	state	Dynamic state to be enabled/disabled.
	 * @param	enable	True to enable the dynamic state.
	 */
    void set(vk::DynamicState state, bool enable);

    /**
	 * @brief   Check if the given dynamic state is enabled.
	 *
	 * @param	state	Dynamic state.
	 * @return	True if the given dynamic state is enabled.
	 */
    bool isSet(vk::DynamicState state);

    /**
     * Additional configuration flags.
     */
    vk::PipelineDynamicStateCreateFlags flags;
private:
    /**
     * Set of enabled dynamic states.
     */
    std::set<vk::DynamicState> dynamic_states_set_;

    /**
     * Vector of enabled dynamic states.
     */
    std::vector<vk::DynamicState> dynamic_states_;

    /**
     * Vulkan create info structure.
     */
    vk::PipelineDynamicStateCreateInfo vk_create_info_;
};

/**
 * @brief   Hold graphical pipeline state.
 */
struct PipelineState {
    /**
	 * @brief   Default constructor that initializes members with either defaults or the given state structures.
	 *
	 * @param	input_assembly	Input assembly state structure.
	 * @param	tessellation    Tesselation state structure.
	 * @param	viewport	    Viewport state structure.
	 * @param	rasterization	Rasterization state structure.
	 * @param	multisample	    Multisample state structure.
	 * @param	depth_stencil   Depth, stencil state structure.
	 * @param	color_blend	    Color blend state structure.
	 * @param	dynamic_states	Dynamic state structure.
	 */
    explicit PipelineState(InputAssemblyState input_assembly = InputAssemblyState(),
                           TessellationState tessellation = TessellationState(),
                           ViewportState viewport = ViewportState(),
                           RasterizationState rasterization = RasterizationState(),
                           MultisampleState multisample = MultisampleState(),
                           DepthStencilState depth_stencil = DepthStencilState(),
                           ColorBlendState color_blend = ColorBlendState(),
                           DynamicState dynamic_states = DynamicState());

    bool operator==(const PipelineState& rhs) const;

    /**
     * Input assembly state structure.
     */
    InputAssemblyState input_assembly;

    /**
     * Tesselation state structure.
     */
    TessellationState tessellation;

    /**
     * Viewport state structure.
     */
    ViewportState viewport;

    /**
     * Rasterization state structure.
     */
    RasterizationState rasterization;

    /**
     * Multisample state structure.
     */
    MultisampleState multisample;

    /**
     * Depth, stencil state structure.
     */
    DepthStencilState depth_stencil;

    /**
     * Color blend state structure.
     */
    ColorBlendState color_blend;

    /**
     * Dynamic state structure.
     */
    DynamicState dynamic_states;
};

} ///!	namespace vkr

#endif ///!	HEADER_GUARD
