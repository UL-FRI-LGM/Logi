/*
* Descriptor pool
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef DESCRIPTORS_DESCRIPTORS_UPDATE_H
#define DESCRIPTORS_DESCRIPTORS_UPDATE_H

#include <utility>
#include <vulkan/vulkan.hpp>
#include "logi/descriptors/DescriptorPool.h"
#include "logi/memory/Buffer.h"
#include "logi/memory/ImageView.h"
#include "logi/memory/Image.h"
#include "logi/memory/Sampler.h"

namespace logi {

/**
 * @brief   Contains buffer related information used in buffer descriptor write.
 */
struct DescriptorBufferInfo {

    /**
     * @brief   Initialize DescriptorBufferInfo struct members.	
     *
     * @param	buffer  Buffer handle.
     * @param	offset  Offset from the start of the buffer.
     * @param	range   Range in bytes.
     */
    DescriptorBufferInfo(Buffer buffer, const uint64_t offset, const uint64_t range)
        : buffer(std::move(buffer)),
          offset(offset),
          range(range) {}

    /**
     * @brief   Build Vulkan Descriptor buffer info using the internal data.
     * 
     * @return  Vulkan DescriptorBufferInfo structure.
     */
	vk::DescriptorBufferInfo build() const {
		return { buffer.getVkHandle(), offset, range };
    }

    /**
     * Buffer handle.
     */
	Buffer buffer;

    /**
     * Offset from the start of the buffer.
     */
	uint64_t offset;
    
    /**
     * Range in bytes.
     */
    uint64_t range;
};


/**
 * @brief	Contains image related information used in image descriptor write.
 */
struct DescriptorImageInfo {

    /**
     * @brief	Initialize DescriptorImageInfo struct members.	
     *
     * @param	sampler Sampler handle.
     * @param	view    ImageView handle.
     * @param	layout  Layout that the image subresources accessible from view will be in at the time this descriptor 
     *                  is accessed. 
     */
    DescriptorImageInfo(Sampler sampler, ImageView view, const vk::ImageLayout layout)
        : sampler(std::move(sampler)),
          view(std::move(view)),
          layout(layout) {}

	/**
     * @brief   Build Vulkan Descriptor image info using the internal data.
     * 
     * @return  Vulkan DescriptorImageInfo structure.
     */
	vk::DescriptorImageInfo build() const {
		return { sampler.getVkHandle(), view.getVkHandle(), layout };
	}

    /**
     * Sampler handle.
     */
	Sampler sampler;

    /**
     * ImageView handle.
     */
	ImageView view;

    /**
     * Layout that the image subresources accessible from view will be in at the time this descriptor is accessed. 
     */
	vk::ImageLayout layout;
};

/**
 * @brief   Contains base descriptor write information.
 */
struct DescriptorWrite : public BuildableExtendable {

    /**
	 * @brief	Initialize base descriptor write data.
	 *
	 * @param	set                 Descriptor set handle. 
	 * @param	dst_binding         Descriptor binding within the set.
	 * @param	dst_array_element   Starting element in that array.
	 */
	DescriptorWrite(DescriptorSet set, const uint32_t dst_binding, const uint32_t dst_array_element)
		: set(std::move(set)), dst_binding(dst_binding), dst_array_element(dst_array_element) {}

	/**
     * @brief   Build Vulkan WriteDescriptorSet using the internal data.
     * 
     * @return  Vulkan WriteDescriptorSet structure.
     */
	virtual vk::WriteDescriptorSet build() = 0;

    /**
     * Descriptor set handle.
     */
	DescriptorSet set;

    /**
     * Descriptor binding within the set.
     */
	uint32_t dst_binding;

    /**
     * Starting element in that array.
     */
	uint32_t dst_array_element;
};

/**
 * @brief   Contains descriptor write information for Image write.
 */
struct DescriptorImageWrite : public DescriptorWrite {

	/**
     * @brief	Initialize base and image specific descriptor write data.
     *
     * @param	set                 Descriptor set handle.
     * @param	dst_binding         Descriptor binding within the set.
     * @param	dst_array_element   Starting element in that array.
     * @param   image_infos         DescriptorImageInfo structures.
     */
	DescriptorImageWrite(const DescriptorSet& set, const uint32_t dst_binding, const uint32_t dst_array_element,
		std::vector<DescriptorImageInfo> image_infos)
        : DescriptorWrite(set, dst_binding, dst_array_element), image_infos(std::move(image_infos)) { }

	/**
	 * @brief   Build Vulkan WriteDescriptorSet using the internal data.
	 *
	 * @return  Vulkan WriteDescriptorSet structure.
	 */
    vk::WriteDescriptorSet build() override;

	/**
     * DescriptorImageInfo structures.
     */
	std::vector<DescriptorImageInfo> image_infos;
private:
    /**
     * Built Vulkan DescriptorImageInfos.
     */
	std::vector<vk::DescriptorImageInfo> vk_image_infos_;
};

/**
 * @brief   Contains descriptor write information for buffer write.
 */
struct DescriptorBufferWrite : public DescriptorWrite {

	/**
     * @brief	Initialize base and image specific descriptor write data.
     *
     * @param	set                 Descriptor set handle.
     * @param	dst_binding         Descriptor binding within the set.
     * @param	dst_array_element   Starting element in that array.
     * @param   buffer_infos        DescriptorBufferInfo structures.
     */
	DescriptorBufferWrite(const DescriptorSet& set, const uint32_t dst_binding, const uint32_t dst_array_element,
		std::vector<DescriptorBufferInfo> buffer_infos)
		: DescriptorWrite(set, dst_binding, dst_array_element), buffer_infos(std::move(buffer_infos)) { }

	/**
     * @brief   Build Vulkan WriteDescriptorSet using the internal data.
     *
     * @return  Vulkan WriteDescriptorSet structure.
     */
	vk::WriteDescriptorSet build() override;

	/**
     * DescriptorBufferInfo structures.
     */
	std::vector<DescriptorBufferInfo> buffer_infos;
private:
	/**
     * Built Vulkan DescriptorBufferInfos.
     */
	std::vector<vk::DescriptorBufferInfo> vk_buffer_infos_;
};

/**
 * @brief   Contains descriptor write information for buffer view write.
 */
struct DescriptorBufferViewWrite : public DescriptorWrite {

	/**
     * @brief	Initialize base and image specific descriptor write data.
     *
     * @param	set                 Descriptor set handle.
     * @param	dst_binding         Descriptor binding within the set.
     * @param	dst_array_element   Starting element in that array.
     * @param   buffer_views        BufferViews handles.
     */
	DescriptorBufferViewWrite(const DescriptorSet& set, const uint32_t dst_binding, const uint32_t dst_array_element,
		std::vector<BufferView> buffer_views)
		: DescriptorWrite(set, dst_binding, dst_array_element), buffer_views(std::move(buffer_views)) {
	}

	/**
     * @brief   Build Vulkan WriteDescriptorSet using the internal data.
     *
     * @return  Vulkan WriteDescriptorSet structure.
     */
	vk::WriteDescriptorSet build() override;

	/**
     * BufferView handles.
     */
	std::vector<BufferView> buffer_views;
private:
	/**
     * Vulkan BufferView handles.
     */
	std::vector<vk::BufferView> vk_buffer_views_;
};

/**
 * @brief   Contains descriptor copy information.
 */
struct DescriptorSetCopy : public BuildableExtendable {

    /**
     * @brief	Initialize descriptor set copy data.
     *
     * @param	src_set             Copy source DescriptorSet.
     * @param	src_binding         Starting source binding.
     * @param	src_array_element   Starting element in that array.
     * @param	dst_set             Copy destination DescriptorSet.
     * @param	dst_binding         Starting destination binding.
     * @param	dst_array_element   Starting element in that array.
     * @param	descriptor_count    Number of descriptors to be copied.
     */
    DescriptorSetCopy(DescriptorSet src_set, const uint32_t src_binding, const uint32_t src_array_element, DescriptorSet dst_set,
        const uint32_t dst_binding, const uint32_t dst_array_element, const uint32_t descriptor_count)
        : src_set(std::move(src_set)), src_binding(src_binding), src_array_element(src_array_element),
          dst_set(std::move(dst_set)), dst_binding(dst_binding), dst_array_element(dst_array_element),
          descriptor_count(descriptor_count) {}

	/**
     * @brief   Build Vulkan CopyDescriptorSet using the internal data.
     *
     * @return  Vulkan WriteDescriptorSet structure.
     */
	vk::CopyDescriptorSet build() const;

    /**
     * Copy source DescriptorSet.
     */
	DescriptorSet src_set;

	/**
     * Starting source binding.
     */
	uint32_t src_binding;

	/**
     * Starting element in that array.
     */
	uint32_t src_array_element;

	/**
     * Copy destination DescriptorSet.
     */
	DescriptorSet dst_set;

	/**
     * Starting destination binding.
     */
	uint32_t dst_binding;

	/**
     * Starting element in that array.
     */
	uint32_t dst_array_element;

	/**
     * Number of descriptors to be copied.
     */
	uint32_t descriptor_count;
};


/**
 * @brief	Handle used to perform DescriptorSet updates on the device from which it was acquired from.
 */
class DescriptorSetUpdate : public DependentDestroyableHandle {
public:
	/**
     * @brief	Construct placeholder DescriptorSetUpdate handle.
     */
	DescriptorSetUpdate();

	/**
	 * @brief   Create and configure Vulkan descriptor pool.
	 *
	 * @param	owner       Pointer to the handle manager responsible for this descriptor set update.
	 * @param	device      Vulkan device handle.
	 */
	DescriptorSetUpdate(std::weak_ptr<HandleManager>& owner, const vk::Device& device);


    /**
	 * @brief	Adds Image write to the update.
	 *
	 * @param	image_write Contains descriptor image write information.
	 */
	void write(const DescriptorImageWrite& image_write) const;

	/**
     * @brief	Adds Buffer write to the update.
     *
     * @param	buffer_write Contains descriptor buffer write information.
     */
	void write(const DescriptorBufferWrite& buffer_write) const;

	/**
     * @brief	Adds BufferView write to the update.
     *
     * @param	buffer_view_write Contains descriptor buffer view write information.
     */
	void write(const DescriptorBufferViewWrite& buffer_view_write) const;

    /**
	 * @brief	Adds set copy to the update.
	 *
	 * @param	set_copy    Descriptor set copy information.
	 */
	void copy(const DescriptorSetCopy& set_copy) const;

    /**
	 * @brief	Execute the update (single update may be executed multiple times).
	 */
	void execute() const;

private:

    /**
     * @brief	Encapsulates shared internal DescriptorSetUpdate data.
     */
    struct DescriptorSetUpdateData {
        /**
         * @brief	Initialize DescriptorSetUpdateData.
         *
         * @param	device  Vulkan device handle.
         */
        explicit DescriptorSetUpdateData(vk::Device device);

        /**
         * Vulkan device handle.
         */
		vk::Device device;

        /**
         * List of DescriptorImageWrite structures (must be held to keep the pointers in vk::WriteDescriptorSet valid).
         */
		std::list<DescriptorImageWrite> image_writes;
		
        /**
         * List of DescriptorBufferWrite structures (must be held to keep the pointers in vk::WriteDescriptorSet valid).
         */
		std::list<DescriptorBufferWrite> buffer_writes;

        /**
         * List of DescriptorBufferViewWrite structures (must be held to keep the pointers in vk::WriteDescriptorSet
         * valid).
         */
		std::list<DescriptorBufferViewWrite> buffer_view_writes;

        /**
         * Vulkan WriteDescriptorSet structures.
         */
		std::vector<vk::WriteDescriptorSet> writes;

		/**
         * Vulkan CopyDescriptorSet structures.
         */
		std::vector<vk::CopyDescriptorSet> copies;
    };

    /**
     * Handle shared data.
     */
	std::shared_ptr<DescriptorSetUpdateData> data_;
};

}

#endif
