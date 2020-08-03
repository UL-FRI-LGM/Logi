#include "base/utility.h"

namespace utility
{   
    logi::CommandBuffer beginSingleTimeCommand(const VulkanState& vulkanState, const utility::PipelineType& queueType)
    {   

        logi::CommandBuffer singleTimeCommand;
        if(queueType == Graphics) {singleTimeCommand = utility::getGraphicsCommand(vulkanState);}
        else if(queueType == Compute) {singleTimeCommand = utility::getComputeCommand(vulkanState);}
        else {singleTimeCommand = utility::getPresentCommand(vulkanState);}

        vk::CommandBufferBeginInfo beginInfo;
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

        singleTimeCommand.begin(beginInfo);

        return singleTimeCommand;
    }

    logi::CommandBuffer getGraphicsCommand(const VulkanState& vulkanState)
    {
        assert(vulkanState.defaultGraphicsCommandPool_ != nullptr && "Default graphics command pool not initialized!");
        return vulkanState.defaultGraphicsCommandPool_->allocateCommandBuffer(vk::CommandBufferLevel::ePrimary);
    }

    logi::CommandBuffer getComputeCommand(const VulkanState& vulkanState)
    {
        assert(vulkanState.defaultComputeCommandPool_ != nullptr && "Default command pool not initialized!");
        return vulkanState.defaultComputeCommandPool_->allocateCommandBuffer(vk::CommandBufferLevel::ePrimary);
    }

    logi::CommandBuffer getPresentCommand(const VulkanState& vulkanState)
    {
        assert(vulkanState.defaultPresentCommandPool_ != nullptr && "Default command pool not initialized!");
        return vulkanState.defaultPresentCommandPool_->allocateCommandBuffer(vk::CommandBufferLevel::ePrimary);
    }

    void endSingleTimeCommand(const VulkanState& vulkanState, 
                              const utility::PipelineType& queueType, logi::CommandBuffer& commandBuffer)
    {   
        commandBuffer.end();

        vk::SubmitInfo submitInfo;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &static_cast<const vk::CommandBuffer&>(commandBuffer);

        if(queueType == Graphics) {utility::submitGraphicsCommand(vulkanState, submitInfo);}
        else if(queueType == Compute) {utility::submitComputeCommand(vulkanState, submitInfo);}
        else {utility::submitPresentCommand(vulkanState, submitInfo);}
    }
    
    void submitGraphicsCommand(const VulkanState& vulkanState, const vk::SubmitInfo& submitInfo)
    {
        assert(vulkanState.defaultGraphicsQueue_ != nullptr && "Default graphics queue not initialized!");
        vulkanState.defaultGraphicsQueue_->submit({submitInfo});
        vulkanState.defaultGraphicsQueue_->waitIdle();
    }
    void submitComputeCommand(const VulkanState& vulkanState, const vk::SubmitInfo& submitInfo)
    {
        assert(vulkanState.defaultComputeQueue_ != nullptr && "Default compute queue not initialized!");
        vulkanState.defaultComputeQueue_->submit({submitInfo});
        vulkanState.defaultComputeQueue_->waitIdle();
    }
    void submitPresentCommand(const VulkanState& vulkanState, const vk::SubmitInfo& submitInfo)
    {
        assert(vulkanState.defaultPresentQueue_ != nullptr && "Default present queue not initialized!");
        vulkanState.defaultPresentQueue_->submit({submitInfo});
        vulkanState.defaultPresentQueue_->waitIdle();
    }
    
} // namespace utility