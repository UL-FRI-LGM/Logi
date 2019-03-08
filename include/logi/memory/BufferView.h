#ifndef MEMORY_BUFFER_VIEW_H
#define MEMORY_BUFFER_VIEW_H

#include <vulkan/vulkan.hpp>
#include "logi/base/ManagedResource.h"
#include "logi/base/Handle.h"

namespace logi {

/**
 * @brief	Contains configuration for BufferView.
 */
struct BufferViewConfiguration {
	/**
	 * @brief	BufferViewConfiguration default constructor.
	 *
	 * @param	format	Buffer view element format.
	 * @param	offset	Buffer view offset from the start of the buffer in Bytes.
	 * @param	range	Buffer view size (number of captured bytes from offset).
	 */
	BufferViewConfiguration(vk::Format format, vk::DeviceSize offset, vk::DeviceSize range);

	vk::Format format;		///< Buffer view element format.
	vk::DeviceSize offset;	///< Buffer view offset from the start of the buffer in Bytes.
	vk::DeviceSize range;	///< Buffer view size (number of captured bytes from offset).
};

/**
 * @brief	Handle used to access Vulkan BufferView resource.
 */
class BufferView : public DependentDestroyableHandle {
public:
	/**
	 * @brief	Create handle used to access BufferView resource.
	 *
	 * @param	owner			HandleManager responsible for this handle.
	 * @param	vk_device		Vulkan device handle.
	 * @param	vk_buffer		Vulkan image handle.
	 * @param	configuration	BufferView configuration.
	 */
	BufferView(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device, const vk::Buffer& vk_buffer, const BufferViewConfiguration& configuration);

	/**
	 * @brief	Retrieve BufferView configuration.	
	 *
	 * @return	BufferViewConfiguration object.
	 */
	const BufferViewConfiguration& configuration() const;
	
	/**
	 * @brief	Retrieve vk::BufferView handle.
	 *
	 * @return	vk::BufferView handle.
	 */
	const vk::BufferView& getVkHandle() const;

protected:
	/**
	 * @brief	Free resources.
	 */
	void free() override;

private:
	using ManagedVkBufferView = ManagedResource<vk::Device, vk::BufferView, vk::DispatchLoaderStatic, &vk::Device::destroyBufferView>;
	std::shared_ptr<BufferViewConfiguration> configuration_;	///< BufferView configuration.
	std::shared_ptr<ManagedVkBufferView> vk_buffer_view_;		///< BufferView Vulkan handle.
};

}

#endif