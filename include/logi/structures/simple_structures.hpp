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

#ifndef LOGI_STRUCTURES_SIMPLE_STRUCTURES_HPP
#define LOGI_STRUCTURES_SIMPLE_STRUCTURES_HPP

#include "logi/structures/structure.hpp"

namespace logi {

using AllocationCallbacks = vk::AllocationCallbacks;
using AttachmentDescription = vk::AttachmentDescription;
using AttachmentReference = vk::AttachmentReference;
using BufferCopy = vk::BufferCopy;
using BufferImageCopy = vk::BufferImageCopy;
using ClearAttachment = vk::ClearAttachment;
using ClearDepthStencilValue = vk::ClearDepthStencilValue;
using ClearRect = vk::ClearRect;
using CoarseSampleLocationNV = vk::CoarseSampleLocationNV;
using ComponentMapping = vk::ComponentMapping;
using ConformanceVersionKHR = vk::ConformanceVersionKHR;
using DescriptorBufferInfo = vk::DescriptorBufferInfo;
using DescriptorImageInfo = vk::DescriptorImageInfo;
using DescriptorPoolSize = vk::DescriptorPoolSize;
using DescriptorUpdateTemplateEntry = vk::DescriptorUpdateTemplateEntry;
using DispatchIndirectCommand = vk::DispatchIndirectCommand;
using DisplayModeParametersKHR = vk::DisplayModeParametersKHR;
using DisplayModePropertiesKHR = vk::DisplayModePropertiesKHR;
using DisplayPlaneCapabilitiesKHR = vk::DisplayPlaneCapabilitiesKHR;
using DisplayPlanePropertiesKHR = vk::DisplayPlanePropertiesKHR;
using DisplayPropertiesKHR = vk::DisplayPropertiesKHR;
using DrawIndexedIndirectCommand = vk::DrawIndexedIndirectCommand;
using DrawIndirectCommand = vk::DrawIndirectCommand;
using DrawMeshTasksIndirectCommandNV = vk::DrawMeshTasksIndirectCommandNV;
using DrmFormatModifierPropertiesEXT = vk::DrmFormatModifierPropertiesEXT;
using ExtensionProperties = vk::ExtensionProperties;
using Extent2D = vk::Extent2D;
using Extent3D = vk::Extent3D;
using ExternalImageFormatPropertiesNV = vk::ExternalImageFormatPropertiesNV;
using ExternalMemoryProperties = vk::ExternalMemoryProperties;
using FormatProperties = vk::FormatProperties;
using ImageBlit = vk::ImageBlit;
using ImageCopy = vk::ImageCopy;
using ImageFormatProperties = vk::ImageFormatProperties;
using ImageResolve = vk::ImageResolve;
using ImageSubresource = vk::ImageSubresource;
using ImageSubresourceLayers = vk::ImageSubresourceLayers;
using ImageSubresourceRange = vk::ImageSubresourceRange;
using IndirectCommandsLayoutTokenNVX = vk::IndirectCommandsLayoutTokenNVX;
using IndirectCommandsTokenNVX = vk::IndirectCommandsTokenNVX;
using InputAttachmentAspectReference = vk::InputAttachmentAspectReference;
using LayerProperties = vk::LayerProperties;
using MemoryHeap = vk::MemoryHeap;
using MemoryRequirements = vk::MemoryRequirements;
using MemoryType = vk::MemoryType;
using ObjectTableDescriptorSetEntryNVX = vk::ObjectTableDescriptorSetEntryNVX;
using ObjectTableEntryNVX = vk::ObjectTableEntryNVX;
using ObjectTableIndexBufferEntryNVX = vk::ObjectTableIndexBufferEntryNVX;
using ObjectTablePipelineEntryNVX = vk::ObjectTablePipelineEntryNVX;
using ObjectTablePushConstantEntryNVX = vk::ObjectTablePushConstantEntryNVX;
using ObjectTableVertexBufferEntryNVX = vk::ObjectTableVertexBufferEntryNVX;
using Offset2D = vk::Offset2D;
using Offset3D = vk::Offset3D;
using PastPresentationTimingGOOGLE = vk::PastPresentationTimingGOOGLE;
using PhysicalDeviceFeatures = vk::PhysicalDeviceFeatures;
using PhysicalDeviceLimits = vk::PhysicalDeviceLimits;
using PhysicalDeviceMemoryProperties = vk::PhysicalDeviceMemoryProperties;
using PhysicalDeviceProperties = vk::PhysicalDeviceProperties;
using PhysicalDeviceSparseProperties = vk::PhysicalDeviceSparseProperties;
using PipelineColorBlendAttachmentState = vk::PipelineColorBlendAttachmentState;
using PresentTimeGOOGLE = vk::PresentTimeGOOGLE;
using PushConstantRange = vk::PushConstantRange;
using QueueFamilyProperties = vk::QueueFamilyProperties;
using Rect2D = vk::Rect2D;
using RectLayerKHR = vk::RectLayerKHR;
using RefreshCycleDurationGOOGLE = vk::RefreshCycleDurationGOOGLE;
using SampleLocationEXT = vk::SampleLocationEXT;
using ShaderResourceUsageAMD = vk::ShaderResourceUsageAMD;
using ShaderStatisticsInfoAMD = vk::ShaderStatisticsInfoAMD;
using SparseImageFormatProperties = vk::SparseImageFormatProperties;
using SparseImageMemoryBind = vk::SparseImageMemoryBind;
using SparseImageMemoryRequirements = vk::SparseImageMemoryRequirements;
using SparseMemoryBind = vk::SparseMemoryBind;
using SpecializationMapEntry = vk::SpecializationMapEntry;
using StencilOpState = vk::StencilOpState;
using SubpassDependency = vk::SubpassDependency;
using SubresourceLayout = vk::SubresourceLayout;
using SurfaceCapabilitiesKHR = vk::SurfaceCapabilitiesKHR;
using SurfaceFormatKHR = vk::SurfaceFormatKHR;
using VertexInputAttributeDescription = vk::VertexInputAttributeDescription;
using VertexInputBindingDescription = vk::VertexInputBindingDescription;
using VertexInputBindingDivisorDescriptionEXT = vk::VertexInputBindingDivisorDescriptionEXT;
using Viewport = vk::Viewport;
using ViewportSwizzleNV = vk::ViewportSwizzleNV;
using ViewportWScalingNV = vk::ViewportWScalingNV;
using XYColorEXT = vk::XYColorEXT;

class CoarseSampleOrderCustomNV : public Structure<vk::CoarseSampleOrderCustomNV> {
 public:
  void updateVkStructure() override {
    vecToCArr(sampleLocations, pSampleLocations, sampleLocationCount);
  }

  using VkType::sampleCount;
  using VkType::shadingRate;
  std::vector<CoarseSampleLocationNV> sampleLocations;
};

class DescriptorSetLayoutBinding : public Structure<vk::DescriptorSetLayoutBinding> {
 public:
  void updateVkStructure() override {
    pImmutableSamplers = immutableSamplers.empty() ? nullptr : immutableSamplers.data();
  }

  using VkType::binding;
  using VkType::descriptorCount;
  using VkType::descriptorType;
  using VkType::stageFlags;
  std::vector<vk::Sampler> immutableSamplers;
};

class PresentRegionKHR : public Structure<vk::PresentRegionKHR> {
 public:
  void updateVkStructure() override {
    vecToCArr(rectangles, pRectangles, rectangleCount);
  }

  std::vector<vk::RectLayerKHR> rectangles;
};

class ShadingRatePaletteNV : public Structure<vk::ShadingRatePaletteNV> {
 public:
  void updateVkStructure() override {
    vecToCArr(shadingRatePaletteEntries, pShadingRatePaletteEntries, shadingRatePaletteEntryCount);
  }

  std::vector<vk::ShadingRatePaletteEntryNV> shadingRatePaletteEntries;
};

class SparseBufferMemoryBindInfo : public Structure<vk::SparseBufferMemoryBindInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(binds, pBinds, bindCount);
  }

  using VkType::buffer;
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

class SparseImageOpaqueMemoryBindInfo : public Structure<vk::SparseImageOpaqueMemoryBindInfo> {
 public:
  void updateVkStructure() override {
    vecToCArr(binds, pBinds, bindCount);
  }

  using VkType::image;
  std::vector<vk::SparseMemoryBind> binds;
};

class SpecializationInfo : public Structure<vk::SpecializationInfo> {
  void updateVkStructure() override {
    vecToCArr(mapEntries, pMapEntries, mapEntryCount);
    dataSize = data.size();
    pData = data.empty() ? nullptr : data.data();
  }

  std::vector<vk::SpecializationMapEntry> mapEntries;
  std::vector<std::byte> data;
};

class SubpassDescription : public Structure<vk::SubpassDescription> {
 public:
  void updateVkStructure() override {
    vecToCArr(inputAttachments, pInputAttachments, inputAttachmentCount);
    vecToCArr(colorAttachments, pColorAttachments, colorAttachmentCount);
    pResolveAttachments = resolveAttachments.empty() ? nullptr : resolveAttachments.data();
    pDepthStencilAttachment = depthStencilAttachment.has_value() ? &depthStencilAttachment.value() : nullptr;
    vecToCArr(preserveAttachments, pPreserveAttachments, preserveAttachmentCount);
  }

  using VkType::flags;
  using VkType::pipelineBindPoint;
  std::vector<vk::AttachmentReference> inputAttachments;
  std::vector<vk::AttachmentReference> colorAttachments;
  std::vector<vk::AttachmentReference> resolveAttachments;
  std::optional<vk::AttachmentReference> depthStencilAttachment;
  std::vector<uint32_t> preserveAttachments;
};

class SampleLocationsInfoEXT : public Structure<vk::SampleLocationsInfoEXT> {
 public:
  void updateVkStructure() override {
    vecToCArr(sampleLocations, pSampleLocations, sampleLocationsCount);
  }

  using VkType::sampleLocationGridSize;
  using VkType::sampleLocationsPerPixel;
  std::vector<vk::SampleLocationEXT> sampleLocations;
};

} // namespace logi

#endif // LOGI_STRUCTURES_SIMPLE_STRUCTURES_HPP
