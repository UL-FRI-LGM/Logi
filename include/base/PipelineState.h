#ifndef BASE_PIPELINE_STATE_H
#define BASE_PIPELINE_STATE_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include <bitset>

namespace vkr {

class PipelineState {
public:
	enum class StencilState {
		FRONT,
		BACK
	};

	PipelineState();

	size_t computeHash();

	bool operator==(const PipelineState &other) const;

	// Input assembly state setters/getters.
	void setPrimitiveTopology(vk::PrimitiveTopology topology);
	void setPrimitiveRestartEnable(bool enable);

	vk::PrimitiveTopology getPrimitiveTopology() const;
	bool getPrimitiveRestartEnable() const;

	// Tessellation state setters/getters.
	void setPatchControlPoints(uint8_t count);
	uint8_t getPatchControlPoints() const;

	// Viewport and scissors state setters/getters.
	void setViewportCount(uint8_t count);
	void setScissorsCount(uint8_t count);

	uint8_t getViewportCount() const;
	uint8_t getScissorsCount() const;

	// Rasterizer state setters/getters.
	void setDepthClampEnabled(bool enable);
	void setRasterizerDiscardEnabled(bool enable);
	void setPolygonMode(vk::PolygonMode poly_mode);
	void setCullMode(vk::CullModeFlagBits cull_mode);
	void setFrontFace(vk::FrontFace front_face);
	void setDepthBiasEnable(bool enable); // Note: if true depth bias values must be set dynamically.
	void setDynamicLineWidth(bool enable);

	bool getDepthClampEnabled() const;
	bool getRasterizerDiscardEnabled() const;
	vk::PolygonMode getPolygonMode() const;
	vk::CullModeFlagBits getCullMode() const;
	vk::FrontFace getFrontFace() const;
	bool getDepthBiasEnable() const;
	bool getDynamicLineWidth() const;

	// Multisampling state setters/getters.
	void setMultisampleStateIndex(uint8_t index);
	uint8_t getMultisampleStateIndex() const;

	// Depth state setters/getters.
	void setDepthTestEnable(bool enable);
	void setDepthWriteEnable(bool enable);
	void setDepthCompareOp(vk::CompareOp op);
	void setDepthBoundsTestEnable(bool enable);

	bool getDepthTestEnable() const;
	bool getDepthWriteEnable() const;
	vk::CompareOp getDepthCompareOp() const;
	bool getDepthBoundsTestEnable() const;

	// Stencil state setters/getters
	void setStencilTestEnable(bool enable);
	void setStencilFailOp(StencilState ss, vk::StencilOp op);
	void setStencilPassOp(StencilState ss, vk::StencilOp op);
	void setStencilDepthFailOp(StencilState ss, vk::StencilOp op);
	void setStencilCompareOp(StencilState ss, vk::CompareOp op);
	void setStencilCompareMask(StencilState ss, uint8_t mask);
	void setStencilWriteMask(StencilState ss, uint8_t mask);
	void setStencilReference(StencilState ss, uint8_t ref);

	bool getStencilTestEnable() const;
	vk::StencilOp getStencilFailOp(StencilState ss) const;
	vk::StencilOp getStencilPassOp(StencilState ss) const;
	vk::StencilOp getStencilDepthFailOp(StencilState ss) const;
	vk::CompareOp getStencilCompareOp(StencilState ss) const;
	uint8_t getStencilCompareMask(StencilState ss) const;
	uint8_t getStencilWriteMask(StencilState ss) const;
	uint8_t getStencilReference(StencilState ss) const;
	
	// Color blend state setters/getters
	void setBlendLogicOpEnable(bool enable);
	void setBlendLogicOp(vk::LogicOp op);
	void setDynamicBlendConstants(bool enable);
	void setNumAttachments(uint8_t num_attachments);
	void setBlendEnable(uint8_t attachment, bool enable);
	void setSrcColorBlendFactor(uint8_t attachment, vk::BlendFactor blend_factor);
	void setDstColorBlendFactor(uint8_t attachment, vk::BlendFactor blend_factor);
	void setColorBlendOp(uint8_t attachment, vk::BlendOp blend_op);
	void setSrcAlphaBlendFactor(uint8_t attachment, vk::BlendFactor blend_factor);
	void setDstAlphaBlendFactor(uint8_t attachment, vk::BlendFactor blend_factor);
	void setAlphaBlendOp(uint8_t attachment, vk::BlendOp blend_op);
	void setBlendMaskRed(uint8_t attachment, bool enable);
	void setBlendMaskGreen(uint8_t attachment, bool enable);
	void setBlendMaskBlue(uint8_t attachment, bool enable);
	void setBlendMaskAlpha(uint8_t attachment, bool enable);

	bool getBlendLogicOpEnable() const;
	vk::LogicOp getBlendLogicOp() const;
	bool getDynamicBlendConstants() const;
	uint8_t getNumAttachments() const;
	bool getBlendEnable(uint8_t attachment) const;
	vk::BlendFactor getSrcColorBlendFactor(uint8_t attachment) const;
	vk::BlendFactor getDstColorBlendFactor(uint8_t attachment) const;
	vk::BlendOp getColorBlendOp(uint8_t attachment) const;
	vk::BlendFactor getSrcAlphaBlendFactor(uint8_t attachment) const;
	vk::BlendFactor getDstAlphaBlendFactor(uint8_t attachment) const;
	vk::BlendOp getAlphaBlendOp(uint8_t attachment) const;
	bool getBlendMaskRed(uint8_t attachment) const;
	bool getBlendMaskGreen(uint8_t attachment) const;
	bool getBlendMaskBlue(uint8_t attachment) const;
	bool getBlendMaskAlpha(uint8_t attachment) const;

	vk::PipelineInputAssemblyStateCreateInfo getInputAssemblyState() const;

	vk::PipelineTessellationStateCreateInfo getTesselationState() const;

	vk::PipelineViewportStateCreateInfo getViewportState() const;

	vk::PipelineRasterizationStateCreateInfo getRasterizationState() const;

	vk::PipelineMultisampleStateCreateInfo getMultisampleState() const;

	vk::PipelineDepthStencilStateCreateInfo getDepthStencilState() const;

	vk::PipelineColorBlendStateCreateInfo getColorBlendState() const;

	vk::PipelineDynamicStateCreateInfo getDynamicStateCreateInfo() const;

private:
	// Num attachments.
	static const size_t kMaxAttachments = 8;

	std::bitset<127 + 8 * 37> state_;
	std::hash<std::bitset<127 + 8 * 37>> hash_fn_;
	size_t cached_hash_;
	bool dirty_;

	std::vector<std::bitset<37>> attachment_states_;
	std::bitset<4> blend_logic_op_;
	std::bitset<5> depth_state_;
	std::bitset<72> stencil_state_;
	
	template<size_t N, size_t K>
	static void setBits(const std::bitset<K>& src, std::bitset<N>& dst, size_t offset);

	template<size_t K, size_t N>
	static std::bitset<K> getBits(const std::bitset<N>& src, size_t offset);
};

template<size_t N, size_t K>
inline void PipelineState::setBits(const std::bitset<K>& src, std::bitset<N>& dst, size_t offset) {
	for (size_t i = 0; i < K; i++) {
		dst[offset + i] = src[i];
	}
}

template<size_t K, size_t N>
inline std::bitset<K> PipelineState::getBits(const std::bitset<N>& src, size_t offset) {
	std::bitset<K> bits{0};

	for (size_t i = 0; i < K; i++) {
		bits[i] = src[offset + i];
	}

	return bits;
}

}



#endif