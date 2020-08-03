#include "vulkanTutorialPort.h"

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>


void VulkanTutorialPort::loadShaders()
{
    shaderReflection_ = utility::loadShaders(vulkanState_, 
                                             "./build/examples/vulkanTutorialPort/shaders/basic.vert.spv",
                                             "./build/examples/vulkanTutorialPort/shaders/basic.frag.spv");

    pipelineLayoutData_ = utility::createPipelineLayout(vulkanState_, shaderReflection_);
}

void VulkanTutorialPort::loadModel()
{
    utility::loadObjModel(MODEL_PATH, model_);
}

void VulkanTutorialPort::allocateBuffers()
{
    logi::MemoryAllocator allocator = vulkanState_.defaultLogicalDevice_->createMemoryAllocator();
    vulkanState_.addAllocator("MainAlloc", allocator);
    vulkanState_.setDefaultAllocator("MainAlloc");

    // Create model buffers
    utility::BufferAllocateInfo vertexBufferAllocate = {};
    vertexBufferAllocate.data = model_.vertices.data();
    vertexBufferAllocate.size = model_.vertices.size() * sizeof(utility::Vertex);
    vertexBufferAllocate.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    vertexBufferAllocate.sharingMode = vk::SharingMode::eExclusive;

    utility::BufferAllocateInfo indexBufferAllocate = {};
    indexBufferAllocate.data = model_.indices.data();
    indexBufferAllocate.size = model_.indices.size() * sizeof(model_.indices[0]);
    indexBufferAllocate.usage = vk::BufferUsageFlagBits::eIndexBuffer;
    indexBufferAllocate.sharingMode = vk::SharingMode::eExclusive;

    std::vector<utility::BufferAllocateInfo> bufferAllocateInfos = {vertexBufferAllocate, indexBufferAllocate};
    std::vector<logi::VMABuffer> buffers;

    utility::allocateBufferStaged(vulkanState_, VMA_MEMORY_USAGE_GPU_ONLY, bufferAllocateInfos, buffers);
    
    vertexBuffer_ = buffers[0];
    indexBuffer_ = buffers[1];


    // Create uniform buffers
    utility::BufferAllocateInfo matrixBufferAllocate = {};
    matrixBufferAllocate.data = nullptr;
    matrixBufferAllocate.size = sizeof(ubo_);
    matrixBufferAllocate.usage = vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst;
    matrixBufferAllocate.sharingMode = vk::SharingMode::eExclusive;

    std::vector<utility::BufferAllocateInfo> matrixBufferAllocations(swapchainImages_.size(), matrixBufferAllocate);

    utility::allocateBuffer(vulkanState_, VMA_MEMORY_USAGE_CPU_TO_GPU, matrixBufferAllocations, matrixUniformBuffers_);


    updateUniformBuffers();

    createTexture();
}

void VulkanTutorialPort::updateUniformBuffers() {
  // Update matrices
  ubo_.projection = glm::perspective(
    glm::radians(90.0f), (float) swapchainImageExtent_.width / (float) swapchainImageExtent_.height, 0.1f, 256.0f);
  ubo_.projection[1][1] = -1; // invert y cord
  
  glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, zoom)) * 
                            glm::lookAt(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

  ubo_.modelView = viewMatrix * glm::translate(glm::mat4(1.0f), cameraPos);
  ubo_.modelView = glm::rotate(ubo_.modelView, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  ubo_.modelView = glm::rotate(ubo_.modelView, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  ubo_.modelView = glm::rotate(ubo_.modelView, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

  for (const auto& buffer : matrixUniformBuffers_) {
    buffer.writeToBuffer(&ubo_, sizeof(ubo_));
  }
}


void VulkanTutorialPort::createDepthResource()
{   
    depthResource_.format = vk::Format::eD32Sfloat;
    depthResource_.image = utility::createImage(vulkanState_, swapchainImageExtent_.width, swapchainImageExtent_.height, depthResource_.format,
                                                 vk::ImageUsageFlagBits::eDepthStencilAttachment, VMA_MEMORY_USAGE_GPU_ONLY);

    depthResource_.imageView = depthResource_.image.createImageView({},vk::ImageViewType::e2D , depthResource_.format, {},
                                                                        vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1));
}

void VulkanTutorialPort::createTexture()
{
    texture_.image = utility::loadImageStaged(vulkanState_, VMA_MEMORY_USAGE_GPU_ONLY, vk::ImageUsageFlagBits::eSampled,
                                              vk::ImageLayout::eShaderReadOnlyOptimal, TEXTURE_PATH.c_str());

    // Image View creation
    texture_.imageView = texture_.image.createImageView({}, vk::ImageViewType::e2D, vk::Format::eR8G8B8A8Unorm, {}, 
                                                vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

    // Image sampler creation
    vk::SamplerCreateInfo samplerInfo;
    samplerInfo.magFilter = vk::Filter::eLinear;
    samplerInfo.minFilter = vk::Filter::eLinear;
    samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
    samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
    samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
    samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.compareOp = vk::CompareOp::eNever;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;
    samplerInfo.maxAnisotropy = 16.0;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.borderColor = vk::BorderColor::eFloatOpaqueWhite;

    texture_.sampler = vulkanState_.defaultLogicalDevice_->createSampler(samplerInfo);
}

void VulkanTutorialPort::initializeDescriptorSets()
{
    // Create descriptor pool
    vk::DescriptorPoolSize uniformBuffeerSize;
    uniformBuffeerSize.type = vk::DescriptorType::eUniformBuffer;
    uniformBuffeerSize.descriptorCount = static_cast<uint32_t>(swapchainImages_.size());    

    vk::DescriptorPoolSize combinedImageSamplerSize;
    combinedImageSamplerSize.type = vk::DescriptorType::eCombinedImageSampler;
    combinedImageSamplerSize.descriptorCount = static_cast<uint32_t>(swapchainImages_.size());

    std::vector<vk::DescriptorPoolSize> poolSizes = {uniformBuffeerSize, combinedImageSamplerSize};

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.maxSets = static_cast<uint32_t>(swapchainImages_.size());
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();

    descriptorPool_ = vulkanState_.defaultLogicalDevice_->createDescriptorPool(poolInfo);

    // Create descriptor sets
    std::vector<vk::DescriptorSetLayout> layouts(swapchainImages_.size(), pipelineLayoutData_.descriptorSetLayouts[0]);
    descriptorSets_ = descriptorPool_.allocateDescriptorSets(layouts);

    // Update descriptor sets
    for (size_t i = 0; i < swapchainImages_.size(); i++) {
			vk::DescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = matrixUniformBuffers_[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(ubo_);

			vk::DescriptorImageInfo imageInfo = {};
			imageInfo.sampler = texture_.sampler;
			imageInfo.imageView = texture_.imageView;
			imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

			std::vector<vk::WriteDescriptorSet> writeDescriptorSet(2);

			writeDescriptorSet[0].dstSet = descriptorSets_[i];
			writeDescriptorSet[0].dstBinding = 0;
			writeDescriptorSet[0].dstArrayElement = 0;
			writeDescriptorSet[0].descriptorCount = 1;
			writeDescriptorSet[0].descriptorType = vk::DescriptorType::eUniformBuffer;
			writeDescriptorSet[0].pBufferInfo = &bufferInfo;

			writeDescriptorSet[1].dstSet = descriptorSets_[i];
			writeDescriptorSet[1].dstBinding = 1;
			writeDescriptorSet[1].dstArrayElement = 0;
			writeDescriptorSet[1].descriptorCount = 1;
			writeDescriptorSet[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
			writeDescriptorSet[1].pImageInfo = &imageInfo;

            vulkanState_.defaultLogicalDevice_->updateDescriptorSets(writeDescriptorSet);
		}
}

void VulkanTutorialPort::createRenderPass()
{   
    vk::AttachmentDescription colorAttachment;
    colorAttachment.format = swapchainImageFormat_;
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentDescription depthAttachment;
    depthAttachment.format = depthResource_.format;
    depthAttachment.samples = vk::SampleCountFlagBits::e1;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
    depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    std::vector<vk::AttachmentDescription> attachments = {colorAttachment, depthAttachment};

    vk::AttachmentReference colorReference;
    colorReference.attachment = 0;
    colorReference.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::AttachmentReference depthReference;
    depthReference.attachment = 1;
    depthReference.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::SubpassDescription subpassDescription;
    subpassDescription.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorReference;
    subpassDescription.pDepthStencilAttachment = &depthReference;

    vk::SubpassDependency dependency;
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcAccessMask = {};
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

    vk::RenderPassCreateInfo renderPassInfo;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    renderPass_ = vulkanState_.defaultLogicalDevice_->createRenderPass(renderPassInfo);
}

void VulkanTutorialPort::createGraphicsPipeline() 
{   
    // Destroy existing pipeline.
    if (graphicsPipeline_) {
        graphicsPipeline_.destroy();
    }

    // Shader stage
    vk::PipelineShaderStageCreateInfo vertInfo;
    vertInfo.stage = vk::ShaderStageFlagBits::eVertex;
    vertInfo.module = shaderReflection_.vertexShader;
    vertInfo.pName = "main";

    vk::PipelineShaderStageCreateInfo fragInfo;
    fragInfo.stage = vk::ShaderStageFlagBits::eFragment;
    fragInfo.module = shaderReflection_.fragmentShader;
    fragInfo.pName = "main";

    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertInfo, fragInfo};

    // Vertex input
    std::vector<logi::VertexAttributeReflectionInfo> vertAttributeInfo = 
        shaderReflection_.vertexShader.getVertexAttributeReflectionInfo("main");

    std::vector<vk::VertexInputAttributeDescription> vertAttributeDescription;
    vertAttributeDescription.reserve(vertAttributeInfo.size());    

    uint32_t offset = 0;
    for(const auto& info : vertAttributeInfo) {
        vertAttributeDescription.emplace_back(info.location, vertBinding, info.format, offset);
        offset += info.elementSize;
    }

    vk::VertexInputBindingDescription bindingDescriptor(vertBinding, sizeof(utility::Vertex), vk::VertexInputRate::eVertex);

    vk::PipelineVertexInputStateCreateInfo inputStateInfo;
    inputStateInfo.vertexBindingDescriptionCount = 1;
    inputStateInfo.pVertexBindingDescriptions = &bindingDescriptor;
    inputStateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertAttributeDescription.size());
    inputStateInfo.pVertexAttributeDescriptions = vertAttributeDescription.data();

    // Input assembly
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    // Viewport
    vk::Viewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapchainImageExtent_.width;
    viewport.height = (float) swapchainImageExtent_.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Scissor
    vk::Rect2D scissor;
    scissor.extent = swapchainImageExtent_;

    vk::PipelineViewportStateCreateInfo viewportState;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // Rasterizer
    vk::PipelineRasterizationStateCreateInfo rasterizer;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
    rasterizer.depthBiasEnable = VK_FALSE;

    // Multisampling
    vk::PipelineMultisampleStateCreateInfo multisampling;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
    vk::SampleMask sampleMask(0xFFFFFF);
    multisampling.pSampleMask = &sampleMask;

    // Depth and stencil testing
    vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;
    depthStencilInfo.depthTestEnable = VK_TRUE;
    depthStencilInfo.depthWriteEnable = VK_TRUE;
    depthStencilInfo.depthCompareOp = vk::CompareOp::eLess;

    // Color blending
    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;

    vk::PipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = vk::LogicOp::eCopy;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;    

    // Dynamic state - none

    vk::GraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &inputStateInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencilInfo;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = pipelineLayoutData_.layout;
    pipelineInfo.renderPass = renderPass_;
    pipelineInfo.subpass = 0;

    graphicsPipeline_ = vulkanState_.defaultLogicalDevice_->createGraphicsPipeline(pipelineInfo);
}

void VulkanTutorialPort::createFramebuffers()
{
    // Destroy previous buffers
    for(const auto& framebuffer : framebuffers_) {
        framebuffer.destroy();
    }
    framebuffers_.clear();

    framebuffers_.reserve(swapchainImageViews_.size());
    for(const auto& imageView : swapchainImageViews_) {
        std::array<vk::ImageView, 2> attachments = { imageView , depthResource_.imageView};

        vk::FramebufferCreateInfo info;
        info.renderPass = renderPass_;
        info.attachmentCount = static_cast<uint32_t>(attachments.size());
        info.pAttachments = attachments.data();
        info.width = swapchainImageExtent_.width;
        info.height = swapchainImageExtent_.height;
        info.layers = 1;

        framebuffers_.emplace_back(vulkanState_.defaultLogicalDevice_->createFramebuffer(info));
    }
}

void VulkanTutorialPort::recordCommandBuffers()
{
  // Destroy old command buffers.
  for (const auto& cmdBuffer : primaryGraphicsCmdBuffers_) {
    cmdBuffer.reset();
  }

  for(size_t i = 0; i < primaryGraphicsCmdBuffers_.size(); i++) {
      vk::CommandBufferBeginInfo beginInfo = {};
      primaryGraphicsCmdBuffers_[i].begin(beginInfo);

      std::array<vk::ClearValue, 2> clearValues;
      clearValues[0].color.setFloat32({0.0f ,0.0f ,0.0f, 0.0f});
      clearValues[1].color.setFloat32({1.0f, 0.0f});
      
      vk::RenderPassBeginInfo renderPassBeginInfo;
      renderPassBeginInfo.renderPass = renderPass_;
      renderPassBeginInfo.framebuffer = framebuffers_[i];
      renderPassBeginInfo.renderArea.extent = swapchainImageExtent_;
      renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassBeginInfo.pClearValues = clearValues.data();

      primaryGraphicsCmdBuffers_[i].beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
      primaryGraphicsCmdBuffers_[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline_);
      primaryGraphicsCmdBuffers_[i].bindVertexBuffers(0, static_cast<const vk::Buffer&>(vertexBuffer_), VkDeviceSize(0));
      primaryGraphicsCmdBuffers_[i].bindIndexBuffer(indexBuffer_, 0, vk::IndexType::eUint32);
      primaryGraphicsCmdBuffers_[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                     pipelineLayoutData_.layout, 0, static_cast<const vk::DescriptorSet&>(descriptorSets_[i]));
      primaryGraphicsCmdBuffers_[i].drawIndexed(static_cast<uint32_t>(model_.indices.size()), 1, 0, 0, 0);
      primaryGraphicsCmdBuffers_[i].endRenderPass();
      primaryGraphicsCmdBuffers_[i].end();
  }
}

void VulkanTutorialPort::initialize()
{   
    zoom = 0.0f;
    rotation = {0.0f, 0.0f, 0.0f};

    loadShaders();
    loadModel();
    allocateBuffers();
    createDepthResource();
    initializeDescriptorSets();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    recordCommandBuffers();
}

void VulkanTutorialPort::onViewChanged()
{
    updateUniformBuffers();
}

void VulkanTutorialPort::onSwapChainRecreate()
{   
    createDepthResource();
    createGraphicsPipeline();
    createFramebuffers();
    recordCommandBuffers();
}

void VulkanTutorialPort::draw(){}