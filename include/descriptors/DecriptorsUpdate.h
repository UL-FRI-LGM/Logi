/*
* Descriptor pool
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef DESCRIPTORS_DESCRIPTORS_UPDATE_H
#define DESCRIPTORS_DESCRIPTORS_UPDATE_H

#include <vulkan/vulkan.hpp>
#include "descriptors/DescriptorPool.h"
#include "memory/Buffer.h"

namespace vkr {

class DescriptorsUpdate {
public:
	DescriptorsUpdate();

	void copyDescriptorSet(const DescriptorSet* src_set, uint32_t src_binding, uint32_t src_array_element, const DescriptorSet* dst_set, uint32_t dst_binding, uint32_t dst_array_element, uint32_t descriptor_count);

	void writeBufferToDescriptorSet(const DescriptorSet* dst_set, uint32_t dst_binding, uint32_t dst_array_element, Buffer* buffer, vk::DeviceSize offset, vk::DeviceSize range);

	const std::vector<vk::CopyDescriptorSet>& getCopyOperations() const;

	const std::vector<vk::WriteDescriptorSet>& getWriteOperations() const;

private:
	std::vector<vk::WriteDescriptorSet> write_operations_;
	std::vector<vk::CopyDescriptorSet> copy_operations_;

	std::vector<std::unique_ptr<vk::DescriptorBufferInfo>> buffer_infos_;
	std::vector<std::unique_ptr<vk::DescriptorImageInfo>> image_infos_;
};

}


#endif