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
