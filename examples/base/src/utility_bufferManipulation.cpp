#include "base/utility.h"

namespace utility
{
    void allocateBuffer(logi::MemoryAllocator& allocator, const VmaMemoryUsage& allocatorUsage,
                             std::vector<BufferAllocateInfo>& bufferAllocateInfos, std::vector<logi::VMABuffer>& bufferHandles)
    {
        assert(allocatorUsage != VMA_MEMORY_USAGE_GPU_ONLY && "To allocate on GPU use allocateBufferStaged!");

        VmaAllocationCreateInfo allocatorInfo = {};
        allocatorInfo.usage = allocatorUsage;

        for(int i = 0; i < bufferAllocateInfos.size(); i++) {
            vk::BufferCreateInfo bufferInfo = {};
            bufferInfo.size = bufferAllocateInfos[i].size;
            bufferInfo.usage = bufferAllocateInfos[i].usage;
            bufferInfo.sharingMode = bufferAllocateInfos[i].sharingMode;

            bufferHandles.emplace_back(allocator.createBuffer(bufferInfo, allocatorInfo));

            if(bufferAllocateInfos[i].data) bufferHandles[i].writeToBuffer(bufferAllocateInfos[i].data, bufferAllocateInfos[i].size);
        }        
    }

    void allocateBufferStaged(logi::Queue& queue, logi::CommandPool& commandPool, logi::MemoryAllocator& allocator, 
                                const VmaMemoryUsage& allocatorUsage, std::vector<BufferAllocateInfo>& bufferAllocateInfos,
                                     std::vector<logi::VMABuffer>& bufferHandles)
    {
        assert(allocatorUsage == VMA_MEMORY_USAGE_GPU_ONLY && "To allocate host visible use allocateBuffers!");
      
        VmaAllocationCreateInfo allocatorStagingInfo = {};
        allocatorStagingInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
            
        // Create staging buffers
        std::vector<logi::VMABuffer> stagingBuffers(bufferAllocateInfos.size());

        for(int i = 0; i < bufferAllocateInfos.size(); i++) {
            vk::BufferCreateInfo bufferInfo = {};
            bufferInfo.size = bufferAllocateInfos[i].size;
            bufferInfo.usage = vk::BufferUsageFlagBits::eTransferSrc;
            bufferInfo.sharingMode = vk::SharingMode::eExclusive;

            stagingBuffers[i] = allocator.createBuffer(bufferInfo, allocatorStagingInfo);
            stagingBuffers[i].writeToBuffer(bufferAllocateInfos[i].data, bufferAllocateInfos[i].size);
        }

        
        // Transfer to device
        logi::CommandBuffer commands = beginSingleTimeCommand(commandPool);

        VmaAllocationCreateInfo allocatorInfo = {};
        allocatorInfo.usage = allocatorUsage;

        for(int i = 0; i < stagingBuffers.size(); i++) {
            vk::BufferCreateInfo bufferInfo = {};
            bufferInfo.size = bufferAllocateInfos[i].size;
            bufferInfo.usage = bufferAllocateInfos[i].usage | vk::BufferUsageFlagBits::eTransferDst;
            bufferInfo.sharingMode = bufferAllocateInfos[i].sharingMode;

            bufferHandles.emplace_back(allocator.createBuffer(bufferInfo, allocatorInfo));

            vk::BufferCopy region;
            region.srcOffset = 0;
            region.dstOffset = 0;
            region.size = bufferAllocateInfos[i].size;

            commands.copyBuffer(stagingBuffers[i], bufferHandles[i], region);
        }

        endSingleTimeCommand(queue, commands);    
    }              
} // namespace utility