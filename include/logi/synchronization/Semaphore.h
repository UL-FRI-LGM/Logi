#ifndef SYNCHRONIZATION_SEMAPHORE_H_
#define SYNCHRONIZATION_SEMAPHORE_H_

#include <vulkan/vulkan.hpp>
#include "logi/base/ManagedResource.h"
#include "logi/base/Handle.h"

namespace logi {

class Semaphore : public DependentDestroyableHandle {
public:
	/**
	 * @brief	Default placeholder constructor.
	 */
	Semaphore();

	/**
	 * @brief	Create a handle used to access Vulkan semaphore object.
	 *
	 * @param	owner	HandleManager responsible for this handle.
	 * @param	device	Vulkan device handle.
	 * @param	flags	Used to specify additional Semaphore properties.
	 */
	Semaphore(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const vk::SemaphoreCreateFlags& flags = {});

	/**
	 * @brief	Retrieve Vulkan semaphore handle.
	 *
	 * @return	Vulkan semaphore handle.
	 */
	const vk::Semaphore& getVkHandle() const;

protected:
	/**
	 * @brief	Destroy the wrapped Vulkan semaphore handle.
	 */
	void free() override;

private:
	using ManagedVkSemaphore = ManagedResource<vk::Device, vk::Semaphore, vk::DispatchLoaderStatic, &vk::Device::destroySemaphore>;
	std::shared_ptr<ManagedVkSemaphore> vk_semaphore_;
};

}

#endif //! SYNCHRONIZATION_SEMAPHORE_H_