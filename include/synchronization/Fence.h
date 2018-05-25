#ifndef SYNCHRONIZATION_FENCE_H_
#define SYNCHRONIZATION_FENCE_H_

#include <vulkan/vulkan.hpp>
#include "base/ManagedResource.h"

namespace vkr {

using ManagedVkFence = ManagedResource<vk::Device, vk::Fence, vk::DispatchLoaderStatic, &vk::Device::destroyFence>;

class Fence {
public:
	/**
	 * @brief	Handle used to access Fence resource.
	 *
	 * @param	vk_fence	Managed Vulkan Fence object.
	 */
	explicit Fence(const std::shared_ptr<ManagedVkFence>& vk_fence);

	/**
	 * @brief Return true if the fence is signaled.
	 *
	 * @return True if the fence is signaled.
	 */
	bool getStatus() const;

	/**
	 * @brief Resets the state to unsignaled.
	 */
	void reset() const;

	/**
	 * @brief Wait for the fence to be signaled.
	 *
	 * @param timeout Timeout time in nanoseconds.
	 * @return Vulkan operation result.
	 */
	vk::Result wait(uint64_t timeout) const;

	/**
	 * @brief Wait for the fences to be signaled.
	 * @note All fences must originate from the same device!
	 * TODO(plavric): Change this so that the fences wont have to orignate from the same device.
	 *
	 * @param	fences		Vector containing the fences.
	 * @param	timeout		Timeout time in nanoseconds.
	 * @param	wait_all	If true the operation will block until all fences are signaled, otherwise until the first fence is signaled.
	 * @return	Vulkan wait result.
	 */
	static vk::Result wait(const std::vector<Fence>& fences, bool wait_all, uint64_t timeout);

	/**
	 * @brief Retrieve vk::Fence handle.
	 *
	 * @return vk::Fence handle.
	 */
	const vk::Fence& getVkHandle() const;

	/**
	 * @brief Free Vulkan fence resources.
	 */
	void destroy() const;

private:
	std::shared_ptr<ManagedVkFence> vk_fence_;
};

}

#endif //! SYNCHRONIZATION_FENCE_H_