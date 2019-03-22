#ifndef MEMORY_BUFFER_VIEW_H
#define MEMORY_BUFFER_VIEW_H

#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/base/ManagedResource.h"

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

  vk::Format format;     ///< Buffer view element format.
  vk::DeviceSize offset; ///< Buffer view offset from the start of the buffer in Bytes.
  vk::DeviceSize range;  ///< Buffer view size (number of captured bytes from offset).
};

class Buffer;

/**
 * @brief	Handle used to access Vulkan BufferView resource.
 */
class BufferView : public DestroyableOwnedHandle<BufferView, Buffer> {
 public:
  /**
   * @brief Default placeholder constructor.
   */
  BufferView() = default;

  /**
   * @brief	Create handle used to access BufferView resource.
   *
   * @param	buffer        Buffer handle.
   * @param	configuration	BufferView configuration.
   */
  BufferView(const Buffer& buffer, const BufferViewConfiguration& config);

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

  /**
   * @brief   Conversion to vk::BufferView.
   *
   * @return  vk::BufferView.
   */
  operator vk::BufferView() const;

 protected:
  /**
   * @brief	Free resources.
   */
  void free() override;

 private:
  using ManagedVkBufferView =
    ManagedResource<vk::Device, vk::BufferView, vk::DispatchLoaderStatic, &vk::Device::destroyBufferView>;

  /**
   * @brief Buffer view internal data.
   */
  struct Data {
    /**
     * @brief Initialize buffer view internal data.
     *
     * @param config          Buffer view configuration.
     * @param vk_buffer_view  Buffer view Vulkan handle.
     */
    Data(BufferViewConfiguration config, ManagedVkBufferView vk_buffer_view)
      : config(config), vk_buffer_view(vk_buffer_view) {}

    /**
     * Buffer view configuration.
     */
    BufferViewConfiguration config;

    /**
     * Buffer view Vulkan handle.
     */
    ManagedVkBufferView vk_buffer_view;
  };

  /**
   * Buffer view internal data.
   */
  std::shared_ptr<Data> data_;
};

} // namespace logi

#endif