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

#ifndef LOGI_LOGI_HPP
#define LOGI_LOGI_HPP

#include "logi/base/exception.hpp"
#include "logi/base/handle.hpp"
#include "logi/base/vulkan_object.hpp"
#include "logi/command/command_buffer.hpp"
#include "logi/command/command_pool.hpp"
#include "logi/descriptor/descriptor_pool.hpp"
#include "logi/descriptor/descriptor_set.hpp"
#include "logi/descriptor/descriptor_update_template.hpp"
#include "logi/device/display_khr.hpp"
#include "logi/device/logical_device.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/debug_report_callback.hpp"
#include "logi/instance/debug_utils_messenger_ext.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/logi.hpp"
#include "logi/memory/acceleration_structure_nv.hpp"
#include "logi/memory/acceleration_structure_khr.hpp"
#include "logi/memory/buffer.hpp"
#include "logi/memory/buffer_view.hpp"
#include "logi/memory/device_memory.hpp"
#include "logi/memory/image.hpp"
#include "logi/memory/image_view.hpp"
#include "logi/memory/memory_allocator.hpp"
#include "logi/memory/sampler.hpp"
#include "logi/memory/vma_buffer.hpp"
#include "logi/memory/vma_image.hpp"
#include "logi/memory/vma_acceleration_structure_nv.hpp"
#include "logi/nvidia/indirect_commands_layout_nv.hpp"
// #include "logi/nvidia/object_table_nvx.hpp"
#include "logi/program/descriptor_set_layout.hpp"
#include "logi/program/pipeline_cache.hpp"
#include "logi/program/pipeline_layout.hpp"
#include "logi/program/shader_module.hpp"
#include "logi/query/query_pool.hpp"
#include "logi/queue/queue.hpp"
#include "logi/queue/queue_family.hpp"
#include "logi/render_pass/framebuffer.hpp"
#include "logi/render_pass/render_pass.hpp"
#include "logi/surface/surface_khr.hpp"
#include "logi/swapchain/swapchain_image.hpp"
#include "logi/swapchain/swapchain_khr.hpp"
#include "logi/synchronization/event.hpp"
#include "logi/synchronization/fence.hpp"
#include "logi/synchronization/semaphore.hpp"

#endif // LOGI_LOGI_HPP
