#include "base/utility.h"

namespace utility
{
    logi::CommandBuffer beginSingleTimeCommand(logi::CommandPool& commandPool)
    {
        logi::CommandBuffer singleTimeCommand = 
            commandPool.allocateCommandBuffer(vk::CommandBufferLevel::ePrimary);

        vk::CommandBufferBeginInfo beginInfo;
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

        singleTimeCommand.begin(beginInfo);

        return singleTimeCommand;
    }

    void endSingleTimeCommand(logi::Queue& queue, logi::CommandBuffer& commandBuffer)
    {
        commandBuffer.end();

        vk::SubmitInfo submitInfo;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &static_cast<const vk::CommandBuffer&>(commandBuffer);

        queue.submit({submitInfo});
        queue.waitIdle();
    }

} // namespace utility