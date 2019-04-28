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
#include "logi/base/exception.hpp"
#include "logi/structures/simple_structures.hpp"
#include "logi/structures/structure.hpp"

namespace logi {

class GeometryAABBNV : public Structure<vk::GeometryAABBNV> {
 public:
  using VkType::aabbData;
  using VkType::numAABBs;
  using VkType::offset;
  using VkType::stride;
};

using GeometryAABBNVChain = StructureChain<GeometryAABBNV>;

class GeometryTrianglesNV : public Structure<vk::GeometryTrianglesNV> {
 public:
  using VkType::indexCount;
  using VkType::indexData;
  using VkType::indexOffset;
  using VkType::indexType;
  using VkType::transformData;
  using VkType::transformOffset;
  using VkType::vertexCount;
  using VkType::vertexData;
  using VkType::vertexFormat;
  using VkType::vertexOffset;
  using VkType::vertexStride;
};

using GeometryTrianglesNVChain = StructureChain<GeometryTrianglesNV>;

class GeometryDataNV : public Structure<vk::GeometryDataNV> {
 public:
  void updateVkStructure() override {
    VkType::triangles = triangles.has_value() ? triangles.value().build() : vk::GeometryTrianglesNV();
    VkType::aabbs = aabbs.has_value() ? aabbs.value().build() : vk::GeometryAABBNV();
  }

  std::optional<GeometryTrianglesNVChain> triangles;
  std::optional<GeometryAABBNVChain> aabbs;
};

class GeometryNV : public Structure<vk::GeometryNV> {
 public:
  void updateVkStructure() override {
    VkType::geometry = geometry.build();
  }

  using VkType::flags;
  using VkType::geometryType;
  GeometryDataNV geometry;
};

using GeometryNVChain = StructureChain<GeometryNV>;

class AccelerationStructureInfoNV : public Structure<vk::AccelerationStructureInfoNV> {
 public:
  void updateVkStructure() override {
    vecLogiToVk(geometries, vkGeometries_);
    vecToCArr(vkGeometries_, pGeometries, geometryCount);
  }

  using VkType::flags;
  using VkType::instanceCount;
  using VkType::type;
  std::vector<GeometryNVChain> geometries;

 private:
  std::vector<vk::GeometryNV> vkGeometries_;
};

using AccelerationStructureInfoNVChain = StructureChain<AccelerationStructureInfoNV>;

class AccelerationStructureCreateInfoNV : public Structure<vk::AccelerationStructureCreateInfoNV> {
 public:
  void updateVkStructure() override {
    VkType::info = info.build();
  }

  using VkType::compactedSize;
  AccelerationStructureInfoNVChain info;
};

using AccelerationStructureCreateInfoNVChain = StructureChain<AccelerationStructureCreateInfoNV>;

class AccelerationStructureMemoryRequirementsInfoNV
  : public Structure<vk::AccelerationStructureMemoryRequirementsInfoNV> {
 public:
  using VkType::accelerationStructure;
  using VkType::type;
};

using AccelerationStructureMemoryRequirementsInfoNVChain =
  StructureChain<AccelerationStructureMemoryRequirementsInfoNV>;

class BindAccelerationStructureMemoryInfoNV : public Structure<vk::BindAccelerationStructureMemoryInfoNV> {
 public:
  void updateVkStructure() override {
    vecToCArr(deviceIndices, pDeviceIndices, deviceIndexCount);
  }

  using VkType::accelerationStructure;
  using VkType::memory;
  using VkType::memoryOffset;
  std::vector<uint32_t> deviceIndices;
};

using BindAccelerationStructureMemoryInfoNVChain = StructureChain<BindAccelerationStructureMemoryInfoNV>;

class AcquireNextImageInfoKHR : public Structure<vk::AcquireNextImageInfoKHR> {
 public:
  using VkType::deviceMask;
  using VkType::fence;
  using VkType::semaphore;
  using VkType::swapchain;
  using VkType::timeout;
};

using AcquireNextImageInfoKHRChain = StructureChain<AcquireNextImageInfoKHR>;

class ApplicationInfo : public Structure<vk::ApplicationInfo> {
 public:
  void updateVkStructure() override {
    pApplicationName = applicationName.c_str();
    pEngineName = engineName.c_str();
  }

  using VkType::apiVersion;
  using VkType::applicationVersion;
  using VkType::engineVersion;
  std::string applicationName;
  std::string engineName;
};

using ApplicationInfoChain = StructureChain<ApplicationInfo>;

class DebugReportCallbackCreateInfoEXT : public Structure<vk::DebugReportCallbackCreateInfoEXT> {
 public:
  using VkType::flags;
  using VkType::pfnCallback;
  using VkType::pUserData;
};

class DebugUtilsLabelEXT : public Structure<vk::DebugUtilsLabelEXT> {
 public:
  void updateVkStructure() override {
    pLabelName = labelName.c_str();
  }

  std::string labelName;
  using VkType::color;
};

using DebugUtilsLabelEXTChain = StructureChain<DebugUtilsLabelEXT>;

class DebugUtilsMessengerCreateInfoEXT : public Structure<vk::DebugUtilsMessengerCreateInfoEXT> {
 public:
  using VkType::flags;
  using VkType::messageSeverity;
  using VkType::messageType;
  using VkType::pfnUserCallback;
  using VkType::pUserData;
};

class DebugUtilsObjectNameInfoEXT : public Structure<vk::DebugUtilsObjectNameInfoEXT> {
 public:
  void updateVkStructure() override {
    pObjectName = objectName.c_str();
  }

  using VkType::objectHandle;
  using VkType::objectType;
  std::string objectName;
};

using DebugUtilsObjectNameInfoEXTChain = StructureChain<DebugUtilsObjectNameInfoEXT>;

class DebugUtilsObjectTagInfoEXT : public Structure<vk::DebugUtilsObjectTagInfoEXT> {
 public:
  void updateVkStructure() override {
    pTag = tag.empty() ? nullptr : tag.data();
    tagSize = tag.size();
  }

  using VkType::objectHandle;
  using VkType::objectType;
  using VkType::tagName;
  std::vector<std::byte> tag;
};

using DebugUtilsObjectTagInfoEXTChain = StructureChain<DebugUtilsObjectTagInfoEXT>;

class ValidationFeaturesEXT : public Structure<vk::ValidationFeaturesEXT> {
 public:
  void updateVkStructure() override {
    vecToCArr(enabledValidationFeatures, pEnabledValidationFeatures, enabledValidationFeatureCount);
    vecToCArr(disabledValidationFeatures, pDisabledValidationFeatures, disabledValidationFeatureCount);
  }

  std::vector<vk::ValidationFeatureEnableEXT> enabledValidationFeatures;
  std::vector<vk::ValidationFeatureDisableEXT> disabledValidationFeatures;
};

class ValidationFlagsEXT : public Structure<vk::ValidationFlagsEXT> {
 public:
  void updateVkStructure() override {
    vecToCArr(disabledValidationChecks, pDisabledValidationChecks, disabledValidationCheckCount);
  }

  std::vector<vk::ValidationCheckEXT> disabledValidationChecks;
};

class InstanceCreateInfo : public Structure<vk::InstanceCreateInfo> {
 public:
  void updateVkStructure() override {
    pApplicationInfo = &applicationInfo.build();

    // Enabled layers.
    cStrEnabledLayerNames_.clear();
    cStrEnabledLayerNames_.reserve(enabledLayerNames.size());
    for (const auto& layer : enabledLayerNames) {
      cStrEnabledLayerNames_.emplace_back(layer.c_str());
    }
    vecToCArr(cStrEnabledLayerNames_, ppEnabledLayerNames, enabledLayerCount);

    // Enabled extensions.
    cStrEnabledExtensionNames_.clear();
    cStrEnabledExtensionNames_.reserve(enabledExtensionNames.size());
    for (const auto& layer : enabledExtensionNames) {
      cStrEnabledExtensionNames_.emplace_back(layer.c_str());
    }
    vecToCArr(cStrEnabledExtensionNames_, ppEnabledExtensionNames, enabledExtensionCount);
  }

  using VkType::flags;
  ApplicationInfoChain applicationInfo;
  std::vector<std::string> enabledLayerNames;
  std::vector<std::string> enabledExtensionNames;

 private:
  std::vector<const char*> cStrEnabledLayerNames_;
  std::vector<const char*> cStrEnabledExtensionNames_;
};

using InstanceCreateInfoChain =
  StructureChain<InstanceCreateInfo, DebugReportCallbackCreateInfoEXT, DebugUtilsMessengerCreateInfoEXT,
                 ValidationFeaturesEXT, ValidationFlagsEXT>;

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
  std::vector<Rect2D> splitInstanceBindRegions;
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

class BufferDeviceAddressInfoEXT : public Structure<vk::BufferDeviceAddressInfoEXT> {
 public:
  using VkType::buffer;
};

using BufferDeviceAddressInfoEXTChain = StructureChain<BufferDeviceAddressInfoEXT>;

class BufferMemoryBarrier : public Structure<vk::BufferMemoryBarrier> {
 public:
  using VkType::buffer;
  using VkType::dstAccessMask;
  using VkType::dstQueueFamilyIndex;
  using VkType::offset;
  using VkType::size;
  using VkType::srcAccessMask;
  using VkType::srcQueueFamilyIndex;
};

using BufferMemoryBarrierChain = StructureChain<BufferMemoryBarrier>;

class BufferMemoryRequirementsInfo2 : public Structure<vk::BufferMemoryRequirementsInfo2> {
 public:
  using VkType::buffer;
};

using BufferMemoryRequirementsInfo2Chain = StructureChain<BufferMemoryRequirementsInfo2>;

class BufferViewCreateInfo : public Structure<vk::BufferViewCreateInfo> {
 public:
  using VkType::buffer;
  using VkType::flags;
  using VkType::format;
  using VkType::offset;
  using VkType::range;
};

using BufferViewCreateInfoChain = StructureChain<BufferViewCreateInfo>;

class CalibratedTimestampInfoEXT : public Structure<vk::CalibratedTimestampInfoEXT> {
 public:
  using VkType::timeDomain;
};

using CalibratedTimestampInfoEXTChain = StructureChain<CalibratedTimestampInfoEXT>;

class CheckpointDataNV : public Structure<vk::CheckpointDataNV> {
 public:
  using VkType::pCheckpointMarker;
  using VkType::stage;
};

using CheckpointDataNVChain = StructureChain<CheckpointDataNV>;

class CmdProcessCommandsInfoNVX : public Structure<vk::CmdProcessCommandsInfoNVX> {
 public:
  void updateVkStructure() override {
    vecToCArr(indirectCommandsTokens, pIndirectCommandsTokens, indirectCommandsTokenCount);
  }

  using VkType::indirectCommandsLayout;
  using VkType::maxSequencesCount;
  using VkType::objectTable;
  using VkType::sequencesCountBuffer;
  using VkType::sequencesCountOffset;
  using VkType::sequencesIndexBuffer;
  using VkType::sequencesIndexOffset;
  using VkType::targetCommandBuffer;
  std::vector<IndirectCommandsTokenNVX> indirectCommandsTokens;
};

using CmdProcessCommandsInfoNVXChain = StructureChain<CmdProcessCommandsInfoNVX>;

class CmdReserveSpaceForCommandsInfoNVX : public Structure<vk::CmdReserveSpaceForCommandsInfoNVX> {
 public:
  using VkType::indirectCommandsLayout;
  using VkType::maxSequencesCount;
  using VkType::objectTable;
};

using CmdReserveSpaceForCommandsInfoNVXChain = StructureChain<CmdReserveSpaceForCommandsInfoNVX>;

class CommandBufferAllocateInfo : public Structure<vk::CommandBufferAllocateInfo> {
 public:
  using VkType::commandBufferCount;
  using VkType::commandPool;
  using VkType::level;
};

using CommandBufferAllocateInfoChain = StructureChain<CommandBufferAllocateInfo>;

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

class CommandPoolCreateInfo : public Structure<vk::CommandPoolCreateInfo> {
 public:
  using VkType::flags;
  using VkType::queueFamilyIndex;
};

using CommandPoolCreateInfoChain = StructureChain<CommandPoolCreateInfo>;

class DeviceGroupCommandBufferBeginInfo : public Structure<vk::DeviceGroupCommandBufferBeginInfo> {
 public:
  using VkType::deviceMask;
};

class CommandBufferBeginInfo : public Structure<vk::CommandBufferBeginInfo> {
 public:
  void updateVkStructure() override {
    pInheritanceInfo = &inheritanceInfo.build();
  }

  using VkType::flags;
  CommandBufferInheritanceInfoChain inheritanceInfo;
};

using CommandBufferBeginInfoChain = StructureChain<CommandBufferBeginInfo, DeviceGroupCommandBufferBeginInfo>;

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

class DescriptorUpdateTemplateCreateInfo : public Structure<vk::DescriptorUpdateTemplateCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(descriptorUpdateEntries, pDescriptorUpdateEntries, descriptorUpdateEntryCount);
  }

  using VkType::flags;
  std::vector<vk::DescriptorUpdateTemplateEntry> descriptorUpdateEntries;
  using VkType::descriptorSetLayout;
  using VkType::pipelineBindPoint;
  using VkType::pipelineLayout;
  using VkType::set;
  using VkType::templateType;
};

using DescriptorUpdateTemplateCreateInfoChain = StructureChain<DescriptorUpdateTemplateCreateInfo>;

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

class PhysicalDeviceImageViewImageFormatInfoEXT : public Structure<vk::PhysicalDeviceImageViewImageFormatInfoEXT> {
 public:
  using VkType::imageViewType;
};

class PhysicalDeviceImageDrmFormatModifierInfoEXT : public Structure<vk::PhysicalDeviceImageDrmFormatModifierInfoEXT> {
 public:
  void updateVkStructure() override {
    vecToCArr(queueFamilyIndices, pQueueFamilyIndices, queueFamilyIndexCount);
  }

  using VkType::drmFormatModifier;
  using VkType::sharingMode;
  std::vector<uint32_t> queueFamilyIndices;
};

class PhysicalDeviceExternalImageFormatInfo : public Structure<vk::PhysicalDeviceExternalImageFormatInfo> {
 public:
  using VkType::handleType;
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

  std::vector<vk::Format> viewFormats;
};

class PhysicalDeviceImageFormatInfo2 : public Structure<vk::PhysicalDeviceImageFormatInfo2> {
 public:
  using VkType::flags;
  using VkType::format;
  using VkType::tiling;
  using VkType::type;
  using VkType::usage;
};

using PhysicalDeviceImageFormatInfo2Chain =
  StructureChain<PhysicalDeviceImageFormatInfo2, ImageFormatListCreateInfoKHR, ImageStencilUsageCreateInfoEXT,
                 PhysicalDeviceExternalImageFormatInfo, PhysicalDeviceImageDrmFormatModifierInfoEXT,
                 PhysicalDeviceImageViewImageFormatInfoEXT>;

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

class QueueFamilyCheckpointPropertiesNV : public Structure<vk::QueueFamilyCheckpointPropertiesNV> {
 public:
  using VkType::checkpointExecutionStageMask;
};

class QueueFamilyProperties2 : public Structure<vk::QueueFamilyProperties2> {
 public:
  using VkType::queueFamilyProperties;
};

using QueueFamilyProperties2Chain = StructureChain<QueueFamilyProperties2, QueueFamilyCheckpointPropertiesNV>;

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

class DeviceQueueInfo2 : public Structure<vk::DeviceQueueInfo2> {
 public:
  using VkType::flags;
  using VkType::queueFamilyIndex;
  using VkType::queueIndex;
};

using DeviceQueueInfo2Chain = StructureChain<DeviceQueueInfo2>;

class DisplayEventInfoEXT : public Structure<vk::DisplayEventInfoEXT> {
 public:
  using VkType::displayEvent;
};

using DisplayEventInfoEXTChain = StructureChain<DisplayEventInfoEXT>;

class DisplayModeCreateInfoKHR : public Structure<vk::DisplayModeCreateInfoKHR> {
 public:
  using VkType::flags;
  using VkType::parameters;
};

using DisplayModeCreateInfoKHRChain = StructureChain<DisplayModeCreateInfoKHR>;

class DisplayModeProperties2KHR : public Structure<vk::DisplayModeProperties2KHR> {
 public:
  using VkType::displayModeProperties;
};

using DisplayModeProperties2KHRChain = StructureChain<DisplayModeProperties2KHR>;

class DeviceMemoryOverallocationCreateInfoAMD : public Structure<vk::DeviceMemoryOverallocationCreateInfoAMD> {
 public:
  using VkType::overallocationBehavior;
};

class DeviceGroupDeviceCreateInfo : public Structure<vk::DeviceGroupDeviceCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(physicalDevices, pPhysicalDevices, physicalDeviceCount);
  }

  std::vector<vk::PhysicalDevice> physicalDevices;
};

class DeviceCreateInfo : public Structure<vk::DeviceCreateInfo> {
 public:
  void updateVkStructure() override {
    // Queue create infos
    vecLogiToVk(queueCreateInfos, vkQueueCreateInfos_);
    vecToCArr(vkQueueCreateInfos_, pQueueCreateInfos, queueCreateInfoCount);

    // Enabled layers.
    cStrEnabledLayerNames_.clear();
    cStrEnabledLayerNames_.reserve(enabledLayerNames.size());
    for (const auto& layer : enabledLayerNames) {
      cStrEnabledLayerNames_.emplace_back(layer.c_str());
    }
    vecToCArr(cStrEnabledLayerNames_, ppEnabledLayerNames, enabledLayerCount);

    // Enabled extensions.
    cStrEnabledExtensionNames_.clear();
    cStrEnabledExtensionNames_.reserve(enabledExtensionNames.size());
    for (const auto& layer : enabledExtensionNames) {
      cStrEnabledExtensionNames_.emplace_back(layer.c_str());
    }
    vecToCArr(cStrEnabledExtensionNames_, ppEnabledExtensionNames, enabledExtensionCount);

    // Features
    pEnabledFeatures = &enabledFeatures;
  }

  using VkType::flags;

  std::vector<DeviceQueueCreateInfoChain> queueCreateInfos;
  std::vector<std::string> enabledLayerNames;
  std::vector<std::string> enabledExtensionNames;
  vk::PhysicalDeviceFeatures enabledFeatures;

 private:
  std::vector<vk::DeviceQueueCreateInfo> vkQueueCreateInfos_;
  std::vector<const char*> cStrEnabledLayerNames_;
  std::vector<const char*> cStrEnabledExtensionNames_;
};

using DeviceCreateInfoChain =
  StructureChain<DeviceCreateInfo, DeviceGroupDeviceCreateInfo, DeviceMemoryOverallocationCreateInfoAMD,
                 PhysicalDeviceFeatures2, PhysicalDevice16BitStorageFeatures, PhysicalDevice8BitStorageFeaturesKHR,
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

class DeviceEventInfoEXT : public Structure<vk::DeviceEventInfoEXT> {
 public:
  using VkType::deviceEvent;
};

using DeviceEventInfoEXTChain = StructureChain<DeviceEventInfoEXT>;

class DeviceGeneratedCommandsFeaturesNVX : public Structure<vk::DeviceGeneratedCommandsFeaturesNVX> {
 public:
  using VkType::computeBindingPointSupport;
};

using DeviceGeneratedCommandsFeaturesNVXCHain = StructureChain<DeviceGeneratedCommandsFeaturesNVX>;

class DeviceGeneratedCommandsLimitsNVX : public Structure<vk::DeviceGeneratedCommandsLimitsNVX> {
 public:
  using VkType::maxIndirectCommandsLayoutTokenCount;
  using VkType::maxObjectEntryCounts;
  using VkType::minCommandsTokenBufferOffsetAlignment;
  using VkType::minSequenceCountBufferOffsetAlignment;
  using VkType::minSequenceIndexBufferOffsetAlignment;
};

using DeviceGeneratedCommandsLimitsNVXCHain = StructureChain<DeviceGeneratedCommandsLimitsNVX>;

class DeviceGroupPresentCapabilitiesKHR : public Structure<vk::DeviceGroupPresentCapabilitiesKHR> {
 public:
  using VkType::modes;
  using VkType::presentMask;
};

using DeviceGroupPresentCapabilitiesKHRChain = StructureChain<DeviceGroupPresentCapabilitiesKHR>;

class ExportFenceCreateInfo : public Structure<vk::ExportFenceCreateInfo> {
 public:
  using VkType::handleTypes;
};

class FenceCreateInfo : public Structure<vk::FenceCreateInfo> {
 public:
  using VkType::flags;
};

using FenceCreateInfoChain = StructureChain<FenceCreateInfo, ExportFenceCreateInfo>;

class ExportSemaphoreCreateInfo : public Structure<vk::ExportSemaphoreCreateInfo> {
 public:
  using VkType::handleTypes;
};

class SemaphoreCreateInfo : public Structure<vk::SemaphoreCreateInfo> {
 public:
  using VkType::flags;
};

using SemaphoreCreateInfoChain = StructureChain<SemaphoreCreateInfo, ExportSemaphoreCreateInfo>;

class SemaphoreGetFdInfoKHR : public Structure<vk::SemaphoreGetFdInfoKHR> {
 public:
  using VkType::handleType;
  using VkType::semaphore;
};

using SemaphoreGetFdInfoKHRChain = StructureChain<SemaphoreGetFdInfoKHR>;

class FormatProperties2 : public Structure<vk::FormatProperties2> {
 public:
  using VkType::formatProperties;
};

using FormatProperties2Chain = StructureChain<FormatProperties2>;

class ImageSwapchainCreateInfoKHR : public Structure<vk::ImageSwapchainCreateInfoKHR> {
 public:
  using VkType::swapchain;
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

class TextureLODGatherFormatPropertiesAMD : public Structure<vk::TextureLODGatherFormatPropertiesAMD> {
 public:
  using VkType::supportsTextureGatherLODBiasAMD;
};

class SamplerReductionModeCreateInfoEXT : public Structure<vk::SamplerReductionModeCreateInfoEXT> {
 public:
  using VkType::reductionMode;
};

class SamplerYcbcrConversionCreateInfo : public Structure<vk::SamplerYcbcrConversionCreateInfo> {
 public:
  using VkType::chromaFilter;
  using VkType::components;
  using VkType::forceExplicitReconstruction;
  using VkType::format;
  using VkType::xChromaOffset;
  using VkType::ycbcrModel;
  using VkType::ycbcrRange;
  using VkType::yChromaOffset;
};

class SamplerCreateInfo : public Structure<vk::SamplerCreateInfo> {
 public:
  using VkType::addressModeU;
  using VkType::addressModeV;
  using VkType::addressModeW;
  using VkType::anisotropyEnable;
  using VkType::borderColor;
  using VkType::compareEnable;
  using VkType::compareOp;
  using VkType::flags;
  using VkType::magFilter;
  using VkType::maxAnisotropy;
  using VkType::maxLod;
  using VkType::minFilter;
  using VkType::minLod;
  using VkType::mipLodBias;
  using VkType::mipmapMode;
  using VkType::unnormalizedCoordinates;
};

using SamplerCreateInfoChain =
  StructureChain<SamplerCreateInfo, SamplerReductionModeCreateInfoEXT, SamplerYcbcrConversionCreateInfo>;

class SamplerYcbcrConversionImageFormatProperties : public Structure<vk::SamplerYcbcrConversionImageFormatProperties> {
 public:
  using VkType::combinedImageSamplerDescriptorCount;
};

class FilterCubicImageViewImageFormatPropertiesEXT
  : public Structure<vk::FilterCubicImageViewImageFormatPropertiesEXT> {
 public:
  using VkType::filterCubic;
  using VkType::filterCubicMinmax;
};

class ExternalImageFormatProperties : public Structure<vk::ExternalImageFormatProperties> {
 public:
  using VkType::externalMemoryProperties;
};

class ImageFormatProperties2 : public Structure<vk::ImageFormatProperties2> {
 public:
  using VkType::imageFormatProperties;
};

using ImageFormatProperties2Chain =
  StructureChain<ImageFormatProperties2, ExternalImageFormatProperties, FilterCubicImageViewImageFormatPropertiesEXT,
                 SamplerYcbcrConversionImageFormatProperties, TextureLODGatherFormatPropertiesAMD>;

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
    vecLogiToVk(bindings, vkBindings_);
    vecToCArr(vkBindings_, pBindings, bindingCount);
  }

  using VkType::flags;
  std::vector<DescriptorSetLayoutBinding> bindings;

 private:
  std::vector<vk::DescriptorSetLayoutBinding> vkBindings_;
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

class ShaderModuleValidationCacheCreateInfoEXT : public Structure<vk::ShaderModuleValidationCacheCreateInfoEXT> {
 public:
  using VkType::validationCache;
};

class ShaderModuleCreateInfo : public Structure<vk::ShaderModuleCreateInfo> {
 public:
  void updateVkStructure() override {
    pCode = code.empty() ? nullptr : reinterpret_cast<const uint32_t*>(code.data());
    codeSize = code.size();
  }

  using VkType::flags;
  std::vector<std::byte> code;
};

using ShaderModuleCreateInfoChain = StructureChain<ShaderModuleCreateInfo, ShaderModuleValidationCacheCreateInfoEXT>;

class PipelineShaderStageCreateInfo : public Structure<vk::PipelineShaderStageCreateInfo> {
 public:
  void updateVkStructure() override {
    pName = name.data();
    pSpecializationInfo = &specializationInfo.build();
  }

  using VkType::flags;
  using VkType::module;
  using VkType::stage;
  std::string name = "main";
  SpecializationInfo specializationInfo;
};

using PipelineShaderStageCreateInfoChain = StructureChain<PipelineShaderStageCreateInfo>;

class ComputePipelineCreateInfo : public Structure<vk::ComputePipelineCreateInfo> {
 public:
  void updateVkStructure() override {
    VkType::stage = stage.build();
  }

  using VkType::basePipelineHandle;
  using VkType::basePipelineIndex;
  using VkType::flags;
  using VkType::layout;
  PipelineShaderStageCreateInfoChain stage;
};

using ComputePipelineCreateInfoChain = StructureChain<ComputePipelineCreateInfo>;

class ConditionalRenderingBeginInfoEXT : public Structure<vk::ConditionalRenderingBeginInfoEXT> {
 public:
  using VkType::buffer;
  using VkType::flags;
  using VkType::offset;
};

using ConditionalRenderingBeginInfoEXTChain = StructureChain<ConditionalRenderingBeginInfoEXT>;

class CooperativeMatrixPropertiesNV : public Structure<vk::CooperativeMatrixPropertiesNV> {
 public:
  using VkType::AType;
  using VkType::BType;
  using VkType::CType;
  using VkType::DType;
  using VkType::KSize;
  using VkType::MSize;
  using VkType::NSize;
  using VkType::scope;
};

using CooperativeMatrixPropertiesNVChain = StructureChain<CooperativeMatrixPropertiesNV>;

class CopyDescriptorSet : public Structure<vk::CopyDescriptorSet> {
 public:
  using VkType::descriptorCount;
  using VkType::dstArrayElement;
  using VkType::dstBinding;
  using VkType::dstSet;
  using VkType::srcArrayElement;
  using VkType::srcBinding;
  using VkType::srcSet;
};

using CopyDescriptorSetChain = StructureChain<CopyDescriptorSet>;

class DebugMarkerMarkerInfoEXT : public Structure<vk::DebugMarkerMarkerInfoEXT> {
 public:
  void updateVkStructure() override {
    pMarkerName = markerName.c_str();
  }

  std::string markerName;
  using VkType::color;
};

using DebugMarkerMarkerInfoEXTChain = StructureChain<DebugMarkerMarkerInfoEXT>;

class DebugMarkerObjectNameInfoEXT : public Structure<vk::DebugMarkerObjectNameInfoEXT> {
 public:
  void updateVkStructure() override {
    pObjectName = objectName.c_str();
  }

  using VkType::object;
  using VkType::objectType;
  std::string objectName;
};

using DebugMarkerObjectNameInfoEXTChain = StructureChain<DebugMarkerObjectNameInfoEXT>;

class DebugMarkerObjectTagInfoEXT : public Structure<vk::DebugMarkerObjectTagInfoEXT> {
 public:
  void updateVkStructure() override {
    pTag = tag.empty() ? nullptr : tag.data();
    tagSize = tag.size();
  }

  using VkType::object;
  using VkType::objectType;
  using VkType::tagName;
  std::vector<std::byte> tag;
};

using DebugMarkerObjectTagInfoEXTChain = StructureChain<DebugMarkerObjectTagInfoEXT>;

class PipelineVertexInputDivisorStateCreateInfoEXT
  : public Structure<vk::PipelineVertexInputDivisorStateCreateInfoEXT> {
 public:
  void updateVkStructure() override {
    vecToCArr(vertexBindingDivisors, pVertexBindingDivisors, vertexBindingDivisorCount);
  }

  std::vector<vk::VertexInputBindingDivisorDescriptionEXT> vertexBindingDivisors;
};

class PipelineVertexInputStateCreateInfo : public Structure<vk::PipelineVertexInputStateCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(vertexBindingDescription, pVertexBindingDescriptions, vertexBindingDescriptionCount);
    vecToCArr(vertexAttributeDescriptions, pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
  }

  using VkType::flags;

  std::vector<vk::VertexInputBindingDescription> vertexBindingDescription;
  std::vector<vk::VertexInputAttributeDescription> vertexAttributeDescriptions;
};

using PipelineVertexInputStateCreateInfoChain =
  StructureChain<PipelineVertexInputStateCreateInfo, PipelineVertexInputDivisorStateCreateInfoEXT>;

class PipelineInputAssemblyStateCreateInfo : public Structure<vk::PipelineInputAssemblyStateCreateInfo> {
 public:
  using VkType::flags;
  using VkType::primitiveRestartEnable;
  using VkType::topology;
};

using PipelineInputAssemblyStateCreateInfoChain = StructureChain<PipelineInputAssemblyStateCreateInfo>;

class PipelineTessellationDomainOriginStateCreateInfo
  : public Structure<vk::PipelineTessellationDomainOriginStateCreateInfo> {
 public:
  using VkType::domainOrigin;
};

class PipelineTessellationStateCreateInfo : public Structure<vk::PipelineTessellationStateCreateInfo> {
 public:
  using VkType::flags;
  using VkType::patchControlPoints;
};

using PipelineTessellationStateCreateInfoChain =
  StructureChain<PipelineTessellationStateCreateInfo, PipelineTessellationDomainOriginStateCreateInfo>;

class PipelineViewportCoarseSampleOrderStateCreateInfoNV
  : public Structure<vk::PipelineViewportCoarseSampleOrderStateCreateInfoNV> {
 public:
  void updateVkStructure() override {
    vecToCArr(customSampleOrders, pCustomSampleOrders, customSampleOrderCount);
  }

  using VkType::sampleOrderType;
  std::vector<vk::CoarseSampleOrderCustomNV> customSampleOrders;
};

class PipelineViewportExclusiveScissorStateCreateInfoNV
  : public Structure<vk::PipelineViewportExclusiveScissorStateCreateInfoNV> {
 public:
  void updateVkStructure() override {
    vecToCArr(exclusiveScissors, pExclusiveScissors, exclusiveScissorCount);
  }

  std::vector<vk::Rect2D> exclusiveScissors;
};

class PipelineViewportShadingRateImageStateCreateInfoNV
  : public Structure<vk::PipelineViewportShadingRateImageStateCreateInfoNV> {
 public:
  void updateVkStructure() override {
    pShadingRatePalettes = shadingRatePalettes.empty() ? nullptr : shadingRatePalettes.data();
  }

  using VkType::shadingRateImageEnable;
  using VkType::viewportCount;
  std::vector<vk::ShadingRatePaletteNV> shadingRatePalettes;
};

class PipelineViewportSwizzleStateCreateInfoNV : public Structure<vk::PipelineViewportSwizzleStateCreateInfoNV> {
 public:
  void updateVkStructure() override {
    vecToCArr(viewportSwizzles, pViewportSwizzles, viewportCount);
  }

  using VkType::flags;
  std::vector<vk::ViewportSwizzleNV> viewportSwizzles;
};

class PipelineViewportWScalingStateCreateInfoNV : public Structure<vk::PipelineViewportWScalingStateCreateInfoNV> {
 public:
  void updateVkStructure() override {
    pViewportWScalings = viewportWScalings.empty() ? nullptr : viewportWScalings.data();
  }

  using VkType::viewportCount;
  using VkType::viewportWScalingEnable;
  std::vector<vk::ViewportWScalingNV> viewportWScalings;
};

class PipelineViewportStateCreateInfo : public Structure<vk::PipelineViewportStateCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(viewports, pViewports, viewportCount);
    vecToCArr(scissors, pScissors, scissorCount);
  }

  using VkType::flags;
  std::vector<vk::Viewport> viewports;
  std::vector<vk::Rect2D> scissors;
};

using PipelineViewportStateCreateInfoChain =
  StructureChain<PipelineViewportStateCreateInfo, PipelineViewportWScalingStateCreateInfoNV,
                 PipelineViewportSwizzleStateCreateInfoNV, PipelineViewportShadingRateImageStateCreateInfoNV,
                 PipelineViewportExclusiveScissorStateCreateInfoNV, PipelineViewportCoarseSampleOrderStateCreateInfoNV>;

class PipelineRasterizationStateStreamCreateInfoEXT
  : public Structure<vk::PipelineRasterizationStateStreamCreateInfoEXT> {
 public:
  using VkType::flags;
  using VkType::rasterizationStream;
};

class PipelineRasterizationStateRasterizationOrderAMD
  : public Structure<vk::PipelineRasterizationStateRasterizationOrderAMD> {
 public:
  using VkType::rasterizationOrder;
};

class PipelineRasterizationDepthClipStateCreateInfoEXT
  : public Structure<vk::PipelineRasterizationDepthClipStateCreateInfoEXT> {
 public:
  using VkType::depthClipEnable;
  using VkType::flags;
};

class PipelineRasterizationConservativeStateCreateInfoEXT
  : public Structure<vk::PipelineRasterizationConservativeStateCreateInfoEXT> {
 public:
  using VkType::conservativeRasterizationMode;
  using VkType::extraPrimitiveOverestimationSize;
  using VkType::flags;
};

class PipelineRasterizationStateCreateInfo : public Structure<vk::PipelineRasterizationStateCreateInfo> {
 public:
  using VkType::cullMode;
  using VkType::depthBiasClamp;
  using VkType::depthBiasConstantFactor;
  using VkType::depthBiasEnable;
  using VkType::depthBiasSlopeFactor;
  using VkType::depthClampEnable;
  using VkType::flags;
  using VkType::frontFace;
  using VkType::lineWidth;
  using VkType::polygonMode;
  using VkType::rasterizerDiscardEnable;
};

using PipelineRasterizationStateCreateInfoChain =
  StructureChain<PipelineRasterizationStateCreateInfo, PipelineRasterizationConservativeStateCreateInfoEXT,
                 PipelineRasterizationDepthClipStateCreateInfoEXT, PipelineRasterizationStateRasterizationOrderAMD,
                 PipelineRasterizationStateStreamCreateInfoEXT>;

class ImageMemoryBarrier : public Structure<vk::ImageMemoryBarrier> {
 public:
  using VkType::dstAccessMask;
  using VkType::dstQueueFamilyIndex;
  using VkType::image;
  using VkType::newLayout;
  using VkType::oldLayout;
  using VkType::srcAccessMask;
  using VkType::srcQueueFamilyIndex;
  using VkType::subresourceRange;
};

using ImageMemoryBarrierChain = StructureChain<ImageMemoryBarrier, SampleLocationsInfoEXT>;

class ImagePlaneMemoryRequirementsInfo : public Structure<vk::ImagePlaneMemoryRequirementsInfo> {
 public:
  using VkType::planeAspect;
};

class ImageMemoryRequirementsInfo2 : public Structure<vk::ImageMemoryRequirementsInfo2> {
 public:
  using VkType::image;
};

using ImageMemoryRequirementsInfo2Chain =
  StructureChain<ImageMemoryRequirementsInfo2, ImagePlaneMemoryRequirementsInfo>;

class SamplerYcbcrConversionInfo : public Structure<vk::SamplerYcbcrConversionInfo> {
 public:
  using VkType::conversion;
};

class ImageViewUsageCreateInfo : public Structure<vk::ImageViewUsageCreateInfo> {
 public:
  using VkType::usage;
};

class ImageViewASTCDecodeModeEXT : public Structure<vk::ImageViewASTCDecodeModeEXT> {
 public:
  using VkType::decodeMode;
};

class ImageViewCreateInfo : public Structure<vk::ImageViewCreateInfo> {
 public:
  using VkType::components;
  using VkType::flags;
  using VkType::format;
  using VkType::image;
  using VkType::subresourceRange;
  using VkType::viewType;
};

using ImageViewCreateInfoChain =
  StructureChain<ImageViewCreateInfo, ImageViewASTCDecodeModeEXT, ImageViewUsageCreateInfo, SamplerYcbcrConversionInfo>;

class MemoryPriorityAllocateInfoEXT : public Structure<vk::MemoryPriorityAllocateInfoEXT> {
 public:
  using VkType::priority;
};

class MemoryDedicatedAllocateInfo : public Structure<vk::MemoryDedicatedAllocateInfo> {
 public:
  using VkType::buffer;
  using VkType::image;
};

class MemoryAllocateFlagsInfo : public Structure<vk::MemoryAllocateFlagsInfo> {
 public:
  using VkType::deviceMask;
  using VkType::flags;
};

class ImportMemoryHostPointerInfoEXT : public Structure<vk::ImportMemoryHostPointerInfoEXT> {
 public:
  using VkType::handleType;
  using VkType::pHostPointer;
};

class ImportMemoryFdInfoKHR : public Structure<vk::ImportMemoryFdInfoKHR> {
 public:
  using VkType::fd;
  using VkType::handleType;
};

class ExportMemoryAllocateInfoNV : public Structure<vk::ExportMemoryAllocateInfoNV> {
 public:
  using VkType::handleTypes;
};

class ExportMemoryAllocateInfo : public Structure<vk::ExportMemoryAllocateInfo> {
 public:
  using VkType::handleTypes;
};

class DedicatedAllocationMemoryAllocateInfoNV : public Structure<vk::DedicatedAllocationMemoryAllocateInfoNV> {
 public:
  using VkType::buffer;
  using VkType::image;
};

class MemoryAllocateInfo : public Structure<vk::MemoryAllocateInfo> {
 public:
  using VkType::allocationSize;
  using VkType::memoryTypeIndex;
};

using MemoryAllocateInfoChain =
  StructureChain<MemoryAllocateInfo, DedicatedAllocationMemoryAllocateInfoNV, ExportMemoryAllocateInfo,
                 ExportMemoryAllocateInfoNV, ImportMemoryFdInfoKHR, ImportMemoryHostPointerInfoEXT,
                 MemoryAllocateFlagsInfo, MemoryDedicatedAllocateInfo, MemoryPriorityAllocateInfoEXT>;

class MemoryDedicatedRequirements : public Structure<vk::MemoryDedicatedRequirements> {
 public:
  using VkType::prefersDedicatedAllocation;
  using VkType::requiresDedicatedAllocation;
};

class MemoryRequirements2 : public Structure<vk::MemoryRequirements2> {
 public:
  using VkType::memoryRequirements;
};

using MemoryRequirements2Chain = StructureChain<MemoryRequirements2, MemoryDedicatedRequirements>;

class PipelineSampleLocationsStateCreateInfoEXT : public Structure<vk::PipelineSampleLocationsStateCreateInfoEXT> {
 public:
  void updateVkStructure() override {
    VkType::sampleLocationsInfo = sampleLocationsInfo.build();
  }

  using VkType::sampleLocationsEnable;
  SampleLocationsInfoEXT sampleLocationsInfo;
};

class PipelineCoverageModulationStateCreateInfoNV : public Structure<vk::PipelineCoverageModulationStateCreateInfoNV> {
 public:
  void updateVkStructure() override {
    vecToCArr(coverageModulationTable, pCoverageModulationTable, coverageModulationTableCount);
  }

  using VkType::coverageModulationMode;
  using VkType::coverageModulationTableEnable;
  using VkType::flags;
  std::vector<float> coverageModulationTable;
};

class PipelineCoverageToColorStateCreateInfoNV : public Structure<vk::PipelineCoverageToColorStateCreateInfoNV> {
 public:
  using VkType::coverageToColorEnable;
  using VkType::coverageToColorLocation;
  using VkType::flags;
};

class PipelineMultisampleStateCreateInfo : public Structure<vk::PipelineMultisampleStateCreateInfo> {
 public:
  void updateVkStructure() override {
    pSampleMask = sampleMask.empty() ? nullptr : sampleMask.data();
  }

  using VkType::alphaToCoverageEnable;
  using VkType::alphaToOneEnable;
  using VkType::flags;
  using VkType::minSampleShading;
  using VkType::rasterizationSamples;
  using VkType::sampleShadingEnable;

  std::vector<vk::SampleMask> sampleMask;
};

using PipelineMultisampleStateCreateInfoChain =
  StructureChain<PipelineMultisampleStateCreateInfo, PipelineCoverageToColorStateCreateInfoNV,
                 PipelineCoverageModulationStateCreateInfoNV, PipelineSampleLocationsStateCreateInfoEXT>;

class PipelineDepthStencilStateCreateInfo : public Structure<vk::PipelineDepthStencilStateCreateInfo> {
 public:
  using VkType::back;
  using VkType::depthBoundsTestEnable;
  using VkType::depthCompareOp;
  using VkType::depthTestEnable;
  using VkType::depthWriteEnable;
  using VkType::flags;
  using VkType::front;
  using VkType::maxDepthBounds;
  using VkType::minDepthBounds;
  using VkType::stencilTestEnable;
};

using PipelineDepthStencilStateCreateInfoChain = StructureChain<PipelineDepthStencilStateCreateInfo>;

class PipelineColorBlendAdvancedStateCreateInfoEXT
  : public Structure<vk::PipelineColorBlendAdvancedStateCreateInfoEXT> {
 public:
  using VkType::blendOverlap;
  using VkType::dstPremultiplied;
  using VkType::srcPremultiplied;
};

class PipelineColorBlendStateCreateInfo : public Structure<vk::PipelineColorBlendStateCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(attachments, pAttachments, attachmentCount);
  }

  using VkType::blendConstants;
  using VkType::flags;
  using VkType::logicOp;
  using VkType::logicOpEnable;

  std::vector<vk::PipelineColorBlendAttachmentState> attachments;
};

using PipelineColorBlendStateCreateInfoChain =
  StructureChain<PipelineColorBlendStateCreateInfo, PipelineColorBlendAdvancedStateCreateInfoEXT>;

class PipelineDynamicStateCreateInfo : public Structure<vk::PipelineDynamicStateCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(dynamicStates, pDynamicStates, dynamicStateCount);
  }

  using VkType::flags;
  std::vector<vk::DynamicState> dynamicStates;
};

using PipelineDynamicStateCreateInfoChain = StructureChain<PipelineDynamicStateCreateInfo>;

class PipelineRepresentativeFragmentTestStateCreateInfoNV
  : public Structure<vk::PipelineRepresentativeFragmentTestStateCreateInfoNV> {
 public:
  using VkType::representativeFragmentTestEnable;
};

class PipelineDiscardRectangleStateCreateInfoEXT : public Structure<vk::PipelineDiscardRectangleStateCreateInfoEXT> {
 public:
  void updateVkStructure() override {
    vecToCArr(discardRectangles, pDiscardRectangles, discardRectangleCount);
  }

  using VkType::discardRectangleMode;
  using VkType::flags;
  std::vector<vk::Rect2D> discardRectangles;
};

class GraphicsPipelineCreateInfo : public Structure<vk::GraphicsPipelineCreateInfo> {
 public:
  void updateVkStructure() override {
    // TODO: Optimize this. Build only when needed.
    vecLogiToVk(stages, vkStages_);
    pVertexInputState = &vertexInputState.build();
    pInputAssemblyState = &inputAssemblyState.build();
    pTessellationState = &tessellationState.build();
    pViewportState = &viewportState.build();
    pRasterizationState = &rasterizationState.build();
    pMultisampleState = &multisampleState.build();
    pDepthStencilState = &depthStencilState.build();
    pColorBlendState = &colorBlendState.build();
    pDynamicState = &dynamicState.build();
  }

  using VkType::basePipelineHandle;
  using VkType::basePipelineIndex;
  using VkType::flags;
  using VkType::layout;
  using VkType::renderPass;
  using VkType::subpass;

  std::vector<PipelineShaderStageCreateInfoChain> stages;
  PipelineVertexInputStateCreateInfoChain vertexInputState;
  PipelineInputAssemblyStateCreateInfoChain inputAssemblyState;
  PipelineTessellationStateCreateInfoChain tessellationState;
  PipelineViewportStateCreateInfoChain viewportState;
  PipelineRasterizationStateCreateInfoChain rasterizationState;
  PipelineMultisampleStateCreateInfoChain multisampleState;
  PipelineDepthStencilStateCreateInfoChain depthStencilState;
  PipelineColorBlendStateCreateInfoChain colorBlendState;
  PipelineDynamicStateCreateInfoChain dynamicState;

 private:
  std::vector<vk::PipelineShaderStageCreateInfo> vkStages_;
};

using GraphicsPipelineCreateInfoChain =
  StructureChain<GraphicsPipelineCreateInfo, PipelineDiscardRectangleStateCreateInfoEXT,
                 PipelineRepresentativeFragmentTestStateCreateInfoNV>;

class RenderPassFragmentDensityMapCreateInfoEXT : public Structure<vk::RenderPassFragmentDensityMapCreateInfoEXT> {
 public:
  using VkType::fragmentDensityMapAttachment;
};

class RenderPassInputAttachmentAspectCreateInfo : public Structure<vk::RenderPassInputAttachmentAspectCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(aspectReferences, pAspectReferences, aspectReferenceCount);
  }

  std::vector<vk::InputAttachmentAspectReference> aspectReferences;
};

class RenderPassMultiviewCreateInfo : public Structure<vk::RenderPassMultiviewCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(viewMasks, pViewMasks, subpassCount);
    vecToCArr(viewOffsets, pViewOffsets, dependencyCount);
    vecToCArr(correlationMasks, pCorrelationMasks, correlationMaskCount);
  }

  std::vector<uint32_t> viewMasks;
  std::vector<int32_t> viewOffsets;
  std::vector<uint32_t> correlationMasks;
};

class RenderPassCreateInfo : public Structure<vk::RenderPassCreateInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(attachments, pAttachments, attachmentCount);
    vecLogiToVk(subpasses, vkSubpasses_);
    vecToCArr(vkSubpasses_, pSubpasses, subpassCount);
    vecToCArr(dependencies, pDependencies, dependencyCount);
  }

  using VkType::flags;
  std::vector<vk::AttachmentDescription> attachments;
  std::vector<SubpassDescription> subpasses;
  std::vector<vk::SubpassDependency> dependencies;

 private:
  std::vector<vk::SubpassDescription> vkSubpasses_;
};

using RenderPassCreateInfoChain =
  StructureChain<RenderPassCreateInfo, RenderPassFragmentDensityMapCreateInfoEXT,
                 RenderPassInputAttachmentAspectCreateInfo, RenderPassMultiviewCreateInfo>;

class SubpassBeginInfoKHR : public Structure<vk::SubpassBeginInfoKHR> {
 public:
  using VkType::contents;
};

using SubpassBeginInfoKHRChain = StructureChain<SubpassBeginInfoKHR>;

class SubpassEndInfoKHR : public Structure<vk::SubpassEndInfoKHR> {};

using SubpassEndInfoKHRChain = StructureChain<SubpassEndInfoKHR>;

class AttachmentDescription2KHR : public Structure<vk::AttachmentDescription2KHR> {
 public:
  using VkType::finalLayout;
  using VkType::flags;
  using VkType::format;
  using VkType::initialLayout;
  using VkType::loadOp;
  using VkType::samples;
  using VkType::stencilLoadOp;
  using VkType::stencilStoreOp;
  using VkType::storeOp;
};

using AttachmentDescription2KHRChain = StructureChain<AttachmentDescription2KHR>;

class AttachmentReference2KHR : public Structure<vk::AttachmentReference2KHR> {
 public:
  using VkType::aspectMask;
  using VkType::attachment;
  using VkType::layout;
};

using AttachmentReference2KHRChain = StructureChain<AttachmentReference2KHR>;

class SubpassDependency2KHR : public Structure<vk::SubpassDependency2KHR> {
 public:
  using VkType::dependencyFlags;
  using VkType::dstAccessMask;
  using VkType::dstStageMask;
  using VkType::dstSubpass;
  using VkType::srcAccessMask;
  using VkType::srcStageMask;
  using VkType::srcSubpass;
  using VkType::viewOffset;
};

using SubpassDependency2KHRChain = StructureChain<SubpassDependency2KHR>;

class SubpassDescriptionDepthStencilResolveKHR : public Structure<vk::SubpassDescriptionDepthStencilResolveKHR> {
 public:
  void updateVkStructure() override {
    pDepthStencilResolveAttachment =
      depthStencilResolveAttachment.has_value() ? &depthStencilResolveAttachment.value().build() : nullptr;
  }

  using VkType::depthResolveMode;
  using VkType::stencilResolveMode;
  std::optional<AttachmentReference2KHRChain> depthStencilResolveAttachment;
};

class SubpassDescription2KHR : public Structure<vk::SubpassDescription2KHR> {
 public:
  void updateVkStructure() override {
    // Input attachments
    vecLogiToVk(inputAttachments, vkInputAttachments_);
    vecToCArr(vkInputAttachments_, pInputAttachments, inputAttachmentCount);

    // Color attachments
    vecLogiToVk(colorAttachments, vkColorAttachments_);
    vecToCArr(vkColorAttachments_, pColorAttachments, colorAttachmentCount);

    // Resolve attachments
    vecLogiToVk(resolveAttachments, vkResolveAttachments_);
    pResolveAttachments = vkResolveAttachments_.empty() ? nullptr : vkResolveAttachments_.data();

    // Depth stencil attachment
    if (depthStencilAttachment.has_value()) {
      vkDepthStencilAttachment_ = depthStencilAttachment.value().build();
      pDepthStencilAttachment = &vkDepthStencilAttachment_;
    } else {
      pDepthStencilAttachment = nullptr;
    }

    // Preserve attachments
    vecToCArr(preserveAttachments, pPreserveAttachments, preserveAttachmentCount);
  }

  using VkType::flags;
  using VkType::pipelineBindPoint;
  using VkType::viewMask;

  std::vector<AttachmentReference2KHRChain> inputAttachments;
  std::vector<AttachmentReference2KHRChain> colorAttachments;
  std::vector<AttachmentReference2KHRChain> resolveAttachments;
  std::optional<AttachmentReference2KHRChain> depthStencilAttachment;
  std::vector<uint32_t> preserveAttachments;

 private:
  std::vector<vk::AttachmentReference2KHR> vkInputAttachments_;
  std::vector<vk::AttachmentReference2KHR> vkColorAttachments_;
  std::vector<vk::AttachmentReference2KHR> vkResolveAttachments_;
  vk::AttachmentReference2KHR vkDepthStencilAttachment_;
};

using SubpassDescription2KHRChain = StructureChain<SubpassDescription2KHR, SubpassDescriptionDepthStencilResolveKHR>;

class RenderPassCreateInfo2KHR : public Structure<vk::RenderPassCreateInfo2KHR> {
 public:
  void updateVkStructure() override {
    vecLogiToVk(attachments, vkAttachments_);
    vecToCArr(vkAttachments_, pAttachments, attachmentCount);

    vecLogiToVk(subpasses, vkSubpasses_);
    vecToCArr(vkSubpasses_, pSubpasses, subpassCount);

    vecLogiToVk(dependencies, vkDependencies_);
    vecToCArr(vkDependencies_, pDependencies, dependencyCount);

    vecToCArr(correlatedViewMasks, pCorrelatedViewMasks, correlatedViewMaskCount);
  }

  using VkType::flags;
  std::vector<AttachmentDescription2KHRChain> attachments;
  std::vector<SubpassDescription2KHRChain> subpasses;
  std::vector<SubpassDependency2KHRChain> dependencies;
  std::vector<uint32_t> correlatedViewMasks;

 private:
  std::vector<vk::AttachmentDescription2KHR> vkAttachments_;
  std::vector<vk::SubpassDescription2KHR> vkSubpasses_;
  std::vector<vk::SubpassDependency2KHR> vkDependencies_;
};

using RenderPassCreateInfo2KHRChain = StructureChain<RenderPassCreateInfo2KHR>;

class DeviceGroupRenderPassBeginInfo : public Structure<vk::DeviceGroupRenderPassBeginInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(deviceRenderAreas, pDeviceRenderAreas, deviceRenderAreaCount);
  }

  using VkType::deviceMask;
  std::vector<vk::Rect2D> deviceRenderAreas;
};

class SubpassSampleLocationsEXT : public Structure<vk::SubpassSampleLocationsEXT> {
 public:
  void updateVkStructure() override {
    VkType::sampleLocationsInfo = sampleLocationsInfo.build();
  }

  using VkType::subpassIndex;
  SampleLocationsInfoEXT sampleLocationsInfo;
};

class AttachmentSampleLocationsEXT : public Structure<vk::AttachmentSampleLocationsEXT> {
 public:
  void updateVkStructure() override {
    VkType::sampleLocationsInfo = sampleLocationsInfo.build();
  }

  using VkType::attachmentIndex;
  SampleLocationsInfoEXT sampleLocationsInfo;
};

class RenderPassSampleLocationsBeginInfoEXT : public Structure<vk::RenderPassSampleLocationsBeginInfoEXT> {
 public:
  void updateVkStructure() override {
    vecLogiToVk(attachmentInitialSampleLocations, vkAttachmentInitialSampleLocations_);
    vecToCArr(vkAttachmentInitialSampleLocations_, pAttachmentInitialSampleLocations,
              attachmentInitialSampleLocationsCount);
    vecLogiToVk(postSubpassSampleLocations, vkPostSubpassSampleLocations_);
    vecToCArr(vkPostSubpassSampleLocations_, pPostSubpassSampleLocations, postSubpassSampleLocationsCount);
  }

  std::vector<AttachmentSampleLocationsEXT> attachmentInitialSampleLocations;
  std::vector<SubpassSampleLocationsEXT> postSubpassSampleLocations;

 private:
  std::vector<vk::AttachmentSampleLocationsEXT> vkAttachmentInitialSampleLocations_;
  std::vector<vk::SubpassSampleLocationsEXT> vkPostSubpassSampleLocations_;
};

class RenderPassBeginInfo : public Structure<vk::RenderPassBeginInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(clearValues, pClearValues, clearValueCount);
  }

  using VkType::framebuffer;
  using VkType::renderArea;
  using VkType::renderPass;

  std::vector<vk::ClearValue> clearValues;
};

using RenderPassBeginInfoChain =
  StructureChain<RenderPassBeginInfo, DeviceGroupRenderPassBeginInfo, RenderPassSampleLocationsBeginInfoEXT>;

class DeviceGroupSwapchainCreateInfoKHR : public Structure<vk::DeviceGroupSwapchainCreateInfoKHR> {
 public:
  using VkType::modes;
};

class SwapchainCounterCreateInfoEXT : public Structure<vk::SwapchainCounterCreateInfoEXT> {
 public:
  using VkType::surfaceCounters;
};

class SwapchainCreateInfoKHR : public Structure<vk::SwapchainCreateInfoKHR> {
 public:
  void updateVkStructure() override {
    vecToCArr(queueFamilyIndices, pQueueFamilyIndices, queueFamilyIndexCount);
  }

  using VkType::clipped;
  using VkType::compositeAlpha;
  using VkType::flags;
  using VkType::imageArrayLayers;
  using VkType::imageColorSpace;
  using VkType::imageExtent;
  using VkType::imageFormat;
  using VkType::imageSharingMode;
  using VkType::imageUsage;
  using VkType::minImageCount;
  using VkType::oldSwapchain;
  using VkType::presentMode;
  using VkType::preTransform;
  using VkType::surface;
  std::vector<uint32_t> queueFamilyIndices;
};

using SwapchainCreateInfoKHRChain = StructureChain<SwapchainCreateInfoKHR, DeviceGroupSwapchainCreateInfoKHR,
                                                   ImageFormatListCreateInfoKHR, SwapchainCounterCreateInfoEXT>;

class ProtectedSubmitInfo : public Structure<vk::ProtectedSubmitInfo> {
 public:
  using VkType::protectedSubmit;
};

class DeviceGroupSubmitInfo : public Structure<vk::DeviceGroupSubmitInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(waitSemaphoreDeviceIndices, pWaitSemaphoreDeviceIndices, waitSemaphoreCount);
    vecToCArr(commandBufferDeviceMasks, pCommandBufferDeviceMasks, commandBufferCount);
    vecToCArr(signalSemaphoreDeviceIndices, pSignalSemaphoreDeviceIndices, signalSemaphoreCount);
  }

  std::vector<uint32_t> waitSemaphoreDeviceIndices;
  std::vector<uint32_t> commandBufferDeviceMasks;
  std::vector<uint32_t> signalSemaphoreDeviceIndices;
};

class SubmitInfo : public Structure<vk::SubmitInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(waitSemaphores, pWaitSemaphores, waitSemaphoreCount);
    pWaitDstStageMask = waitDstStageMask.empty() ? nullptr : waitDstStageMask.data();
    vecToCArr(commandBuffers, pCommandBuffers, commandBufferCount);
    vecToCArr(signalSemaphores, pSignalSemaphores, signalSemaphoreCount);
  }

  std::vector<vk::Semaphore> waitSemaphores;
  std::vector<vk::PipelineStageFlags> waitDstStageMask;
  std::vector<vk::CommandBuffer> commandBuffers;
  std::vector<vk::Semaphore> signalSemaphores;
};

using SubmitInfoChain = StructureChain<SubmitInfo, ProtectedSubmitInfo, DeviceGroupSubmitInfo>;

class SharedPresentSurfaceCapabilitiesKHR : public Structure<vk::SharedPresentSurfaceCapabilitiesKHR> {
 public:
  using VkType::sharedPresentSupportedUsageFlags;
};

class SurfaceCapabilities2KHR : public Structure<vk::SurfaceCapabilities2KHR> {
 public:
  using VkType::surfaceCapabilities;
};

using SurfaceCapabilities2KHRChain = StructureChain<SurfaceCapabilities2KHR, SharedPresentSurfaceCapabilitiesKHR>;

class WriteDescriptorSetAccelerationStructureNV : public Structure<vk::WriteDescriptorSetAccelerationStructureNV> {
 public:
  void updateVkStructure() override {
    vecToCArr(accelerationStructures, pAccelerationStructures, accelerationStructureCount);
  }

  std::vector<vk::AccelerationStructureNV> accelerationStructures;
};

class WriteDescriptorSetInlineUniformBlockEXT : public Structure<vk::WriteDescriptorSetInlineUniformBlockEXT> {
 public:
  void updateVkStructure() override {
    pData = data.empty() ? nullptr : data.data();
    dataSize = data.size();
  }

  std::vector<std::byte> data;
};

class WriteDescriptorSet : public Structure<vk::WriteDescriptorSet> {
 public:
  void updateVkStructure() override {
    switch (descriptorType) {
      case vk::DescriptorType::eSampler:
      case vk::DescriptorType::eCombinedImageSampler:
      case vk::DescriptorType::eSampledImage:
      case vk::DescriptorType::eStorageImage:
      case vk::DescriptorType::eInputAttachment:
        pImageInfo = imageInfo.empty() ? nullptr : imageInfo.data();
        break;
      case vk::DescriptorType::eUniformBuffer:
      case vk::DescriptorType::eStorageBuffer:
      case vk::DescriptorType::eUniformBufferDynamic:
      case vk::DescriptorType::eStorageBufferDynamic:
        pBufferInfo = bufferInfo.empty() ? nullptr : bufferInfo.data();
        break;
      case vk::DescriptorType::eUniformTexelBuffer:
      case vk::DescriptorType::eStorageTexelBuffer:
        pTexelBufferView = texelBufferView.empty() ? nullptr : texelBufferView.data();
        break;
      case vk::DescriptorType::eAccelerationStructureNV:
      case vk::DescriptorType::eInlineUniformBlockEXT:
        // Nothing to do.
        break;
    }
  }

  using VkType::descriptorCount;
  using VkType::descriptorType;
  using VkType::dstArrayElement;
  using VkType::dstBinding;
  using VkType::dstSet;
  std::vector<vk::DescriptorImageInfo> imageInfo;
  std::vector<vk::DescriptorBufferInfo> bufferInfo;
  std::vector<vk::BufferView> texelBufferView;
};

using WriteDescriptorSetChain = StructureChain<WriteDescriptorSet, WriteDescriptorSetInlineUniformBlockEXT,
                                               WriteDescriptorSetAccelerationStructureNV>;

class SurfaceCapabilities2EXT : public Structure<vk::SurfaceCapabilities2EXT> {
 public:
  using VkType::currentExtent;
  using VkType::currentTransform;
  using VkType::maxImageArrayLayers;
  using VkType::maxImageCount;
  using VkType::maxImageExtent;
  using VkType::minImageCount;
  using VkType::minImageExtent;
  using VkType::supportedCompositeAlpha;
  using VkType::supportedSurfaceCounters;
  using VkType::supportedTransforms;
  using VkType::supportedUsageFlags;
};

using SurfaceCapabilities2EXTChain = StructureChain<SurfaceCapabilities2EXT>;

class SurfaceFormat2KHR : public Structure<vk::SurfaceFormat2KHR> {
 public:
  using VkType::surfaceFormat;
};

using SurfaceFormat2KHRChain = StructureChain<SurfaceFormat2KHR>;

class ValidationCacheCreateInfoEXT : public Structure<vk::ValidationCacheCreateInfoEXT> {
 public:
  using VkType::flags;
  using VkType::initialDataSize;
  using VkType::pInitialData;
};

using ValidationCacheCreateInfoEXTChain = StructureChain<ValidationCacheCreateInfoEXT>;

} // namespace logi

#endif // LOGI_STRUCTURES_VK_STRUCTURE_HPP
