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

#ifndef LOGI_PROGRAM_PIPELINE_HPP
#define LOGI_PROGRAM_PIPELINE_HPP

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"
#include "logi/program/pipeline_impl.hpp"

namespace logi {

class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;
class PipelineImpl;

class Pipeline : public Handle<PipelineImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetRayTracingShaderGroupStackSizeKHR.html">vkGetRayTracingShaderGroupStackSizeKHR</a>
   */
  vk::DeviceSize getRayTracingShaderGroupStackSizeKHR(uint32_t group, vk::ShaderGroupShaderKHR groupShader) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetRayTracingCaptureReplayShaderGroupHandlesKHR.html">vkGetRayTracingCaptureReplayShaderGroupHandlesKHR</a>
   */
  template <typename T>
  vk::ResultValueType<void>::type getRayTracingCaptureReplayShaderGroupHandlesKHR(uint32_t firstGroup, uint32_t groupCount, 
                                                                                  const vk::ArrayProxy<T>& data) const {
    return object_->getRayTracingCaptureReplayShaderGroupHandlesKHR<T>(firstGroup, groupCount, data);
  }

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetRayTracingShaderGroupHandlesKHR.html">vkGetRayTracingShaderGroupHandlesKHR</a>
   */
  template <typename T>
  vk::ResultValueType<void>::type getRayTracingShaderGroupHandlesKHR(uint32_t firstGroup, uint32_t groupCount,
                                                                    vk::ArrayProxy<T> data) const {
    return object_->getRayTracingShaderGroupHandlesKHR<T>(firstGroup, groupCount, data);
  }

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCompileDeferredNV.html">vkCompileDeferredNV</a>
   */
  vk::ResultValueType<void>::type compileDeferredNV(uint32_t shader) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetShaderInfoAMD.html">vkGetShaderInfoAMD</a>
   */
  typename vk::ResultValueType<std::vector<uint8_t>>::type getShaderInfoAMD(vk::ShaderStageFlagBits shaderStage,
                                                                            vk::ShaderInfoTypeAMD infoType) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetRayTracingShaderGroupHandlesNV.html">vkGetRayTracingShaderGroupHandlesNV</a>
   */
  template <typename T>
  vk::ResultValueType<void>::type getRayTracingShaderGroupHandlesNV(uint32_t firstGroup, uint32_t groupCount,
                                                                    vk::ArrayProxy<T> data) const {
    return object_->getRayTracingShaderGroupHandlesNV<T>(firstGroup, groupCount, data);
  }

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::Pipeline&() const;

  // endregion
};

} // namespace logi

#endif // LOGI_PROGRAM_PIPELINE_HPP
