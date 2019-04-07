/**
 * Project Logi source code
 * Copyright (C) 2019 Primoz Lavric
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LOGI_STRUCTURES_VK_STRUCTURE_HPP
#define LOGI_STRUCTURES_VK_STRUCTURE_HPP

#include <vulkan/vulkan.hpp>
#include "logi/structures/structure.hpp"

namespace logi {

class BindBufferMemoryDeviceGroupInfo : public Structure<vk::BindBufferMemoryDeviceGroupInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(deviceIndices, pDeviceIndices, deviceIndexCount);
  }

  std::vector<uint32_t> deviceIndices;
};

class BindBufferMemoryInfo : public Structure<vk::BindBufferMemoryInfo> {
 public:
  using VkType::buffer;
  using VkType::memory;
  using VkType::memoryOffset;
};

using BindBufferMemoryInfoChain = StructureChain<BindBufferMemoryInfo, BindBufferMemoryDeviceGroupInfo>;

class BindImagePlaneMemoryInfo : public Structure<vk::BindImagePlaneMemoryInfo> {
 public:
  using VkType::planeAspect;
};

class BindImageMemorySwapchainInfoKHR : public Structure<vk::BindImageMemorySwapchainInfoKHR> {
 public:
  using VkType::imageIndex;
  using VkType::swapchain;
};

class BindImageMemoryDeviceGroupInfo : public Structure<vk::BindImageMemoryDeviceGroupInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(deviceIndices, pDeviceIndices, deviceIndexCount);
    vecToCArr(splitInstanceBindRegions, pSplitInstanceBindRegions, splitInstanceBindRegionCount);
  }

  std::vector<uint32_t> deviceIndices;
  std::vector<vk::Rect2D> splitInstanceBindRegions;
};

class BindImageMemoryInfo : public Structure<vk::BindImageMemoryInfo> {
 public:
  using VkType::image;
  using VkType::memory;
  using VkType::memoryOffset;
};

using BindImageMemoryInfoChain = StructureChain<BindImageMemoryInfo, BindImageMemoryDeviceGroupInfo,
                                                BindImageMemorySwapchainInfoKHR, BindImagePlaneMemoryInfo>;

class DeviceGroupBindSparseInfo : public Structure<vk::DeviceGroupBindSparseInfo> {
 public:
  using VkType::memoryDeviceIndex;
  using VkType::resourceDeviceIndex;
};

class SparseBufferMemoryBindInfo : public Structure<vk::SparseBufferMemoryBindInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(binds, pBinds, bindCount);
  }

  using VkType::buffer;
  std::vector<vk::SparseMemoryBind> binds;
};

class SparseImageOpaqueMemoryBindInfo : public Structure<vk::SparseImageOpaqueMemoryBindInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(binds, pBinds, bindCount);
  }

  using VkType::image;
  std::vector<vk::SparseMemoryBind> binds;
};

class SparseImageMemoryBindInfo : public Structure<vk::SparseImageMemoryBindInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(binds, pBinds, bindCount);
  }

  using VkType::image;
  std::vector<vk::SparseImageMemoryBind> binds;
};

class BindSparseInfo : public Structure<vk::BindSparseInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(waitSemaphores, pWaitSemaphores, waitSemaphoreCount);
    vecLogiToVk(bufferBinds, vkBufferBinds);
    vecToCArr(vkBufferBinds, pBufferBinds, bufferBindCount);
    vecLogiToVk(imageOpaqueBinds, vkImageOpaqueBinds);
    vecToCArr(vkImageOpaqueBinds, pImageOpaqueBinds, imageOpaqueBindCount);
    vecLogiToVk(imageBinds, vkImageBinds);
    vecToCArr(vkImageBinds, pImageBinds, imageBindCount);
    vecToCArr(signalSemaphores, pSignalSemaphores, signalSemaphoreCount);
  }

  std::vector<vk::Semaphore> waitSemaphores;
  std::vector<SparseBufferMemoryBindInfo> bufferBinds;
  std::vector<SparseImageOpaqueMemoryBindInfo> imageOpaqueBinds;
  std::vector<SparseImageMemoryBindInfo> imageBinds;
  std::vector<vk::Semaphore> signalSemaphores;

 private:
  std::vector<vk::SparseBufferMemoryBindInfo> vkBufferBinds;
  std::vector<vk::SparseImageOpaqueMemoryBindInfo> vkImageOpaqueBinds;
  std::vector<vk::SparseImageMemoryBindInfo> vkImageBinds;
};

using BindSparseInfoChain = StructureChain<BindSparseInfo, SparseImageMemoryBindInfo, SparseImageOpaqueMemoryBindInfo,
                                           SparseBufferMemoryBindInfo, DeviceGroupBindSparseInfo>;

class ExternalMemoryBufferCreateInfo : public Structure<vk::ExternalMemoryBufferCreateInfo> {
 public:
  using VkType::handleTypes;
};

class DedicatedAllocationBufferCreateInfoNV : public Structure<vk::DedicatedAllocationBufferCreateInfoNV> {
 public:
  using VkType::dedicatedAllocation;
};

class BufferDeviceAddressCreateInfoEXT : public Structure<vk::BufferDeviceAddressCreateInfoEXT> {
 public:
  using VkType::deviceAddress;
};

class BufferCreateInfo : public Structure<vk::BufferCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(queueFamilyIndices, pQueueFamilyIndices, queueFamilyIndexCount);
  }

  using VkType::flags;
  using VkType::sharingMode;
  using VkType::size;
  using VkType::usage;
  std::vector<uint32_t> queueFamilyIndices;
};

using BufferCreateInfoChain = StructureChain<BufferCreateInfo, BufferDeviceAddressCreateInfoEXT,
                                             DedicatedAllocationBufferCreateInfoNV, ExternalMemoryBufferCreateInfo>;

class CommandBufferInheritanceConditionalRenderingInfoEXT
  : public Structure<vk::CommandBufferInheritanceConditionalRenderingInfoEXT> {
 public:
  using VkType::conditionalRenderingEnable;
};

class CommandBufferInheritanceInfo : public Structure<vk::CommandBufferInheritanceInfo> {
 public:
  using VkType::framebuffer;
  using VkType::occlusionQueryEnable;
  using VkType::pipelineStatistics;
  using VkType::queryFlags;
  using VkType::renderPass;
  using VkType::subpass;
};

using CommandBufferInheritanceInfoChain =
  StructureChain<CommandBufferInheritanceInfo, CommandBufferInheritanceConditionalRenderingInfoEXT>;

class DescriptorPoolInlineUniformBlockCreateInfoEXT
  : public Structure<vk::DescriptorPoolInlineUniformBlockCreateInfoEXT> {
 public:
  using VkType::maxInlineUniformBlockBindings;
};

class DescriptorPoolCreateInfo : public Structure<vk::DescriptorPoolCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(poolSizes, pPoolSizes, poolSizeCount);
  }

  using VkType::flags;
  using VkType::maxSets;
  std::vector<vk::DescriptorPoolSize> poolSizes;
};

using DescriptorPoolCreateInfoChain =
  StructureChain<DescriptorPoolCreateInfo, DescriptorPoolInlineUniformBlockCreateInfoEXT>;

class DescriptorSetVariableDescriptorCountAllocateInfoEXT
  : public Structure<vk::DescriptorSetVariableDescriptorCountAllocateInfoEXT> {
 public:
  void updateVkStructure() override {
    vecToCArr(descriptorCounts, pDescriptorCounts, descriptorSetCount);
  }

  std::vector<uint32_t> descriptorCounts;
};

class DescriptorSetAllocateInfo : public Structure<vk::DescriptorSetAllocateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(setLayouts, pSetLayouts, descriptorSetCount);
  }

  using VkType::descriptorPool;
  std::vector<vk::DescriptorSetLayout> setLayouts;
};

using DescriptorSetAllocateInfoChain =
  StructureChain<DescriptorSetAllocateInfo, DescriptorSetVariableDescriptorCountAllocateInfoEXT>;

class DescriptorSetVariableDescriptorCountLayoutSupportEXT
  : public Structure<vk::DescriptorSetVariableDescriptorCountLayoutSupportEXT> {
 public:
  using VkType::maxVariableDescriptorCount;
};

class DescriptorSetLayoutSupport : public Structure<vk::DescriptorSetLayoutSupport> {
 public:
  using VkType::supported;
};

using DescriptorSetLayoutSupportChain =
  StructureChain<DescriptorSetLayoutSupport, DescriptorSetVariableDescriptorCountLayoutSupportEXT>;

class PhysicalDeviceMemoryBudgetPropertiesEXT : public Structure<vk::PhysicalDeviceMemoryBudgetPropertiesEXT> {
 public:
  using VkType::heapBudget;
  using VkType::heapUsage;
};

class PhysicalDeviceMemoryProperties2 : public Structure<vk::PhysicalDeviceMemoryProperties2> {
 public:
  using VkType::memoryProperties;
};

using PhysicalDeviceMemoryProperties2Chain =
  StructureChain<PhysicalDeviceMemoryProperties2, PhysicalDeviceMemoryBudgetPropertiesEXT>;

class PhysicalDeviceVertexAttributeDivisorPropertiesEXT
  : public Structure<vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT> {
 public:
  using VkType::maxVertexAttribDivisor;
};

class PhysicalDeviceTransformFeedbackPropertiesEXT
  : public Structure<vk::PhysicalDeviceTransformFeedbackPropertiesEXT> {
 public:
  using VkType::maxTransformFeedbackBufferDataSize;
  using VkType::maxTransformFeedbackBufferDataStride;
  using VkType::maxTransformFeedbackBuffers;
  using VkType::maxTransformFeedbackBufferSize;
  using VkType::maxTransformFeedbackStreamDataSize;
  using VkType::maxTransformFeedbackStreams;
  using VkType::transformFeedbackDraw;
  using VkType::transformFeedbackQueries;
  using VkType::transformFeedbackRasterizationStreamSelect;
  using VkType::transformFeedbackStreamsLinesTriangles;
};

class PhysicalDeviceSubgroupProperties : public Structure<vk::PhysicalDeviceSubgroupProperties> {
 public:
  using VkType::quadOperationsInAllStages;
  using VkType::subgroupSize;
  using VkType::supportedOperations;
  using VkType::supportedStages;
};

class PhysicalDeviceShadingRateImagePropertiesNV : public Structure<vk::PhysicalDeviceShadingRateImagePropertiesNV> {
 public:
  using VkType::shadingRateMaxCoarseSamples;
  using VkType::shadingRatePaletteSize;
  using VkType::shadingRateTexelSize;
};

class PhysicalDeviceShaderCorePropertiesAMD : public Structure<vk::PhysicalDeviceShaderCorePropertiesAMD> {
 public:
  using VkType::computeUnitsPerShaderArray;
  using VkType::maxSgprAllocation;
  using VkType::maxVgprAllocation;
  using VkType::minSgprAllocation;
  using VkType::minVgprAllocation;
  using VkType::sgprAllocationGranularity;
  using VkType::sgprsPerSimd;
  using VkType::shaderArraysPerEngineCount;
  using VkType::shaderEngineCount;
  using VkType::simdPerComputeUnit;
  using VkType::vgprAllocationGranularity;
  using VkType::vgprsPerSimd;
  using VkType::wavefrontSize;
  using VkType::wavefrontsPerSimd;
};

class PhysicalDeviceSamplerFilterMinmaxPropertiesEXT
  : public Structure<vk::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT> {
 public:
  using VkType::filterMinmaxImageComponentMapping;
  using VkType::filterMinmaxSingleComponentFormats;
};

class PhysicalDeviceSampleLocationsPropertiesEXT : public Structure<vk::PhysicalDeviceSampleLocationsPropertiesEXT> {
 public:
  using VkType::maxSampleLocationGridSize;
  using VkType::sampleLocationCoordinateRange;
  using VkType::sampleLocationSampleCounts;
  using VkType::sampleLocationSubPixelBits;
  using VkType::variableSampleLocations;
};

class PhysicalDeviceRayTracingPropertiesNV : public Structure<vk::PhysicalDeviceRayTracingPropertiesNV> {
 public:
  using VkType::maxDescriptorSetAccelerationStructures;
  using VkType::maxGeometryCount;
  using VkType::maxInstanceCount;
  using VkType::maxRecursionDepth;
  using VkType::maxShaderGroupStride;
  using VkType::maxTriangleCount;
  using VkType::shaderGroupBaseAlignment;
  using VkType::shaderGroupHandleSize;
};

class PhysicalDevicePushDescriptorPropertiesKHR : public Structure<vk::PhysicalDevicePushDescriptorPropertiesKHR> {
 public:
  using VkType::maxPushDescriptors;
};

class PhysicalDeviceProtectedMemoryProperties : public Structure<vk::PhysicalDeviceProtectedMemoryProperties> {
 public:
  using VkType::protectedNoFault;
};

class PhysicalDevicePointClippingProperties : public Structure<vk::PhysicalDevicePointClippingProperties> {
 public:
  using VkType::pointClippingBehavior;
};

class PhysicalDevicePCIBusInfoPropertiesEXT : public Structure<vk::PhysicalDevicePCIBusInfoPropertiesEXT> {
 public:
  using VkType::pciBus;
  using VkType::pciDevice;
  using VkType::pciDomain;
  using VkType::pciFunction;
};

class PhysicalDeviceMultiviewProperties : public Structure<vk::PhysicalDeviceMultiviewProperties> {
 public:
  using VkType::maxMultiviewInstanceIndex;
  using VkType::maxMultiviewViewCount;
};

class PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX
  : public Structure<vk::PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX> {
 public:
  using VkType::perViewPositionAllComponents;
};

class PhysicalDeviceMeshShaderPropertiesNV : public Structure<vk::PhysicalDeviceMeshShaderPropertiesNV> {
 public:
  using VkType::maxDrawMeshTasksCount;
  using VkType::maxMeshMultiviewViewCount;
  using VkType::maxMeshOutputPrimitives;
  using VkType::maxMeshOutputVertices;
  using VkType::maxMeshTotalMemorySize;
  using VkType::maxMeshWorkGroupInvocations;
  using VkType::maxMeshWorkGroupSize;
  using VkType::maxTaskOutputCount;
  using VkType::maxTaskTotalMemorySize;
  using VkType::maxTaskWorkGroupInvocations;
  using VkType::maxTaskWorkGroupSize;
  using VkType::meshOutputPerPrimitiveGranularity;
  using VkType::meshOutputPerVertexGranularity;
};

class PhysicalDeviceMaintenance3Properties : public Structure<vk::PhysicalDeviceMaintenance3Properties> {
 public:
  using VkType::maxMemoryAllocationSize;
  using VkType::maxPerSetDescriptors;
};

class PhysicalDeviceInlineUniformBlockPropertiesEXT
  : public Structure<vk::PhysicalDeviceInlineUniformBlockPropertiesEXT> {
 public:
  using VkType::maxDescriptorSetInlineUniformBlocks;
  using VkType::maxDescriptorSetUpdateAfterBindInlineUniformBlocks;
  using VkType::maxInlineUniformBlockSize;
  using VkType::maxPerStageDescriptorInlineUniformBlocks;
  using VkType::maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks;
};

class PhysicalDeviceIDProperties : public Structure<vk::PhysicalDeviceIDProperties> {
 public:
  using VkType::deviceLUID;
  using VkType::deviceLUIDValid;
  using VkType::deviceNodeMask;
  using VkType::deviceUUID;
  using VkType::driverUUID;
};

class PhysicalDeviceFragmentDensityMapPropertiesEXT
  : public Structure<vk::PhysicalDeviceFragmentDensityMapPropertiesEXT> {
 public:
  using VkType::fragmentDensityInvocations;
  using VkType::maxFragmentDensityTexelSize;
  using VkType::minFragmentDensityTexelSize;
};

class PhysicalDeviceFloatControlsPropertiesKHR : public Structure<vk::PhysicalDeviceFloatControlsPropertiesKHR> {
 public:
  using VkType::separateDenormSettings;
  using VkType::separateRoundingModeSettings;
  using VkType::shaderDenormFlushToZeroFloat16;
  using VkType::shaderDenormFlushToZeroFloat32;
  using VkType::shaderDenormFlushToZeroFloat64;
  using VkType::shaderDenormPreserveFloat16;
  using VkType::shaderDenormPreserveFloat32;
  using VkType::shaderDenormPreserveFloat64;
  using VkType::shaderRoundingModeRTEFloat16;
  using VkType::shaderRoundingModeRTEFloat32;
  using VkType::shaderRoundingModeRTEFloat64;
  using VkType::shaderRoundingModeRTZFloat16;
  using VkType::shaderRoundingModeRTZFloat32;
  using VkType::shaderRoundingModeRTZFloat64;
  using VkType::shaderSignedZeroInfNanPreserveFloat16;
  using VkType::shaderSignedZeroInfNanPreserveFloat32;
  using VkType::shaderSignedZeroInfNanPreserveFloat64;
};

class PhysicalDeviceExternalMemoryHostPropertiesEXT
  : public Structure<vk::PhysicalDeviceExternalMemoryHostPropertiesEXT> {
 public:
  using VkType::minImportedHostPointerAlignment;
};

class PhysicalDeviceDriverPropertiesKHR : public Structure<vk::PhysicalDeviceDriverPropertiesKHR> {
 public:
  using VkType::conformanceVersion;
  using VkType::driverID;
  using VkType::driverInfo;
  using VkType::driverName;
};

class PhysicalDeviceDiscardRectanglePropertiesEXT : public Structure<vk::PhysicalDeviceDiscardRectanglePropertiesEXT> {
 public:
  using VkType::maxDiscardRectangles;
};

class PhysicalDeviceDescriptorIndexingPropertiesEXT
  : public Structure<vk::PhysicalDeviceDescriptorIndexingPropertiesEXT> {
 public:
  using VkType::maxDescriptorSetUpdateAfterBindInputAttachments;
  using VkType::maxDescriptorSetUpdateAfterBindSampledImages;
  using VkType::maxDescriptorSetUpdateAfterBindSamplers;
  using VkType::maxDescriptorSetUpdateAfterBindStorageBuffers;
  using VkType::maxDescriptorSetUpdateAfterBindStorageBuffersDynamic;
  using VkType::maxDescriptorSetUpdateAfterBindStorageImages;
  using VkType::maxDescriptorSetUpdateAfterBindUniformBuffers;
  using VkType::maxDescriptorSetUpdateAfterBindUniformBuffersDynamic;
  using VkType::maxPerStageDescriptorUpdateAfterBindInputAttachments;
  using VkType::maxPerStageDescriptorUpdateAfterBindSampledImages;
  using VkType::maxPerStageDescriptorUpdateAfterBindSamplers;
  using VkType::maxPerStageDescriptorUpdateAfterBindStorageBuffers;
  using VkType::maxPerStageDescriptorUpdateAfterBindStorageImages;
  using VkType::maxPerStageDescriptorUpdateAfterBindUniformBuffers;
  using VkType::maxPerStageUpdateAfterBindResources;
  using VkType::maxUpdateAfterBindDescriptorsInAllPools;
  using VkType::quadDivergentImplicitLod;
  using VkType::robustBufferAccessUpdateAfterBind;
  using VkType::shaderInputAttachmentArrayNonUniformIndexingNative;
  using VkType::shaderSampledImageArrayNonUniformIndexingNative;
  using VkType::shaderStorageBufferArrayNonUniformIndexingNative;
  using VkType::shaderStorageImageArrayNonUniformIndexingNative;
  using VkType::shaderUniformBufferArrayNonUniformIndexingNative;
};

class PhysicalDeviceDepthStencilResolvePropertiesKHR
  : public Structure<vk::PhysicalDeviceDepthStencilResolvePropertiesKHR> {
 public:
  using VkType::independentResolve;
  using VkType::independentResolveNone;
  using VkType::supportedDepthResolveModes;
  using VkType::supportedStencilResolveModes;
};

class PhysicalDeviceCooperativeMatrixPropertiesNV : public Structure<vk::PhysicalDeviceCooperativeMatrixPropertiesNV> {
 public:
  using VkType::cooperativeMatrixSupportedStages;
};

class PhysicalDeviceConservativeRasterizationPropertiesEXT
  : public Structure<vk::PhysicalDeviceConservativeRasterizationPropertiesEXT> {
 public:
  using VkType::conservativePointAndLineRasterization;
  using VkType::conservativeRasterizationPostDepthCoverage;
  using VkType::degenerateLinesRasterized;
  using VkType::degenerateTrianglesRasterized;
  using VkType::extraPrimitiveOverestimationSizeGranularity;
  using VkType::fullyCoveredFragmentShaderInputVariable;
  using VkType::maxExtraPrimitiveOverestimationSize;
  using VkType::primitiveOverestimationSize;
  using VkType::primitiveUnderestimation;
};

class PhysicalDeviceBlendOperationAdvancedPropertiesEXT
  : public Structure<vk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT> {
 public:
  using VkType::advancedBlendAllOperations;
  using VkType::advancedBlendCorrelatedOverlap;
  using VkType::advancedBlendIndependentBlend;
  using VkType::advancedBlendMaxColorAttachments;
  using VkType::advancedBlendNonPremultipliedDstColor;
  using VkType::advancedBlendNonPremultipliedSrcColor;
};

class PhysicalDeviceProperties2 : public Structure<vk::PhysicalDeviceProperties2> {
 public:
  using VkType::properties;
};

using PhysicalDeviceProperties2Chain =
  StructureChain<PhysicalDeviceProperties2, PhysicalDeviceBlendOperationAdvancedPropertiesEXT,
                 PhysicalDeviceConservativeRasterizationPropertiesEXT, PhysicalDeviceCooperativeMatrixPropertiesNV,
                 PhysicalDeviceDepthStencilResolvePropertiesKHR, PhysicalDeviceDescriptorIndexingPropertiesEXT,
                 PhysicalDeviceDiscardRectanglePropertiesEXT, PhysicalDeviceDriverPropertiesKHR,
                 PhysicalDeviceExternalMemoryHostPropertiesEXT, PhysicalDeviceFloatControlsPropertiesKHR,
                 PhysicalDeviceFragmentDensityMapPropertiesEXT, PhysicalDeviceIDProperties,
                 PhysicalDeviceInlineUniformBlockPropertiesEXT, PhysicalDeviceMaintenance3Properties,
                 PhysicalDeviceMeshShaderPropertiesNV, PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX,
                 PhysicalDeviceMultiviewProperties, PhysicalDevicePCIBusInfoPropertiesEXT,
                 PhysicalDevicePointClippingProperties, PhysicalDeviceProtectedMemoryProperties,
                 PhysicalDevicePushDescriptorPropertiesKHR, PhysicalDeviceRayTracingPropertiesNV,
                 PhysicalDeviceSampleLocationsPropertiesEXT, PhysicalDeviceSamplerFilterMinmaxPropertiesEXT,
                 PhysicalDeviceShaderCorePropertiesAMD, PhysicalDeviceShadingRateImagePropertiesNV,
                 PhysicalDeviceSubgroupProperties, PhysicalDeviceTransformFeedbackPropertiesEXT,
                 PhysicalDeviceVertexAttributeDivisorPropertiesEXT>;

class PhysicalDeviceVulkanMemoryModelFeaturesKHR : public Structure<vk::PhysicalDeviceVulkanMemoryModelFeaturesKHR> {
 public:
  using VkType::vulkanMemoryModel;
  using VkType::vulkanMemoryModelAvailabilityVisibilityChains;
  using VkType::vulkanMemoryModelDeviceScope;
};

class PhysicalDeviceVertexAttributeDivisorFeaturesEXT
  : public Structure<vk::PhysicalDeviceVertexAttributeDivisorFeaturesEXT> {
 public:
  using VkType::vertexAttributeInstanceRateDivisor;
  using VkType::vertexAttributeInstanceRateZeroDivisor;
};

class PhysicalDeviceVariablePointerFeatures : public Structure<vk::PhysicalDeviceVariablePointerFeatures> {
 public:
  using VkType::variablePointers;
  using VkType::variablePointersStorageBuffer;
};

class PhysicalDeviceTransformFeedbackFeaturesEXT : public Structure<vk::PhysicalDeviceTransformFeedbackFeaturesEXT> {
 public:
  using VkType::geometryStreams;
  using VkType::transformFeedback;
};

class PhysicalDeviceShadingRateImageFeaturesNV : public Structure<vk::PhysicalDeviceShadingRateImageFeaturesNV> {
 public:
  using VkType::shadingRateCoarseSampleOrder;
  using VkType::shadingRateImage;
};

class PhysicalDeviceShaderImageFootprintFeaturesNV
  : public Structure<vk::PhysicalDeviceShaderImageFootprintFeaturesNV> {
 public:
  using VkType::imageFootprint;
};

class PhysicalDeviceShaderDrawParameterFeatures : public Structure<vk::PhysicalDeviceShaderDrawParameterFeatures> {
 public:
  using VkType::shaderDrawParameters;
};

class PhysicalDeviceShaderAtomicInt64FeaturesKHR : public Structure<vk::PhysicalDeviceShaderAtomicInt64FeaturesKHR> {
 public:
  using VkType::shaderBufferInt64Atomics;
  using VkType::shaderSharedInt64Atomics;
};

class PhysicalDeviceScalarBlockLayoutFeaturesEXT : public Structure<vk::PhysicalDeviceScalarBlockLayoutFeaturesEXT> {
 public:
  using VkType::scalarBlockLayout;
};

class PhysicalDeviceSamplerYcbcrConversionFeatures
  : public Structure<vk::PhysicalDeviceSamplerYcbcrConversionFeatures> {
 public:
  using VkType::samplerYcbcrConversion;
};

class PhysicalDeviceRepresentativeFragmentTestFeaturesNV
  : public Structure<vk::PhysicalDeviceRepresentativeFragmentTestFeaturesNV> {
 public:
  using VkType::representativeFragmentTest;
};

class PhysicalDeviceProtectedMemoryFeatures : public Structure<vk::PhysicalDeviceProtectedMemoryFeatures> {
 public:
  using VkType::protectedMemory;
};

class PhysicalDeviceMultiviewFeatures : public Structure<vk::PhysicalDeviceMultiviewFeatures> {
 public:
  using VkType::multiview;
  using VkType::multiviewGeometryShader;
  using VkType::multiviewTessellationShader;
};

class PhysicalDeviceMeshShaderFeaturesNV : public Structure<vk::PhysicalDeviceMeshShaderFeaturesNV> {
 public:
  using VkType::meshShader;
  using VkType::taskShader;
};

class PhysicalDeviceMemoryPriorityFeaturesEXT : public Structure<vk::PhysicalDeviceMemoryPriorityFeaturesEXT> {
 public:
  using VkType::memoryPriority;
};

class PhysicalDeviceInlineUniformBlockFeaturesEXT : public Structure<vk::PhysicalDeviceInlineUniformBlockFeaturesEXT> {
 public:
  using VkType::descriptorBindingInlineUniformBlockUpdateAfterBind;
  using VkType::inlineUniformBlock;
};

class PhysicalDeviceFragmentShaderBarycentricFeaturesNV
  : public Structure<vk::PhysicalDeviceFragmentShaderBarycentricFeaturesNV> {
 public:
  using VkType::fragmentShaderBarycentric;
};

class PhysicalDeviceFragmentDensityMapFeaturesEXT : public Structure<vk::PhysicalDeviceFragmentDensityMapFeaturesEXT> {
 public:
  using VkType::fragmentDensityMap;
  using VkType::fragmentDensityMapDynamic;
  using VkType::fragmentDensityMapNonSubsampledImages;
};

class PhysicalDeviceFloat16Int8FeaturesKHR : public Structure<vk::PhysicalDeviceFloat16Int8FeaturesKHR> {
 public:
  using VkType::shaderFloat16;
  using VkType::shaderInt8;
};

class PhysicalDeviceExclusiveScissorFeaturesNV : public Structure<vk::PhysicalDeviceExclusiveScissorFeaturesNV> {
 public:
  using VkType::exclusiveScissor;
};

class PhysicalDeviceDescriptorIndexingFeaturesEXT : public Structure<vk::PhysicalDeviceDescriptorIndexingFeaturesEXT> {
 public:
  using VkType::descriptorBindingPartiallyBound;
  using VkType::descriptorBindingSampledImageUpdateAfterBind;
  using VkType::descriptorBindingStorageBufferUpdateAfterBind;
  using VkType::descriptorBindingStorageImageUpdateAfterBind;
  using VkType::descriptorBindingStorageTexelBufferUpdateAfterBind;
  using VkType::descriptorBindingUniformBufferUpdateAfterBind;
  using VkType::descriptorBindingUniformTexelBufferUpdateAfterBind;
  using VkType::descriptorBindingUpdateUnusedWhilePending;
  using VkType::descriptorBindingVariableDescriptorCount;
  using VkType::runtimeDescriptorArray;
  using VkType::shaderInputAttachmentArrayDynamicIndexing;
  using VkType::shaderInputAttachmentArrayNonUniformIndexing;
  using VkType::shaderSampledImageArrayNonUniformIndexing;
  using VkType::shaderStorageBufferArrayNonUniformIndexing;
  using VkType::shaderStorageImageArrayNonUniformIndexing;
  using VkType::shaderStorageTexelBufferArrayDynamicIndexing;
  using VkType::shaderStorageTexelBufferArrayNonUniformIndexing;
  using VkType::shaderUniformBufferArrayNonUniformIndexing;
  using VkType::shaderUniformTexelBufferArrayDynamicIndexing;
  using VkType::shaderUniformTexelBufferArrayNonUniformIndexing;
};

class PhysicalDeviceDepthClipEnableFeaturesEXT : public Structure<vk::PhysicalDeviceDepthClipEnableFeaturesEXT> {
 public:
  using VkType::depthClipEnable;
};

class PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV
  : public Structure<vk::PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV> {
 public:
  using VkType::dedicatedAllocationImageAliasing;
};

class PhysicalDeviceCornerSampledImageFeaturesNV : public Structure<vk::PhysicalDeviceCornerSampledImageFeaturesNV> {
 public:
  using VkType::cornerSampledImage;
};

class PhysicalDeviceCooperativeMatrixFeaturesNV : public Structure<vk::PhysicalDeviceCooperativeMatrixFeaturesNV> {
 public:
  using VkType::cooperativeMatrix;
  using VkType::cooperativeMatrixRobustBufferAccess;
};

class PhysicalDeviceConditionalRenderingFeaturesEXT
  : public Structure<vk::PhysicalDeviceConditionalRenderingFeaturesEXT> {
 public:
  using VkType::conditionalRendering;
  using VkType::inheritedConditionalRendering;
};

class PhysicalDeviceComputeShaderDerivativesFeaturesNV
  : public Structure<vk::PhysicalDeviceComputeShaderDerivativesFeaturesNV> {
 public:
  using VkType::computeDerivativeGroupLinear;
  using VkType::computeDerivativeGroupQuads;
};

class PhysicalDeviceBufferAddressFeaturesEXT : public Structure<vk::PhysicalDeviceBufferAddressFeaturesEXT> {
 public:
  using VkType::bufferDeviceAddress;
  using VkType::bufferDeviceAddressCaptureReplay;
  using VkType::bufferDeviceAddressMultiDevice;
};

class PhysicalDeviceBlendOperationAdvancedFeaturesEXT
  : public Structure<vk::PhysicalDeviceBlendOperationAdvancedFeaturesEXT> {
 public:
  using VkType::advancedBlendCoherentOperations;
};

class PhysicalDeviceASTCDecodeFeaturesEXT : public Structure<vk::PhysicalDeviceASTCDecodeFeaturesEXT> {
 public:
  using VkType::decodeModeSharedExponent;
};

class PhysicalDevice8BitStorageFeaturesKHR : public Structure<vk::PhysicalDevice8BitStorageFeaturesKHR> {
 public:
  using VkType::storageBuffer8BitAccess;
  using VkType::storagePushConstant8;
  using VkType::uniformAndStorageBuffer8BitAccess;
};

class PhysicalDevice16BitStorageFeatures : public Structure<vk::PhysicalDevice16BitStorageFeatures> {
 public:
  using VkType::storageBuffer16BitAccess;
  using VkType::storageInputOutput16;
  using VkType::storagePushConstant16;
  using VkType::uniformAndStorageBuffer16BitAccess;
};

class PhysicalDeviceFeatures2 : public Structure<vk::PhysicalDeviceFeatures2> {
 public:
  using VkType::features;
};

using PhysicalDeviceFeatures2Chain =
  StructureChain<PhysicalDeviceFeatures2, PhysicalDevice16BitStorageFeatures, PhysicalDevice8BitStorageFeaturesKHR,
                 PhysicalDeviceASTCDecodeFeaturesEXT, PhysicalDeviceBlendOperationAdvancedFeaturesEXT,
                 PhysicalDeviceBufferAddressFeaturesEXT, PhysicalDeviceComputeShaderDerivativesFeaturesNV,
                 PhysicalDeviceConditionalRenderingFeaturesEXT, PhysicalDeviceCooperativeMatrixFeaturesNV,
                 PhysicalDeviceCornerSampledImageFeaturesNV, PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV,
                 PhysicalDeviceDepthClipEnableFeaturesEXT, PhysicalDeviceDescriptorIndexingFeaturesEXT,
                 PhysicalDeviceExclusiveScissorFeaturesNV, PhysicalDeviceFloat16Int8FeaturesKHR,
                 PhysicalDeviceFragmentDensityMapFeaturesEXT, PhysicalDeviceFragmentShaderBarycentricFeaturesNV,
                 PhysicalDeviceInlineUniformBlockFeaturesEXT, PhysicalDeviceMemoryPriorityFeaturesEXT,
                 PhysicalDeviceMeshShaderFeaturesNV, PhysicalDeviceMultiviewFeatures,
                 PhysicalDeviceProtectedMemoryFeatures, PhysicalDeviceRepresentativeFragmentTestFeaturesNV,
                 PhysicalDeviceSamplerYcbcrConversionFeatures, PhysicalDeviceScalarBlockLayoutFeaturesEXT,
                 PhysicalDeviceShaderAtomicInt64FeaturesKHR, PhysicalDeviceShaderDrawParameterFeatures,
                 PhysicalDeviceShaderImageFootprintFeaturesNV, PhysicalDeviceShadingRateImageFeaturesNV,
                 PhysicalDeviceTransformFeedbackFeaturesEXT, PhysicalDeviceVariablePointerFeatures,
                 PhysicalDeviceVertexAttributeDivisorFeaturesEXT, PhysicalDeviceVulkanMemoryModelFeaturesKHR>;

class DeviceMemoryOverallocationCreateInfoAMD : public Structure<vk::DeviceMemoryOverallocationCreateInfoAMD> {
 public:
  using VkType::overallocationBehavior;
};

class DeviceGroupDeviceCreateInfo : public Structure<vk::DeviceGroupDeviceCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(physicalDevices, pPhysicalDevices, physicalDeviceCount);
  }

  uint32_t physicalDeviceCount;
  std::vector<vk::PhysicalDevice> physicalDevices;
};

class DeviceQueueGlobalPriorityCreateInfoEXT : public Structure<vk::DeviceQueueGlobalPriorityCreateInfoEXT> {
 public:
  using VkType::globalPriority;
};

class DeviceQueueCreateInfo : public Structure<vk::DeviceQueueCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(queuePriorities, pQueuePriorities, queueCount);
  }

  using VkType::flags;
  using VkType::queueFamilyIndex;
  std::vector<float> queuePriorities;
};

using DeviceQueueCreateInfoChain = StructureChain<DeviceQueueCreateInfo, DeviceQueueGlobalPriorityCreateInfoEXT>;

class ExportFenceCreateInfo : public Structure<vk::ExportFenceCreateInfo> {
 public:
  using VkType::handleTypes;
};

class FenceCreateInfo : public Structure<vk::FenceCreateInfo> {
 public:
  using VkType::flags;
};

using FenceCreateInfoChain = StructureChain<FenceCreateInfo, ExportFenceCreateInfo>;

class FormatProperties2 : public Structure<vk::FormatProperties2> {
 public:
  using VkType::formatProperties;
};

using FormatProperties2Chain = StructureChain<FormatProperties2>;

class ImageSwapchainCreateInfoKHR : public Structure<vk::ImageSwapchainCreateInfoKHR> {
 public:
  using VkType::swapchain;
};

class ImageStencilUsageCreateInfoEXT : public Structure<vk::ImageStencilUsageCreateInfoEXT> {
 public:
  using VkType::stencilUsage;
};

class ImageFormatListCreateInfoKHR : public Structure<vk::ImageFormatListCreateInfoKHR> {
 public:
  void updateVkStructure() override {
    vecToCArr(viewFormats, pViewFormats, viewFormatCount);
  }

  uint32_t viewFormatCount;
  std::vector<vk::Format> viewFormats;
};

class ImageDrmFormatModifierListCreateInfoEXT : public Structure<vk::ImageDrmFormatModifierListCreateInfoEXT> {
 public:
  void updateVkStructure() override {
    vecToCArr(drmFormatModifiers, pDrmFormatModifiers, drmFormatModifierCount);
  }

  std::vector<uint64_t> drmFormatModifiers;
};

class ImageDrmFormatModifierExplicitCreateInfoEXT : public Structure<vk::ImageDrmFormatModifierExplicitCreateInfoEXT> {
 public:
  void updateVkStructure() override {
    vecToCArr(planeLayouts, pPlaneLayouts, drmFormatModifierPlaneCount);
  }

  using VkType::drmFormatModifier;
  std::vector<vk::SubresourceLayout> planeLayouts;
};

class ExternalMemoryImageCreateInfoNV : public Structure<vk::ExternalMemoryImageCreateInfoNV> {
 public:
  using VkType::handleTypes;
};

class ExternalMemoryImageCreateInfo : public Structure<vk::ExternalMemoryImageCreateInfo> {
 public:
  using VkType::handleTypes;
};

class DedicatedAllocationImageCreateInfoNV : public Structure<vk::DedicatedAllocationImageCreateInfoNV> {
 public:
  using VkType::dedicatedAllocation;
};

class ImageCreateInfo : public Structure<vk::ImageCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(queueFamilyIndices, pQueueFamilyIndices, queueFamilyIndexCount);
  }

  using VkType::arrayLayers;
  using VkType::extent;
  using VkType::flags;
  using VkType::format;
  using VkType::imageType;
  using VkType::mipLevels;
  using VkType::samples;
  using VkType::sharingMode;
  using VkType::tiling;
  using VkType::usage;
  std::vector<uint32_t> queueFamilyIndices;
  using VkType::initialLayout;
};

using ImageCreateInfoChain =
  StructureChain<ImageCreateInfo, ImageSwapchainCreateInfoKHR, ImageStencilUsageCreateInfoEXT,
                 ImageFormatListCreateInfoKHR, ImageDrmFormatModifierListCreateInfoEXT,
                 ImageDrmFormatModifierExplicitCreateInfoEXT, ExternalMemoryImageCreateInfoNV,
                 ExternalMemoryImageCreateInfo, DedicatedAllocationImageCreateInfoNV>;

class DescriptorSetLayoutBinding : public SimpleStructure<vk::DescriptorSetLayoutBinding> {
 public:
  vk::DescriptorSetLayoutBinding build() const override {
    return vk::DescriptorSetLayoutBinding(binding, descriptorType, descriptorCount, stageFlags,
                                          immutableSamplers.data());
  }

  uint32_t binding;
  vk::DescriptorType descriptorType;
  uint32_t descriptorCount;
  vk::ShaderStageFlags stageFlags;
  std::vector<vk::Sampler> immutableSamplers;
};

class DescriptorSetLayoutBindingFlagsCreateInfoEXT
  : public Structure<vk::DescriptorSetLayoutBindingFlagsCreateInfoEXT> {
 public:
  void updateVkStructure() override {
    vecToCArr(bindingFlags, pBindingFlags, bindingCount);
  }

  std::vector<vk::DescriptorBindingFlagsEXT> bindingFlags;
};

class DescriptorSetLayoutCreateInfo : public Structure<vk::DescriptorSetLayoutCreateInfo> {
 public:
  void updateVkStructure() override {
    vk_bindings_.clear();
    vk_bindings_.reserve(bindings.size());
    for (const auto& binding : bindings) {
      vk_bindings_.emplace_back(binding.build());
    }
    vecToCArr(vk_bindings_, pBindings, bindingCount);
  }

  using VkType::flags;
  std::vector<DescriptorSetLayoutBinding> bindings;

 private:
  std::vector<vk::DescriptorSetLayoutBinding> vk_bindings_;
};

using DescriptorSetLayoutCreateInfoChain =
  StructureChain<DescriptorSetLayoutCreateInfo, DescriptorSetLayoutBindingFlagsCreateInfoEXT>;

class PipelineLayoutCreateInfo : public Structure<vk::PipelineLayoutCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(setLayouts, pSetLayouts, setLayoutCount);
    vecToCArr(pushConstantRanges, pPushConstantRanges, pushConstantRangeCount);
  }

  using VkType::flags;
  std::vector<vk::DescriptorSetLayout> setLayouts;
  std::vector<vk::PushConstantRange> pushConstantRanges;
};

using PipelineLayoutCreateInfoChain = StructureChain<PipelineLayoutCreateInfo>;

class SpecializationInfo : public Structure<vk::SpecializationInfo> {
  void updateVkStructure() override {
    vecToCArr(mapEntries, pMapEntries, mapEntryCount);
    dataSize = data.size();
    pData = data.empty() ? nullptr : data.data();
  }

  std::vector<vk::SpecializationMapEntry> mapEntries;
  std::vector<std::byte> data;
};

class PipelineShaderStageCreateInfo : public Structure<vk::PipelineShaderStageCreateInfo> {
 public:
  void updateVkStructure() override {
    pNext = name.data();
    pSpecializationInfo = &specializationInfo.build();
  }

  using VkType::flags;
  using VkType::module;
  using VkType::stage;
  std::string name = "main";
  SpecializationInfo specializationInfo;
};

/*
class ComputePipelineCreateInfo : public Structure<vk::ComputePipelineCreateInfo> {
  using VkType::flags;
};

VkPipelineCreateFlags flags;
VkPipelineShaderStageCreateInfo stage;
VkPipelineLayout layout;
VkPipeline basePipelineHandle;
int32_t basePipelineIndex;
*/
// endregion

} // namespace logi

#endif // LOGI_STRUCTURES_VK_STRUCTURE_HPP
