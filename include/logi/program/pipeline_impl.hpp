/**
 * Project Logi source code
 * Copyright (C) 2019 Lana Besevic
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

#ifndef LOGI_PROGRAM_PIPELINE_IMPL_HPP
#define LOGI_PROGRAM_PIPELINE_IMPL_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;

class PipelineImpl : public VulkanObject<PipelineImpl> {
 public:
  PipelineImpl(LogicalDeviceImpl& logicalDevice, const vk::Pipeline& vkPipeline,
               const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Vulkan Declarations

  vk::ResultValueType<void>::type compileDeferredNV(uint32_t shader) const;

  typename vk::ResultValueType<std::vector<uint8_t>>::type getShaderInfoAMD(vk::ShaderStageFlagBits shaderStage,
                                                                            vk::ShaderInfoTypeAMD infoType) const;

  template <typename T>
  vk::ResultValueType<void>::type getRayTracingShaderGroupHandlesNV(uint32_t firstGroup, uint32_t groupCount,
                                                                    vk::ArrayProxy<T> data) const;

  // endregion

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::Pipeline&() const;

 protected:
  void free() override;

  // endregion

 private:
  LogicalDeviceImpl& logicalDevice_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::Pipeline vkPipeline_;
};

template <typename T>
vk::ResultValueType<void>::type PipelineImpl::getRayTracingShaderGroupHandlesNV(uint32_t firstGroup,
                                                                                uint32_t groupCount,
                                                                                vk::ArrayProxy<T> data) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getRayTracingShaderGroupHandlesNV(vkPipeline_, firstGroup, groupCount, data, getDispatcher());
}

} // namespace logi

#endif // LOGI_PROGRAM_PIPELINE_IMPL_HPP
