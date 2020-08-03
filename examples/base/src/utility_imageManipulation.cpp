#include "base/utility.h"

#define STB_IMAGE_IMPLEMENTATION
#include "base/stb_image.h"

namespace utility {
    
    logi::VMAImage loadImage(const VulkanState& vulkanState, const VmaMemoryUsage& memoryUsage, 
                        const vk::ImageUsageFlagBits& usage, const char* path, const uint32_t& mipLevels, 
                        const vk::SampleCountFlagBits& sampleCount)
    {   
        assert(vulkanState.defaultAllocator_ != nullptr && "Default allocator not initialized!");
        assert(memoryUsage != VMA_MEMORY_USAGE_GPU_ONLY && "To allocate on GPU use loadImageStaged!");

        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if(!pixels) {
            throw std::runtime_error("failed to load texture image!");
        }


    	logi::VMAImage image;

        VmaAllocationCreateInfo allocationInfo = {};
        allocationInfo.usage = memoryUsage;

        vk::ImageCreateInfo imageInfo;
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.format = vk::Format::eR8G8B8A8Unorm;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = sampleCount;
        imageInfo.tiling = vk::ImageTiling::eLinear;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;

        // Set initial layout of the image to undefined
        imageInfo.initialLayout = vk::ImageLayout::ePreinitialized;

        imageInfo.extent = vk::Extent3D(texWidth, texHeight, 1);
        imageInfo.usage = vk::ImageUsageFlagBits::eTransferDst | usage;

        image = vulkanState.defaultAllocator_->createImage(imageInfo, allocationInfo);
        image.writeToImage(pixels, imageSize);

            
        stbi_image_free(pixels);

        return image;        
    }

    logi::VMAImage loadImageStaged(const VulkanState& vulkanState, const VmaMemoryUsage& memoryUsage, 
                                   const vk::ImageUsageFlagBits& usage, const vk::ImageLayout& layout, const char* path,
                                   const uint32_t& mipLevels, const vk::SampleCountFlagBits& sampleCount)
    {   
        assert(vulkanState.defaultAllocator_ != nullptr && "Default allocator not initialized!");
        assert(memoryUsage == VMA_MEMORY_USAGE_GPU_ONLY && "To allocate host visible use loadImage!");

        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if(!pixels) {
            throw std::runtime_error("failed to load texture image!");
        }


    	logi::VMAImage image;

        // Staging buffer
        VmaAllocationCreateInfo allocStaged = {};
        allocStaged.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_ONLY;   

        vk::BufferCreateInfo imageStaging;
        imageStaging.size = imageSize;
        imageStaging.usage = vk::BufferUsageFlagBits::eTransferSrc;
        imageStaging.sharingMode = vk::SharingMode::eExclusive;

        logi::VMABuffer stagingBuffer = vulkanState.defaultAllocator_->createBuffer(imageStaging, allocStaged);
        stagingBuffer.writeToBuffer(pixels, imageSize);  
                
        // Image allocation
        image = createImage(vulkanState, texWidth, texHeight, vk::Format::eR8G8B8A8Unorm, 
                            vk::ImageUsageFlagBits::eTransferDst | usage, memoryUsage);

        transitionImageLayout(vulkanState, image, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

        copyBufferToImage(vulkanState, stagingBuffer, image, texWidth, texHeight);

        transitionImageLayout(vulkanState, image, vk::ImageLayout::eTransferDstOptimal, layout);


        stbi_image_free(pixels);

        return image;
    }

    logi::VMAImage createImage(const VulkanState& vulkanState, const uint32_t& width, const uint32_t& height, const vk::Format& format,
                               const vk::ImageUsageFlags& usage, const VmaMemoryUsage& memoryUsage, const uint32_t& mipLevels, 
                               const vk::SampleCountFlagBits& sampleCount)
    {   
        assert(vulkanState.defaultAllocator_ != nullptr && "Default allocator not initialized!");

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = memoryUsage;

        vk::ImageCreateInfo imageInfo;  
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.format = format;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = sampleCount;
        imageInfo.tiling = vk::ImageTiling::eOptimal;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;

        // Set initial layout of the image to undefined
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;

        imageInfo.extent = vk::Extent3D(width, height, 1);
        imageInfo.usage = usage;

        return vulkanState.defaultAllocator_->createImage(imageInfo, allocInfo);   
    }

    void transitionImageLayout(const VulkanState& vulkanState, logi::VMAImage& image, const vk::ImageLayout& oldLayout,
                               const vk::ImageLayout& newLayout, const uint32_t& mipLevel)
    {
        logi::CommandBuffer commandBuffer = utility::beginSingleTimeCommand(vulkanState, utility::Graphics);

        vk::ImageMemoryBarrier barrier = {};
        barrier.image = image;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = mipLevel;

        vk::PipelineStageFlags sourceStage;
        vk::PipelineStageFlags destinationStage;

        if(oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
            
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

            sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
            destinationStage = vk::PipelineStageFlagBits::eTransfer;

        } else if(oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
           
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

            sourceStage = vk::PipelineStageFlagBits::eTransfer;
            destinationStage = vk::PipelineStageFlagBits::eFragmentShader;

        } else if(oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eColorAttachmentOptimal) {
            
            barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

            sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
            destinationStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;

        } else {
            throw std::runtime_error("unsupported layout transition!");
        }

        commandBuffer.pipelineBarrier(sourceStage, destinationStage,
                                        {}, {}, {}, barrier);

        utility::endSingleTimeCommand(vulkanState, Graphics, commandBuffer);
    }

    void copyBufferToImage(const VulkanState& vulkanState, logi::VMABuffer& buffer, 
                           logi::Image& image, const uint32_t& width, const uint32_t& height)
    {
        logi::CommandBuffer commandBuffer = utility::beginSingleTimeCommand(vulkanState, utility::Graphics);

        vk::BufferImageCopy region;
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {width, height, 1};

        commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, region);

        utility::endSingleTimeCommand(vulkanState, Graphics, commandBuffer);
    }
    
} // namespace utility