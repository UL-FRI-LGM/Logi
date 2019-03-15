#ifndef PROGRAM_LAYOUT_DESCRIPTOR_SET_LAYOUT_H
#define PROGRAM_LAYOUT_DESCRIPTOR_SET_LAYOUT_H

#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/base/ManagedResource.h"
#include "logi/program/layout/DescriptorBindingLayout.h"
#include "logi/program/layout/DescriptorCount.h"

namespace logi {

class PipelineLayout;

/**
 * @brief	Wraps Vulkan DescriptorBindingLayout data.
 */
class DescriptorSetLayout : public OwnedHandle<PipelineLayout> {
 public:
  /**
   * @brief Placeholder constructor.
   */
  DescriptorSetLayout() = default;

  /**
   * @brief Default constructor.
   *
   * @param	device					    Logical device.
   * @param	binding_intializers Structures used to populate DescriptorBindingLayouts.
   */
  DescriptorSetLayout(const PipelineLayout& pipeline_layout, const vk::Device& device,
                      const std::vector<internal::DescriptorBindingInitializer>& binding_intializers);

  /**
   * @brief	Retrieve DescriptorBindingLayout with the given binding index.
   *
   * @param	binding	Binding index.
   * @return	Pointer to the DescriptorBindingLayout or nullptr if the DescriptorBindingLayout with the give binding
   * index does not exist.
   */
  const DescriptorBindingLayout& getDescriptorBinding(uint32_t binding) const;

  /**
   * @brief	Get DescriptorSetLayout
   */
  const vk::DescriptorSetLayout& getVkHandle() const;

  /**
   * @brief	Returns DescriptorCount object that contains descriptor type counters.
   *
   * @return	DescriptorCount object.
   */
  const DescriptorsCount& getDescriptorsCount() const;

 protected:
  void free() override;

 private:
  using ManagedVkDescriptorSetLayout = ManagedResource<vk::Device, vk::DescriptorSetLayout, vk::DispatchLoaderStatic,
                                                       &vk::Device::destroyDescriptorSetLayout>;

  /**
   * @brief Descriptor set layout internal data.
   */
  struct Data {
    /**
     * @brief Initializes descriptor set layout internal data.
     *
     * @param vk_desc_layout    Descriptor set layout handle.
     * @param bindings          Descriptor bindings in this set (bindings may be defined sparsely).
     * @param descriptors_count Counts number of each descriptor type.
     */
    Data(ManagedVkDescriptorSetLayout vk_desc_layout, std::vector<DescriptorBindingLayout> bindings,
         const DescriptorsCount& descriptors_count);

    /**
     * Descriptor set layout handle.
     */
    ManagedVkDescriptorSetLayout vk_desc_layout;

    /**
     * Descriptor bindings in this set (bindings may be defined sparsely).
     */
    std::vector<DescriptorBindingLayout> bindings;

    /**
     * Counts number of each descriptor type.
     */
    DescriptorsCount descriptors_count;
  };

  /**
   * Descriptor set layout internal data.
   */
  std::shared_ptr<Data> data_;
};

} // namespace logi

#endif ///!	HEADER_GUARD