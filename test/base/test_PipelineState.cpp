#include <gtest/gtest.h>
#include <vulkan/vulkan.hpp>
#include "memory/AllocationManager.h"

/* this is a test that can pass*/
TEST(AddTest, CanPass) {
	system("pause");

	/*
	vkr::PipelineState test_state;
	test_state.setPrimitiveTopology(vk::PrimitiveTopology::eTriangleFan);
	test_state.setPrimitiveRestartEnable(true);
	test_state.setPatchControlPoints(6);
	
	test_state.setViewportCount(2);
	test_state.setScissorsCount(7);
	
	test_state.setDepthClampEnabled(false);
	test_state.setRasterizerDiscardEnabled(true);
	test_state.setPolygonMode(vk::PolygonMode::eFillRectangleNV);
	test_state.setCullMode(vk::CullModeFlagBits::eFrontAndBack);
	test_state.setFrontFace(vk::FrontFace::eCounterClockwise);
	test_state.setDepthBiasEnable(true);
	test_state.setDynamicLineWidth(true);

	test_state.setMultisampleStateIndex(5);

	test_state.setDepthTestEnable(true);
	test_state.setDepthWriteEnable(false);
	test_state.setDepthCompareOp(vk::CompareOp::eGreaterOrEqual);
	test_state.setDepthBoundsTestEnable(true);

	test_state.setStencilTestEnable(true);
	test_state.setStencilFailOp(vkr::PipelineState::StencilState::FRONT, vk::StencilOp::eIncrementAndWrap);
	test_state.setStencilPassOp(vkr::PipelineState::StencilState::FRONT, vk::StencilOp::eDecrementAndWrap);
	test_state.setStencilDepthFailOp(vkr::PipelineState::StencilState::FRONT, vk::StencilOp::eInvert);
	test_state.setStencilCompareOp(vkr::PipelineState::StencilState::FRONT, vk::CompareOp::eNever);
	test_state.setStencilCompareMask(vkr::PipelineState::StencilState::FRONT, 1);
	test_state.setStencilWriteMask(vkr::PipelineState::StencilState::FRONT, 6);
	test_state.setStencilReference(vkr::PipelineState::StencilState::FRONT, 46);
	test_state.setStencilFailOp(vkr::PipelineState::StencilState::BACK, vk::StencilOp::eZero);
	test_state.setStencilPassOp(vkr::PipelineState::StencilState::BACK, vk::StencilOp::eKeep);
	test_state.setStencilDepthFailOp(vkr::PipelineState::StencilState::BACK, vk::StencilOp::eReplace);
	test_state.setStencilCompareOp(vkr::PipelineState::StencilState::BACK, vk::CompareOp::eEqual);
	test_state.setStencilCompareMask(vkr::PipelineState::StencilState::BACK, 13);
	test_state.setStencilWriteMask(vkr::PipelineState::StencilState::BACK, 230);
	test_state.setStencilReference(vkr::PipelineState::StencilState::BACK, 126);

	test_state.setBlendLogicOpEnable(true);
	test_state.setBlendLogicOp(vk::LogicOp::eAndReverse);
	test_state.setDynamicBlendConstants(true);
	test_state.setNumAttachments(6);

	test_state.setBlendEnable(1u, true);
	test_state.setSrcColorBlendFactor(1u, vk::BlendFactor::eConstantAlpha);
	test_state.setDstColorBlendFactor(1u, vk::BlendFactor::eConstantColor);
	test_state.setColorBlendOp(1u, vk::BlendOp::eAdd);
	test_state.setSrcAlphaBlendFactor(1u, vk::BlendFactor::eDstAlpha);
	test_state.setDstAlphaBlendFactor(1u, vk::BlendFactor::eDstColor);
	test_state.setAlphaBlendOp(1u, vk::BlendOp::eMax);
	test_state.setBlendMaskRed(1u, false);
	test_state.setBlendMaskGreen(1u, true);
	test_state.setBlendMaskBlue(1u, false);
	test_state.setBlendMaskAlpha(1u, true);

	test_state.setBlendEnable(4u, true);
	test_state.setSrcColorBlendFactor(4u, vk::BlendFactor::eOne);
	test_state.setDstColorBlendFactor(4u, vk::BlendFactor::eOneMinusConstantAlpha);
	test_state.setColorBlendOp(4u, vk::BlendOp::eMin);
	test_state.setSrcAlphaBlendFactor(4u, vk::BlendFactor::eOneMinusConstantColor);
	test_state.setDstAlphaBlendFactor(4u, vk::BlendFactor::eOneMinusDstAlpha);
	test_state.setAlphaBlendOp(4u, vk::BlendOp::eAdd);
	test_state.setBlendMaskRed(4u, true);
	test_state.setBlendMaskGreen(4u, false);
	test_state.setBlendMaskBlue(4u, true);
	test_state.setBlendMaskAlpha(4u, false);

	EXPECT_EQ(test_state.getPrimitiveTopology(), vk::PrimitiveTopology::eTriangleFan);
	EXPECT_EQ(test_state.getPrimitiveRestartEnable(), true);
	EXPECT_EQ(test_state.getPatchControlPoints(), 6);

	EXPECT_EQ(test_state.getViewportCount(), 2);
	EXPECT_EQ(test_state.getScissorsCount(), 7);

	EXPECT_EQ(test_state.getDepthClampEnabled(), false);
	EXPECT_EQ(test_state.getRasterizerDiscardEnabled(), true);
	EXPECT_EQ(test_state.getPolygonMode(), vk::PolygonMode::eFillRectangleNV);
	EXPECT_EQ(test_state.getCullMode(), vk::CullModeFlagBits::eFrontAndBack);
	EXPECT_EQ(test_state.getFrontFace(), vk::FrontFace::eCounterClockwise);
	EXPECT_EQ(test_state.getDepthBiasEnable(), true);
	EXPECT_EQ(test_state.getDynamicLineWidth(), true);

	EXPECT_EQ(test_state.getMultisampleStateIndex(), 5);

	EXPECT_EQ(test_state.getDepthTestEnable(), true);
	EXPECT_EQ(test_state.getDepthWriteEnable(), false);
	EXPECT_EQ(test_state.getDepthCompareOp(), vk::CompareOp::eGreaterOrEqual);
	EXPECT_EQ(test_state.getDepthBoundsTestEnable(), true);

	EXPECT_EQ(test_state.getStencilTestEnable(), true);
	EXPECT_EQ(test_state.getStencilFailOp(vkr::PipelineState::StencilState::FRONT), vk::StencilOp::eIncrementAndWrap);
	EXPECT_EQ(test_state.getStencilPassOp(vkr::PipelineState::StencilState::FRONT), vk::StencilOp::eDecrementAndWrap);
	EXPECT_EQ(test_state.getStencilDepthFailOp(vkr::PipelineState::StencilState::FRONT), vk::StencilOp::eInvert);
	EXPECT_EQ(test_state.getStencilCompareOp(vkr::PipelineState::StencilState::FRONT), vk::CompareOp::eNever);
	EXPECT_EQ(test_state.getStencilCompareMask(vkr::PipelineState::StencilState::FRONT), 1);
	EXPECT_EQ(test_state.getStencilWriteMask(vkr::PipelineState::StencilState::FRONT), 6);
	EXPECT_EQ(test_state.getStencilReference(vkr::PipelineState::StencilState::FRONT), 46);
	EXPECT_EQ(test_state.getStencilFailOp(vkr::PipelineState::StencilState::BACK), vk::StencilOp::eZero);
	EXPECT_EQ(test_state.getStencilPassOp(vkr::PipelineState::StencilState::BACK), vk::StencilOp::eKeep);
	EXPECT_EQ(test_state.getStencilDepthFailOp(vkr::PipelineState::StencilState::BACK), vk::StencilOp::eReplace);
	EXPECT_EQ(test_state.getStencilCompareOp(vkr::PipelineState::StencilState::BACK), vk::CompareOp::eEqual);
	EXPECT_EQ(test_state.getStencilCompareMask(vkr::PipelineState::StencilState::BACK), 13);
	EXPECT_EQ(test_state.getStencilWriteMask(vkr::PipelineState::StencilState::BACK), 230);
	EXPECT_EQ(test_state.getStencilReference(vkr::PipelineState::StencilState::BACK), 126);

	EXPECT_EQ(test_state.getBlendLogicOpEnable(), true);
	EXPECT_EQ(test_state.getBlendLogicOp(), vk::LogicOp::eAndReverse);
	EXPECT_EQ(test_state.getDynamicBlendConstants(), true);
	EXPECT_EQ(test_state.getNumAttachments(), 6);

	EXPECT_EQ(test_state.getBlendEnable(1u), true);
	EXPECT_EQ(test_state.getSrcColorBlendFactor(1u), vk::BlendFactor::eConstantAlpha);
	EXPECT_EQ(test_state.getDstColorBlendFactor(1u), vk::BlendFactor::eConstantColor);
	EXPECT_EQ(test_state.getColorBlendOp(1u), vk::BlendOp::eAdd);
	EXPECT_EQ(test_state.getSrcAlphaBlendFactor(1u), vk::BlendFactor::eDstAlpha);
	EXPECT_EQ(test_state.getDstAlphaBlendFactor(1u), vk::BlendFactor::eDstColor);
	EXPECT_EQ(test_state.getAlphaBlendOp(1u), vk::BlendOp::eMax);
	EXPECT_EQ(test_state.getBlendMaskRed(1u), false);
	EXPECT_EQ(test_state.getBlendMaskGreen(1u), true);
	EXPECT_EQ(test_state.getBlendMaskBlue(1u), false);
	EXPECT_EQ(test_state.getBlendMaskAlpha(1u), true);

	EXPECT_EQ(test_state.getBlendEnable(4u), true);
	EXPECT_EQ(test_state.getSrcColorBlendFactor(4u), vk::BlendFactor::eOne);
	EXPECT_EQ(test_state.getDstColorBlendFactor(4u), vk::BlendFactor::eOneMinusConstantAlpha);
	EXPECT_EQ(test_state.getColorBlendOp(4u), vk::BlendOp::eMin);
	EXPECT_EQ(test_state.getSrcAlphaBlendFactor(4u), vk::BlendFactor::eOneMinusConstantColor);
	EXPECT_EQ(test_state.getDstAlphaBlendFactor(4u), vk::BlendFactor::eOneMinusDstAlpha);
	EXPECT_EQ(test_state.getAlphaBlendOp(4u), vk::BlendOp::eAdd);
	EXPECT_EQ(test_state.getBlendMaskRed(4u), true);
	EXPECT_EQ(test_state.getBlendMaskGreen(4u), false);
	EXPECT_EQ(test_state.getBlendMaskBlue(4u), true);
	EXPECT_EQ(test_state.getBlendMaskAlpha(4u), false);

	vkr::PipelineState copy = test_state;
	copy.getBlendMaskGreen(7);
	EXPECT_EQ(test_state.computeHash(), copy.computeHash());*/
}