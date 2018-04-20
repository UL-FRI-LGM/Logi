#include "base/PipelineState.h"

namespace vkr {

#pragma region Assembly state bits.
static const size_t   kPrimitiveTopologyOffset = 0, kPrimitiveTopologyNumBits = 4;
static const size_t kPrimitiveRestartEnableBit = 4;
#pragma endregion

#pragma region Tessellation state bits (Max number of patch control points currently does not exceed 32. State has max possible value 256 in order to be future proof.).
static const size_t kPatchControlPointsOffset = 5,  kPatchControlPointsNumBits = 8;
#pragma endregion

#pragma region Viewport and scissors state bits.
static const size_t		kViewportCountOffset = 13, kViewportCountNumBits = 4;
static const size_t		kScissorsCountOffset = 17, kScissorsCountNumBits = 4;
#pragma endregion

#pragma region Rasterizer state bits.
static const size_t		kDepthClampEnableBit = 21;
static const size_t		kRasterizerDiscardEnabledBit = 22;
static const size_t		kPolygonModeOffset = 23, kPolygonModeNumBits = 2;
static const size_t		kCullModeOffset = 25, kCullModeNumBits = 2;
static const size_t		kFrontFaceBit = 27;
static const size_t		kDepthBiasEnableBit = 28; // Dynamically set depth bias.
static const size_t		kDynamicStateLineWidth = 29;
#pragma endregion

#pragma region Multisample state bits (Can index up to 256 configurations).
static const size_t	kMultisampleStateIndexOffset = 30, kMultisampleStateIndexNumBits = 8;
#pragma endregion

#pragma region Depth state bits.
static const size_t kCompareOpNumBits = 3;
// Depth enable
static const size_t kDepthTestEnableBit = 38;
static const size_t kDepthStateBitsOffset = 39;
static const size_t kDepthStateNumBits = 5;
// Depth state
static const size_t kDepthWriteEnableBit = 0;
static const size_t kDepthCompareOpOffset = 1;
static const size_t kDepthBoundsTestEnableBit = 4; // Implicitly enable dynamic depth bounds set
#pragma endregion

#pragma region Stencil state bits.
// Stencil enable
static const size_t kStencilTestEnableBit = 44;
static const size_t kStencilStateBitsOffset = 45;
static const size_t kStencilStateNumBits = 72;
static const size_t kStencilOpNumBits = 3;
static const size_t kStencilValueNumBits = 8;
// Stencil front state
static const size_t      kStencilFrontFailOpOffset = 0;
static const size_t      kStencilFrontPassOpOffset = 3;
static const size_t kStencilFrontDepthFailOpOffset = 6;
static const size_t   kStencilFrontCompareOpOffset = 9;
static const size_t kStencilFrontCompareMaskOffset = 12;
static const size_t   kStencilFrontWriteMaskOffset = 20;
static const size_t   kStencilFrontReferenceOffset = 28;
// Stencil back state
static const size_t			  kStencilBackFailOpOffset = 36;
static const size_t				kStencilBackPassOpOffset = 39;
static const size_t	 kStencilBackDepthFailOpOffset = 42;
static const size_t    kStencilBackCompareOpOffset = 45;
static const size_t  kStencilBackCompareMaskOffset = 48;
static const size_t    kStencilBackWriteMaskOffset = 56;
static const size_t    kStencilBackReferenceOffset = 64;
#pragma endregion

#pragma region Color blend state bits.
static const size_t kBlendLogicOpEnableBit = 117;
static const size_t kBlendLogicOpOffset = 118,	kBlendLogicOpNumBits = 4;
static const size_t kDynamicBlendConstants = 122;
static const size_t kNumAttachmentsOffset = 123, kNumAttachmentsNumBits = 4;

static const size_t kColorBlendAttachmentsOffset = 127;
// Attachment config
static const size_t		  kAttachmentStateSize = 37;
static const size_t            kBlendEnableBit = 0;
static const size_t kSrcColorBlendFactorOffset = 1,		kSrcColorBlendFactorNumBits = 5;
static const size_t kDstColorBlendFactorOffset = 6,		kDstColorBlendFactorNumBits = 5;
static const size_t        kColorBlendOpOffset = 11,	kColorBlendOpNumBits = 6;
static const size_t kSrcAlphaBlendFactorOffset = 17,	kSrcAlphaBlendFactorNumBits = 5;
static const size_t kDstAlphaBlendFactorOffset = 22,	kDstAlphaBlendFactorNumBits = 5;
static const size_t        kAlphaBlendOpOffset = 27,	kAlphaBlendOpNumBits = 6;
static const size_t           kBlendMaskRedBit = 33;
static const size_t         kBlendMaskGreenBit = 34;
static const size_t          kBlendMaskBlueBit = 35;
static const size_t				  kBlendMaskAlphaBit = 36;
#pragma endregion

#pragma region Encoding/Decoding functions.
inline uint64_t primitiveTopologyToBits(vk::PrimitiveTopology topology) {
	switch (topology) {
		case vk::PrimitiveTopology::ePointList:						return 0u;	break;
		case vk::PrimitiveTopology::eLineList:						return 1u;	break;
		case vk::PrimitiveTopology::eLineStrip:						return 2u;	break;
		case vk::PrimitiveTopology::eTriangleList:					return 3u;	break;
		case vk::PrimitiveTopology::eTriangleStrip:					return 4u;	break;
		case vk::PrimitiveTopology::eTriangleFan:					return 5u;	break;
		case vk::PrimitiveTopology::eLineListWithAdjacency:			return 6u;	break;
		case vk::PrimitiveTopology::eLineStripWithAdjacency:		return 7u;	break;
		case vk::PrimitiveTopology::eTriangleListWithAdjacency:		return 8u;	break;
		case vk::PrimitiveTopology::eTriangleStripWithAdjacency:	return 9u;	break;
		case vk::PrimitiveTopology::ePatchList:						return 10u; break;

		default: throw std::runtime_error("Tried to encode unsupported PrimitiveTopology."); break;
	}
}

inline vk::PrimitiveTopology bitsToPrimitiveTopology(uint64_t bits) {
	switch (bits) {
		case 0u:		return vk::PrimitiveTopology::ePointList;					break;
		case 1u:		return vk::PrimitiveTopology::eLineList;					break;
		case 2u:		return vk::PrimitiveTopology::eLineStrip;					break;
		case 3u:		return vk::PrimitiveTopology::eTriangleList;				break;
		case 4u:		return vk::PrimitiveTopology::eTriangleStrip;				break;
		case 5u:		return vk::PrimitiveTopology::eTriangleFan;					break;
		case 6u:		return vk::PrimitiveTopology::eLineListWithAdjacency;		break;
		case 7u:		return vk::PrimitiveTopology::eLineStripWithAdjacency;		break;
		case 8u:		return vk::PrimitiveTopology::eTriangleListWithAdjacency;	break;
		case 9u:		return vk::PrimitiveTopology::eTriangleStripWithAdjacency;	break;
		case 10u:		return vk::PrimitiveTopology::ePatchList;					break;

		default: throw std::runtime_error("Failed to decode PrimitiveTopology."); break;
	}
}

inline uint64_t polygonModeToBits(vk::PolygonMode polymode) {
	switch (polymode) {
		case vk::PolygonMode::eFill:				return 0u; break;
		case vk::PolygonMode::eLine:				return 1u; break;
		case vk::PolygonMode::ePoint:				return 2u; break;
		case vk::PolygonMode::eFillRectangleNV:		return 3u; break;

		default: throw std::runtime_error("Tried to encode unsupported PolygonMode."); break;
	}
}

inline vk::PolygonMode bitsToPolygonMode(uint64_t bits) {
	switch (bits) {
		case 0u:	return vk::PolygonMode::eFill;				break;
		case 1u:	return vk::PolygonMode::eLine;				break;
		case 2u:	return vk::PolygonMode::ePoint;				break;
		case 3u:	return vk::PolygonMode::eFillRectangleNV;	break;

		default: throw std::runtime_error("Failed to decode PolygonMode."); break;
	}
}

inline uint64_t cullModeFlagToBits(vk::CullModeFlagBits flag) {
	switch (flag) {
		case vk::CullModeFlagBits::eNone:			return 0u; break;
		case vk::CullModeFlagBits::eFront:			return 1u; break;
		case vk::CullModeFlagBits::eBack:			return 2u; break;
		case vk::CullModeFlagBits::eFrontAndBack:	return 3u; break;

		default: throw std::runtime_error("Tried to encode unsupported CullModeFlag."); break;
	}
}

inline vk::CullModeFlagBits bitsToCullModeFlag(uint64_t bits) {
	switch (bits) {
		case 0u: return vk::CullModeFlagBits::eNone;			break;
		case 1u: return vk::CullModeFlagBits::eFront;			break;
		case 2u: return vk::CullModeFlagBits::eBack;			break;
		case 3u: return vk::CullModeFlagBits::eFrontAndBack;	break;

		default: throw std::runtime_error("Failed to decode CullModeFlag."); break;
	}
}

inline uint64_t compareOpToBits(vk::CompareOp op) {
	switch (op) {
		case vk::CompareOp::eNever:				return 0u; break;
		case vk::CompareOp::eLess:				return 1u; break;
		case vk::CompareOp::eEqual:				return 2u; break;
		case vk::CompareOp::eLessOrEqual:		return 3u; break;
		case vk::CompareOp::eGreater:			return 4u; break;
		case vk::CompareOp::eNotEqual:			return 5u; break;
		case vk::CompareOp::eGreaterOrEqual:	return 6u; break;
		case vk::CompareOp::eAlways:			return 7u; break;

		default: throw std::runtime_error("Tried to encode unsupported CompareOp."); break;
	}
}

inline vk::CompareOp bitsToCompareOp(uint64_t bits) {
	switch (bits) {
		case 0u: return vk::CompareOp::eNever;			break;
		case 1u: return vk::CompareOp::eLess;			break;
		case 2u: return vk::CompareOp::eEqual;			break;
		case 3u: return vk::CompareOp::eLessOrEqual;	break;
		case 4u: return vk::CompareOp::eGreater;		break;
		case 5u: return vk::CompareOp::eNotEqual;		break;
		case 6u: return vk::CompareOp::eGreaterOrEqual;	break;
		case 7u: return vk::CompareOp::eAlways;			break;

		default: throw std::runtime_error("Failed to decode CompareOp."); break;
	}
}

inline uint64_t stencilOpToBits(vk::StencilOp op) {
	switch (op) {
		case vk::StencilOp::eKeep:				return 0u; break;
		case vk::StencilOp::eZero:				return 1u; break;
		case vk::StencilOp::eReplace:			return 2u; break;
		case vk::StencilOp::eIncrementAndClamp:	return 3u; break;
		case vk::StencilOp::eDecrementAndClamp:	return 4u; break;
		case vk::StencilOp::eInvert:			return 5u; break;
		case vk::StencilOp::eIncrementAndWrap:	return 6u; break;
		case vk::StencilOp::eDecrementAndWrap:	return 7u; break;

		default: throw std::runtime_error("Tried to encode unsupported StencilOp."); break;
	}
}

inline vk::StencilOp bitsToStencilOp(uint64_t bits) {
	switch (bits) {
		case 0u:	return vk::StencilOp::eKeep;				break;
		case 1u:	return vk::StencilOp::eZero;				break;
		case 2u:	return vk::StencilOp::eReplace;				break;
		case 3u:	return vk::StencilOp::eIncrementAndClamp;	break;
		case 4u:	return vk::StencilOp::eDecrementAndClamp;	break;
		case 5u:	return vk::StencilOp::eInvert;				break;
		case 6u:	return vk::StencilOp::eIncrementAndWrap;	break;
		case 7u:	return vk::StencilOp::eDecrementAndWrap;	break;

		default: throw std::runtime_error("Failed to decode StencilOp."); break;
	}
}

inline uint64_t logicOpToBits(vk::LogicOp op) {
	switch (op) {
		case vk::LogicOp::eClear:			return 0u;	break;
		case vk::LogicOp::eAnd:				return 1u;	break;
		case vk::LogicOp::eAndReverse:		return 2u;	break;
		case vk::LogicOp::eCopy:			return 3u;	break;
		case vk::LogicOp::eAndInverted:		return 4u;	break;
		case vk::LogicOp::eNoOp:			return 5u;	break;
		case vk::LogicOp::eXor:				return 6u;	break;
		case vk::LogicOp::eOr:				return 7u;	break;
		case vk::LogicOp::eNor:				return 8u;	break;
		case vk::LogicOp::eEquivalent:		return 9u;	break;
		case vk::LogicOp::eInvert:			return 10u; break;
		case vk::LogicOp::eOrReverse:		return 11u; break;
		case vk::LogicOp::eCopyInverted:	return 12u; break;
		case vk::LogicOp::eOrInverted:		return 13u; break;
		case vk::LogicOp::eNand:			return 14u; break;
		case vk::LogicOp::eSet:				return 15u;	break;

		default: throw std::runtime_error("Tried to encode unsupported LogicOp."); break;
	}
}

inline vk::LogicOp bitsToLogicOp(uint64_t bits) {
	switch (bits) {
		case 0u:	return vk::LogicOp::eClear;			break;
		case 1u:	return vk::LogicOp::eAnd;			break;
		case 2u:	return vk::LogicOp::eAndReverse;	break;
		case 3u:	return vk::LogicOp::eCopy;			break;
		case 4u:	return vk::LogicOp::eAndInverted;	break;
		case 5u:	return vk::LogicOp::eNoOp;			break;
		case 6u:	return vk::LogicOp::eXor;			break;
		case 7u:	return vk::LogicOp::eOr;			break;
		case 8u:	return vk::LogicOp::eNor;			break;
		case 9u:	return vk::LogicOp::eEquivalent;	break;
		case 10u:	return vk::LogicOp::eInvert;		break;
		case 11u:	return vk::LogicOp::eOrReverse;		break;
		case 12u:	return vk::LogicOp::eCopyInverted;	break;
		case 13u:	return vk::LogicOp::eOrInverted;	break;
		case 14u:	return vk::LogicOp::eNand;			break;
		case 15u:	return vk::LogicOp::eSet;			break;

		default: throw std::runtime_error("Failed to decode LogicOp."); break;
	}
}

inline uint64_t blendFactorToBits(vk::BlendFactor factor) {
	switch (factor) {
		case vk::BlendFactor::eZero:					return 0u;	break;
		case vk::BlendFactor::eOne:						return 1u;	break;
		case vk::BlendFactor::eSrcColor:				return 2u;	break;
		case vk::BlendFactor::eOneMinusSrcColor:		return 3u;	break;
		case vk::BlendFactor::eDstColor:				return 4u;	break;
		case vk::BlendFactor::eOneMinusDstColor:		return 5u;	break;
		case vk::BlendFactor::eSrcAlpha:				return 6u;	break;
		case vk::BlendFactor::eOneMinusSrcAlpha:		return 7u;	break;
		case vk::BlendFactor::eDstAlpha:				return 8u;	break;
		case vk::BlendFactor::eOneMinusDstAlpha:		return 9u;	break;
		case vk::BlendFactor::eConstantColor:			return 10u; break;
		case vk::BlendFactor::eOneMinusConstantColor:	return 11u; break;
		case vk::BlendFactor::eConstantAlpha:			return 12u; break;
		case vk::BlendFactor::eOneMinusConstantAlpha:	return 13u; break;
		case vk::BlendFactor::eSrcAlphaSaturate:		return 14u; break;
		case vk::BlendFactor::eSrc1Color:				return 15u; break;
		case vk::BlendFactor::eOneMinusSrc1Color:		return 16u; break;
		case vk::BlendFactor::eSrc1Alpha:				return 17u; break;
		case vk::BlendFactor::eOneMinusSrc1Alpha:		return 18u; break;

		default: throw std::runtime_error("Tried to encode unsupported BlendFactor."); break;
	}
}

inline vk::BlendFactor bitsToBlendFactor(uint64_t bits) {
	switch (bits) {
		case  0u:	return vk::BlendFactor::eZero;					break;
		case  1u:	return vk::BlendFactor::eOne;					break;
		case  2u:	return vk::BlendFactor::eSrcColor;				break;
		case  3u:	return vk::BlendFactor::eOneMinusSrcColor;		break;
		case  4u:	return vk::BlendFactor::eDstColor;				break;
		case  5u:	return vk::BlendFactor::eOneMinusDstColor;		break;
		case  6u:	return vk::BlendFactor::eSrcAlpha;				break;
		case  7u:	return vk::BlendFactor::eOneMinusSrcAlpha;		break;
		case  8u:	return vk::BlendFactor::eDstAlpha;				break;
		case  9u:	return vk::BlendFactor::eOneMinusDstAlpha;		break;
		case 10u:	return vk::BlendFactor::eConstantColor;			break;
		case 11u:	return vk::BlendFactor::eOneMinusConstantColor;	break;
		case 12u:	return vk::BlendFactor::eConstantAlpha;			break;
		case 13u:	return vk::BlendFactor::eOneMinusConstantAlpha;	break;
		case 14u:	return vk::BlendFactor::eSrcAlphaSaturate;		break;
		case 15u:	return vk::BlendFactor::eSrc1Color;				break;
		case 16u:	return vk::BlendFactor::eOneMinusSrc1Color;		break;
		case 17u:	return vk::BlendFactor::eSrc1Alpha;				break;
		case 18u:	return vk::BlendFactor::eOneMinusSrc1Alpha;		break;

		default: throw std::runtime_error("Failed to decode BlendFactor."); break;
	}
}


inline uint64_t blendOpToBits(vk::BlendOp op) {
	switch (op) {
		case vk::BlendOp::eAdd:			return 0u;	break;
		case vk::BlendOp::eSubtract:	return 1u;	break;
		case vk::BlendOp::eMin:			return 2u;	break;
		case vk::BlendOp::eMax:			return 3u;	break;

		default: throw std::runtime_error("Tried to encode unsupported BlendOp."); break;
	}
}

inline vk::BlendOp bitsToBlendOp(uint64_t bits) {
	switch (bits) {
		case 0u: return vk::BlendOp::eAdd;		break;
		case 1u: return vk::BlendOp::eSubtract;	break;
		case 2u: return vk::BlendOp::eMin;		break;
		case 3u: return vk::BlendOp::eMax;		break;

		default: throw std::runtime_error("Failed to decode BlendOp."); break;
	}
}

#pragma endregion

// Default constructor
PipelineState::PipelineState() : state_(0), hash_fn_(), cached_hash_(0), dirty_(true), attachment_states_(kMaxAttachments, {0}), blend_logic_op_(0), depth_state_(0), stencil_state_() { }

size_t PipelineState::computeHash() {
	if (dirty_) {
		cached_hash_ = hash_fn_(state_);
		dirty_ = false;
	}

	return cached_hash_;
}

bool PipelineState::operator==(const PipelineState &other) const {
	return state_ == other.state_;
}

#pragma region Assembly state setters/getters

void PipelineState::setPrimitiveTopology(vk::PrimitiveTopology topology) {
	std::bitset<kPrimitiveTopologyNumBits> bits{ primitiveTopologyToBits(topology) };
	setBits(bits, state_, kPrimitiveTopologyOffset);
	dirty_ = true;
}

void PipelineState::setPrimitiveRestartEnable(bool enable) {
	state_[kPrimitiveRestartEnableBit] = enable;
	dirty_ = true;
}


vk::PrimitiveTopology PipelineState::getPrimitiveTopology() const {
	return bitsToPrimitiveTopology(getBits<kPrimitiveTopologyNumBits>(state_, kPrimitiveTopologyOffset).to_ulong());
}

bool PipelineState::getPrimitiveRestartEnable() const {
	return state_[kPrimitiveRestartEnableBit];
}

#pragma endregion

#pragma region Tessellation state setters/getters

void PipelineState::setPatchControlPoints(uint8_t count) {
	std::bitset<kPatchControlPointsNumBits> bits{ count };
	setBits(bits, state_, kPatchControlPointsOffset);
	dirty_ = true;
}


uint8_t PipelineState::getPatchControlPoints() const {
	return static_cast<uint8_t>(getBits<kPatchControlPointsNumBits>(state_, kPatchControlPointsOffset).to_ulong());
}

#pragma endregion

#pragma region Viewport and scissors state setters/getters

void PipelineState::setViewportCount(uint8_t count) {
	std::bitset<kViewportCountNumBits> bits{ count };
	setBits(bits, state_, kViewportCountOffset);
	dirty_ = true;
}

void PipelineState::setScissorsCount(uint8_t count) {
	std::bitset<kScissorsCountNumBits> bits{ count };
	setBits(bits, state_, kScissorsCountOffset);
	dirty_ = true;
}


uint8_t PipelineState::getViewportCount() const {
	return static_cast<uint8_t>(getBits<kViewportCountNumBits>(state_, kViewportCountOffset).to_ulong());
}

uint8_t PipelineState::getScissorsCount() const {
	return static_cast<uint8_t>(getBits<kScissorsCountNumBits>(state_, kScissorsCountOffset).to_ulong());
}

#pragma endregion

#pragma region Rasterizer state setters/getters

void PipelineState::setDepthClampEnabled(bool enable) {
	state_[kDepthClampEnableBit] = enable;
	dirty_ = true;
}

void PipelineState::setRasterizerDiscardEnabled(bool enable) {
	state_[kRasterizerDiscardEnabledBit] = enable;
	dirty_ = true;
}

void PipelineState::setPolygonMode(vk::PolygonMode poly_mode) {
	std::bitset<kPolygonModeNumBits> bits{ polygonModeToBits(poly_mode) };
	setBits(bits, state_, kPolygonModeOffset);
	dirty_ = true;
}

void PipelineState::setCullMode(vk::CullModeFlagBits cull_mode) {
	std::bitset<kCullModeNumBits> bits{ cullModeFlagToBits(cull_mode) };
	setBits(bits, state_, kCullModeOffset);
	dirty_ = true;
}

void PipelineState::setFrontFace(vk::FrontFace front_face) {
	state_[kFrontFaceBit] = front_face == vk::FrontFace::eCounterClockwise;
	dirty_ = true;
}

void PipelineState::setDepthBiasEnable(bool enable) {
	state_[kDepthBiasEnableBit] = enable;
	dirty_ = true;
}

void PipelineState::setDynamicLineWidth(bool enable) {
	state_[kDynamicStateLineWidth] = enable;
	dirty_ = true;
}


bool PipelineState::getDepthClampEnabled() const {
	return state_[kDepthClampEnableBit];
}

bool PipelineState::getRasterizerDiscardEnabled() const {
	return state_[kRasterizerDiscardEnabledBit];
}

vk::PolygonMode PipelineState::getPolygonMode() const {
	return bitsToPolygonMode(getBits<kPolygonModeNumBits>(state_, kPolygonModeOffset).to_ulong());
}

vk::CullModeFlagBits PipelineState::getCullMode() const {
	return bitsToCullModeFlag(getBits<kCullModeNumBits>(state_, kCullModeOffset).to_ulong());
}

vk::FrontFace PipelineState::getFrontFace() const {
	return (state_[kFrontFaceBit]) ? vk::FrontFace::eCounterClockwise : vk::FrontFace::eClockwise;
}

bool PipelineState::getDepthBiasEnable() const {
	return state_[kDepthBiasEnableBit];
}

bool PipelineState::getDynamicLineWidth() const {
	return state_[kDynamicStateLineWidth];
}

#pragma endregion

#pragma region Multisample state setters/getters

void PipelineState::setMultisampleStateIndex(uint8_t index) {
	std::bitset<kMultisampleStateIndexNumBits> bits{ index };
	setBits(bits, state_, kMultisampleStateIndexOffset);
	dirty_ = true;
}

uint8_t PipelineState::getMultisampleStateIndex() const {
	return static_cast<uint8_t>(getBits<kMultisampleStateIndexNumBits>(state_, kMultisampleStateIndexOffset).to_ulong());
}

#pragma endregion

#pragma region Depth state setters/getters

void PipelineState::setDepthTestEnable(bool enable) {
	state_[kDepthTestEnableBit] = enable;

	// Copy or reset the remaining depth test configuration.
	if (enable) {
		setBits(depth_state_, state_, kDepthStateBitsOffset);
	}
	else {
		std::bitset<kDepthStateNumBits> bits{ 0 };
		setBits(bits, state_, kDepthStateBitsOffset);
	}
	dirty_ = true;
}

void PipelineState::setDepthWriteEnable(bool enable) {
	depth_state_[kDepthWriteEnableBit] = enable;

	// If depth testing enabled apply configuration also to main state.
	if (getDepthTestEnable()) {
		state_[kDepthStateBitsOffset + kDepthWriteEnableBit] = depth_state_[kDepthWriteEnableBit];
		dirty_ = true;
	}
}

void PipelineState::setDepthCompareOp(vk::CompareOp op) {
	std::bitset<kCompareOpNumBits> bits{ compareOpToBits(op) };
	setBits(bits, depth_state_, kDepthCompareOpOffset);

	// If depth testing enabled apply configuration also to main state.
	if (getDepthTestEnable()) {
		setBits(bits, state_, kDepthStateBitsOffset + kDepthCompareOpOffset);
		dirty_ = true;
	}
}

void PipelineState::setDepthBoundsTestEnable(bool enable) {
	depth_state_[kDepthBoundsTestEnableBit] = enable;

	// If depth testing enabled apply configuration also to main state.
	if (getDepthTestEnable()) {
		state_[kDepthStateBitsOffset + kDepthBoundsTestEnableBit] = depth_state_[kDepthBoundsTestEnableBit];
		dirty_ = true;
	}
}

bool PipelineState::getDepthTestEnable() const {
	return state_[kDepthTestEnableBit];
}

bool PipelineState::getDepthWriteEnable() const {
	return depth_state_[kDepthWriteEnableBit];
}

vk::CompareOp PipelineState::getDepthCompareOp() const {
	return bitsToCompareOp(getBits<kCompareOpNumBits>(depth_state_, kDepthCompareOpOffset).to_ulong());
}

bool PipelineState::getDepthBoundsTestEnable() const {
	return depth_state_[kDepthBoundsTestEnableBit];
}

#pragma endregion

#pragma region Stencil state setters/getters

void PipelineState::setStencilTestEnable(bool enable) {
	state_[kStencilTestEnableBit] = enable;

	// Copy or reset the remaining depth test configuration.
	if (enable) {
		setBits(stencil_state_, state_, kStencilStateBitsOffset);
	}
	else {
		std::bitset<kStencilStateNumBits> bits{ 0 };
		setBits(bits, state_, kStencilStateBitsOffset);
	}
	dirty_ = true;
}

void PipelineState::setStencilFailOp(StencilState ss, vk::StencilOp op) {
	std::bitset<kStencilOpNumBits> bits{ stencilOpToBits(op) };

	if (ss == StencilState::FRONT) {
		setBits(bits, stencil_state_, kStencilFrontFailOpOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilFrontFailOpOffset);
			dirty_ = true;
		}
	}
	else if (ss == StencilState::BACK) {
		setBits(bits, stencil_state_, kStencilBackFailOpOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilBackFailOpOffset);
			dirty_ = true;
		}
	}
}

void PipelineState::setStencilPassOp(StencilState ss, vk::StencilOp op) {
	std::bitset<kStencilOpNumBits> bits{ stencilOpToBits(op) };

	if (ss == StencilState::FRONT) {
		setBits(bits, stencil_state_, kStencilFrontPassOpOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilFrontPassOpOffset);
			dirty_ = true;
		}
	}
	else if (ss == StencilState::BACK) {
		setBits(bits, stencil_state_, kStencilBackPassOpOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilBackPassOpOffset);
			dirty_ = true;
		}
	}
}

void PipelineState::setStencilDepthFailOp(StencilState ss, vk::StencilOp op) {
	std::bitset<kStencilOpNumBits> bits{ stencilOpToBits(op) };

	if (ss == StencilState::FRONT) {
		setBits(bits, stencil_state_, kStencilFrontDepthFailOpOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilFrontDepthFailOpOffset);
			dirty_ = true;
		}
	}
	else if (ss == StencilState::BACK) {
		setBits(bits, stencil_state_, kStencilBackDepthFailOpOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilBackDepthFailOpOffset);
			dirty_ = true;
		}
	}
}

void PipelineState::setStencilCompareOp(StencilState ss, vk::CompareOp op) {
	std::bitset<kCompareOpNumBits> bits{ compareOpToBits(op) };

	if (ss == StencilState::FRONT) {
		setBits(bits, stencil_state_, kStencilFrontCompareOpOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilFrontCompareOpOffset);
			dirty_ = true;
		}
	}
	else if (ss == StencilState::BACK) {
		setBits(bits, stencil_state_, kStencilBackCompareOpOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilBackCompareOpOffset);
			dirty_ = true;
		}
	}
}

void PipelineState::setStencilCompareMask(StencilState ss, uint8_t mask) {
	std::bitset<kStencilValueNumBits> bits{ mask };

	if (ss == StencilState::FRONT) {
		setBits(bits, stencil_state_, kStencilFrontCompareMaskOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilFrontCompareMaskOffset);
			dirty_ = true;
		}
	}
	else if (ss == StencilState::BACK) {
		setBits(bits, stencil_state_, kStencilBackCompareMaskOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilBackCompareMaskOffset);
			dirty_ = true;
		}
	}
}

void PipelineState::setStencilWriteMask(StencilState ss, uint8_t mask) {
	std::bitset<kStencilValueNumBits> bits{ mask };

	if (ss == StencilState::FRONT) {
		setBits(bits, stencil_state_, kStencilFrontWriteMaskOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilFrontWriteMaskOffset);
			dirty_ = true;
		}
	}
	else if (ss == StencilState::BACK) {
		setBits(bits, stencil_state_, kStencilBackWriteMaskOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilBackWriteMaskOffset);
			dirty_ = true;
		}
	}
}

void PipelineState::setStencilReference(StencilState ss, uint8_t ref) {
	std::bitset<kStencilValueNumBits> bits{ ref };

	if (ss == StencilState::FRONT) {
		setBits(bits, stencil_state_, kStencilFrontReferenceOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilFrontReferenceOffset);
			dirty_ = true;
		}
	}
	else if (ss == StencilState::BACK) {
		setBits(bits, stencil_state_, kStencilBackReferenceOffset);
		if (getStencilTestEnable()) {
			setBits(bits, state_, kStencilStateBitsOffset + kStencilBackReferenceOffset);
			dirty_ = true;
		}
	}
}


bool PipelineState::getStencilTestEnable() const {
	return state_[kStencilTestEnableBit];
}

vk::StencilOp PipelineState::getStencilFailOp(StencilState ss) const {
	if (ss == StencilState::FRONT) {
		return bitsToStencilOp(getBits<kStencilOpNumBits>(stencil_state_, kStencilFrontFailOpOffset).to_ulong());
	}
	else if (ss == StencilState::BACK) {
		return bitsToStencilOp(getBits<kStencilOpNumBits>(stencil_state_, kStencilBackFailOpOffset).to_ulong());
	}

	throw std::runtime_error("Unknown stencil state.");
}

vk::StencilOp PipelineState::getStencilPassOp(StencilState ss) const {
	if (ss == StencilState::FRONT) {
		return bitsToStencilOp(getBits<kStencilOpNumBits>(stencil_state_, kStencilFrontPassOpOffset).to_ulong());
	}
	else if (ss == StencilState::BACK) {
		return bitsToStencilOp(getBits<kStencilOpNumBits>(stencil_state_, kStencilBackPassOpOffset).to_ulong());
	}

	throw std::runtime_error("Unknown stencil state.");
}

vk::StencilOp PipelineState::getStencilDepthFailOp(StencilState ss) const {
	if (ss == StencilState::FRONT) {
		return bitsToStencilOp(getBits<kStencilOpNumBits>(stencil_state_, kStencilFrontDepthFailOpOffset).to_ulong());
	}
	else if (ss == StencilState::BACK) {
		return bitsToStencilOp(getBits<kStencilOpNumBits>(stencil_state_, kStencilBackDepthFailOpOffset).to_ulong());
	}

	throw std::runtime_error("Unknown stencil state.");
}

vk::CompareOp PipelineState::getStencilCompareOp(StencilState ss) const {
	if (ss == StencilState::FRONT) {
		return bitsToCompareOp(getBits<kCompareOpNumBits>(stencil_state_, kStencilFrontCompareOpOffset).to_ulong());
	}
	else if (ss == StencilState::BACK) {
		return bitsToCompareOp(getBits<kCompareOpNumBits>(stencil_state_, kStencilBackCompareOpOffset).to_ulong());
	}

	throw std::runtime_error("Unknown stencil state.");
}

uint8_t PipelineState::getStencilCompareMask(StencilState ss) const {
	if (ss == StencilState::FRONT) {
		return static_cast<uint8_t>(getBits<kStencilValueNumBits>(stencil_state_, kStencilFrontCompareMaskOffset).to_ulong());
	}
	else if (ss == StencilState::BACK) {
		return static_cast<uint8_t>(getBits<kStencilValueNumBits>(stencil_state_, kStencilBackCompareMaskOffset).to_ulong());
	}

	throw std::runtime_error("Unknown stencil state.");
}

uint8_t PipelineState::getStencilWriteMask(StencilState ss) const {
	if (ss == StencilState::FRONT) {
		return static_cast<uint8_t>(getBits<kStencilValueNumBits>(stencil_state_, kStencilFrontWriteMaskOffset).to_ulong());
	}
	else if (ss == StencilState::BACK) {
		return static_cast<uint8_t>(getBits<kStencilValueNumBits>(stencil_state_, kStencilBackWriteMaskOffset).to_ulong());
	}

	throw std::runtime_error("Unknown stencil state.");
}

uint8_t PipelineState::getStencilReference(StencilState ss) const {
	if (ss == StencilState::FRONT) {
		return static_cast<uint8_t>(getBits<kStencilValueNumBits>(stencil_state_, kStencilFrontReferenceOffset).to_ulong());
	}
	else if (ss == StencilState::BACK) {
		return static_cast<uint8_t>(getBits<kStencilValueNumBits>(stencil_state_, kStencilBackReferenceOffset).to_ulong());
	}

	throw std::runtime_error("Unknown stencil state.");
}

#pragma endregion

#pragma region Blending state setters/getters
void PipelineState::setBlendLogicOpEnable(bool enable) {
	state_[kBlendLogicOpEnableBit] = enable;

	if (enable) {
		setBits(blend_logic_op_, state_, kBlendLogicOpOffset);
	}
	dirty_ = true;
}

void PipelineState::setBlendLogicOp(vk::LogicOp op) {
	blend_logic_op_ = logicOpToBits(op);

	if (getBlendLogicOpEnable()) {
		setBits(blend_logic_op_, state_, kBlendLogicOpOffset);
		dirty_ = true;
	}
}

void PipelineState::setDynamicBlendConstants(bool enable) {
	state_[kDynamicBlendConstants] = enable;
	dirty_ = true;
}

void PipelineState::setNumAttachments(uint8_t num_attachments) {
	if (num_attachments > kMaxAttachments) {
		throw std::runtime_error("Invalid number of attachments.");
	}

	// Set number of attachments
	std::bitset<4> bits{ num_attachments };
	setBits(bits, state_, kNumAttachmentsOffset);

	std::bitset<kAttachmentStateSize> zero_bits{ 0u };

	// Copy attachment states.
	for (size_t i = 0; i < num_attachments; i++) {
		if (getBlendEnable(static_cast<uint8_t>(i))) {
			setBits(attachment_states_[i], state_, kColorBlendAttachmentsOffset + i * kAttachmentStateSize);
		}
		else {
			setBits(zero_bits, state_, kColorBlendAttachmentsOffset + i * kAttachmentStateSize);
		}
	}

	// Reset bits of unused attachments
	for (size_t i = num_attachments; i < kMaxAttachments; i++) {
		setBits(zero_bits, state_, kColorBlendAttachmentsOffset + i * kAttachmentStateSize);
	}

	dirty_ = true;
}

void PipelineState::setBlendEnable(uint8_t attachment, bool enable) {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	attachment_states_[attachment][kBlendEnableBit] = enable;
	if (enable) {
		setBits(attachment_states_[attachment], state_, kColorBlendAttachmentsOffset + attachment * kAttachmentStateSize);
	}
	else {
		std::bitset<kAttachmentStateSize> zero_bits{ 0u };
		setBits(zero_bits, state_, kColorBlendAttachmentsOffset + attachment * kAttachmentStateSize);
	}

	dirty_ = true;
}

void PipelineState::setSrcColorBlendFactor(uint8_t attachment, vk::BlendFactor blend_factor) {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	std::bitset<kSrcColorBlendFactorNumBits> bits = blendFactorToBits(blend_factor);
	setBits(bits, attachment_states_[attachment], kSrcColorBlendFactorOffset);

	if (getBlendEnable(attachment)) {
		setBits(bits, state_, kColorBlendAttachmentsOffset + attachment * kAttachmentStateSize + kSrcColorBlendFactorOffset);
		dirty_ = true;
	}
}

void PipelineState::setDstColorBlendFactor(uint8_t attachment, vk::BlendFactor blend_factor) {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	std::bitset<kDstColorBlendFactorNumBits> bits = blendFactorToBits(blend_factor);
	setBits(bits, attachment_states_[attachment], kDstColorBlendFactorOffset);

	if (getBlendEnable(attachment)) {
		setBits(bits, state_, kColorBlendAttachmentsOffset + attachment * kAttachmentStateSize + kDstColorBlendFactorOffset);
		dirty_ = true;
	}
}

void PipelineState::setColorBlendOp(uint8_t attachment, vk::BlendOp blend_op) {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	std::bitset<kColorBlendOpNumBits> bits = blendOpToBits(blend_op);
	setBits(bits, attachment_states_[attachment], kColorBlendOpOffset);

	if (getBlendEnable(attachment)) {
		setBits(bits, state_, kColorBlendAttachmentsOffset + attachment * kAttachmentStateSize + kColorBlendOpOffset);
		dirty_ = true;
	}
}

void PipelineState::setSrcAlphaBlendFactor(uint8_t attachment, vk::BlendFactor blend_factor) {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	std::bitset<kSrcAlphaBlendFactorNumBits> bits = blendFactorToBits(blend_factor);
	setBits(bits, attachment_states_[attachment], kSrcAlphaBlendFactorOffset);

	if (getBlendEnable(attachment)) {
		setBits(bits, state_, kColorBlendAttachmentsOffset + attachment * kAttachmentStateSize + kSrcAlphaBlendFactorOffset);
		dirty_ = true;
	}
}

void PipelineState::setDstAlphaBlendFactor(uint8_t attachment, vk::BlendFactor blend_factor) {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	std::bitset<kDstAlphaBlendFactorNumBits> bits = blendFactorToBits(blend_factor);
	setBits(bits, attachment_states_[attachment], kDstAlphaBlendFactorOffset);

	if (getBlendEnable(attachment)) {
		setBits(bits, state_, kColorBlendAttachmentsOffset + attachment * kAttachmentStateSize + kDstAlphaBlendFactorOffset);
		dirty_ = true;
	}
}

void PipelineState::setAlphaBlendOp(uint8_t attachment, vk::BlendOp blend_op) {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	std::bitset<kAlphaBlendOpNumBits> bits = blendOpToBits(blend_op);
	setBits(bits, attachment_states_[attachment], kAlphaBlendOpOffset);

	if (getBlendEnable(attachment)) {
		setBits(bits, state_, kColorBlendAttachmentsOffset + attachment * kAttachmentStateSize + kAlphaBlendOpOffset);
		dirty_ = true;
	}
}

void PipelineState::setBlendMaskRed(uint8_t attachment, bool enable) {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	attachment_states_[attachment][kBlendMaskRedBit] = enable;
	if (getBlendEnable(attachment)) {
		state_[kColorBlendAttachmentsOffset + attachment * kAttachmentStateSize + kBlendMaskRedBit] = enable;
		dirty_ = true;
	}
}

void PipelineState::setBlendMaskGreen(uint8_t attachment, bool enable) {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	attachment_states_[attachment][kBlendMaskGreenBit] = enable;
	if (getBlendEnable(attachment)) {
		state_[kColorBlendAttachmentsOffset + attachment * kAttachmentStateSize + kBlendMaskGreenBit] = enable;
		dirty_ = true;
	}
}

void PipelineState::setBlendMaskBlue(uint8_t attachment, bool enable) {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	attachment_states_[attachment][kBlendMaskBlueBit] = enable;
	if (getBlendEnable(attachment)) {
		state_[kColorBlendAttachmentsOffset + attachment * kAttachmentStateSize + kBlendMaskBlueBit] = enable;
		dirty_ = true;
	}
}

void PipelineState::setBlendMaskAlpha(uint8_t attachment, bool enable) {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	attachment_states_[attachment][kBlendMaskAlphaBit] = enable;
	if (getBlendEnable(attachment)) {
		state_[kColorBlendAttachmentsOffset + attachment * kAttachmentStateSize + kBlendMaskAlphaBit] = enable;
		dirty_ = true;
	}
}


bool PipelineState::getBlendLogicOpEnable() const {
	return state_[kBlendLogicOpEnableBit];
}

vk::LogicOp PipelineState::getBlendLogicOp() const {
	return bitsToLogicOp(blend_logic_op_.to_ulong());
}

bool PipelineState::getDynamicBlendConstants() const {
	return state_[kDynamicBlendConstants];
}

uint8_t PipelineState::getNumAttachments() const {
	return static_cast<uint8_t>(getBits<kNumAttachmentsNumBits>(state_, kNumAttachmentsOffset).to_ulong());
}

bool PipelineState::getBlendEnable(uint8_t attachment) const {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	return attachment_states_[attachment][kBlendEnableBit];
}

vk::BlendFactor PipelineState::getSrcColorBlendFactor(uint8_t attachment) const {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	return bitsToBlendFactor(getBits<kSrcColorBlendFactorNumBits>(attachment_states_[attachment], kSrcColorBlendFactorOffset).to_ulong());
}

vk::BlendFactor PipelineState::getDstColorBlendFactor(uint8_t attachment) const {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	return bitsToBlendFactor(getBits<kDstColorBlendFactorNumBits>(attachment_states_[attachment], kDstColorBlendFactorOffset).to_ulong());
}

vk::BlendOp PipelineState::getColorBlendOp(uint8_t attachment) const {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	return bitsToBlendOp(getBits<kColorBlendOpNumBits>(attachment_states_[attachment], kColorBlendOpOffset).to_ulong());
}

vk::BlendFactor PipelineState::getSrcAlphaBlendFactor(uint8_t attachment) const {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	return bitsToBlendFactor(getBits<kSrcAlphaBlendFactorNumBits>(attachment_states_[attachment], kSrcAlphaBlendFactorOffset).to_ulong());
}

vk::BlendFactor PipelineState::getDstAlphaBlendFactor(uint8_t attachment) const {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	return bitsToBlendFactor(getBits<kDstAlphaBlendFactorNumBits>(attachment_states_[attachment], kDstAlphaBlendFactorOffset).to_ulong());
}

vk::BlendOp PipelineState::getAlphaBlendOp(uint8_t attachment) const {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	return bitsToBlendOp(getBits<kAlphaBlendOpNumBits>(attachment_states_[attachment], kAlphaBlendOpOffset).to_ulong());
}

bool PipelineState::getBlendMaskRed(uint8_t attachment) const {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	return attachment_states_[attachment][kBlendMaskRedBit];
}

bool PipelineState::getBlendMaskGreen(uint8_t attachment) const {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	return attachment_states_[attachment][kBlendMaskGreenBit];
}

bool PipelineState::getBlendMaskBlue(uint8_t attachment) const {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	return attachment_states_[attachment][kBlendMaskBlueBit];
}

bool PipelineState::getBlendMaskAlpha(uint8_t attachment) const {
	if (attachment >= kMaxAttachments) {
		throw std::runtime_error("Attachment out of range.");
	}

	return attachment_states_[attachment][kBlendMaskAlphaBit];
}

vk::PipelineInputAssemblyStateCreateInfo PipelineState::getInputAssemblyState() const {
	return vk::PipelineInputAssemblyStateCreateInfo(vk::PipelineInputAssemblyStateCreateFlags(), getPrimitiveTopology(), getPrimitiveRestartEnable());
}

vk::PipelineTessellationStateCreateInfo PipelineState::getTesselationState() const {
	return vk::PipelineTessellationStateCreateInfo(vk::PipelineTessellationStateCreateFlags(), static_cast<uint32_t>(getPatchControlPoints()));
}

vk::PipelineViewportStateCreateInfo PipelineState::getViewportState() const {
	return vk::PipelineViewportStateCreateInfo(vk::PipelineViewportStateCreateFlags(), static_cast<uint32_t>(getViewportCount()), nullptr, static_cast<uint32_t>(getScissorsCount()), nullptr);
}

vk::PipelineRasterizationStateCreateInfo PipelineState::getRasterizationState() const {
	return vk::PipelineRasterizationStateCreateInfo(vk::PipelineRasterizationStateCreateFlags(), getDepthClampEnabled(), getRasterizerDiscardEnabled(), getPolygonMode(), getCullMode(), getFrontFace(), getDepthBiasEnable(), getBia)
}

vk::PipelineMultisampleStateCreateInfo PipelineState::getMultisampleState() const {

}

vk::PipelineDepthStencilStateCreateInfo PipelineState::getDepthStencilState() const {

}

vk::PipelineColorBlendStateCreateInfo PipelineState::getColorBlendState() const {

}

vk::PipelineDynamicStateCreateInfo PipelineState::getDynamicStateCreateInfo() const {

}

}
