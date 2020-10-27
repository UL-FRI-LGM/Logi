#include "texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void TextureExample::loadShaders() {
  shaderReflection_ = utility::loadShaders(vulkanState_,
                                          "./shaders/texture.vert.spv",
                                          "./shaders/texture.frag.spv");

  pipelineLayoutData_ = utility::createPipelineLayout(vulkanState_, shaderReflection_);
}

void TextureExample::allocateBuffers() {
  // Set allocator
  logi::MemoryAllocator allocator = vulkanState_.defaultLogicalDevice_->createMemoryAllocator();
  vulkanState_.addAllocator("MainAlloc", allocator);
  vulkanState_.setDefaultAllocator("MainAlloc");
  // allocator_ = logicalDevice_.createMemoryAllocator();

  VmaAllocationCreateInfo allocationInfo = {};
  allocationInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_TO_GPU;

  // Create vertex and index buffer
  utility::BufferAllocateInfo vertexBufferAllocateInfo = {};
  vertexBufferAllocateInfo.data = vertices_.data();
  vertexBufferAllocateInfo.size = vertices_.size() * sizeof(Vertex);
  vertexBufferAllocateInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst;
  vertexBufferAllocateInfo.sharingMode = vk::SharingMode::eExclusive;

  utility::BufferAllocateInfo indexBufferAllocateInfo = {};
  indexBufferAllocateInfo.data = indices_.data();
  indexBufferAllocateInfo.size = indices_.size() * sizeof(uint32_t);
  indexBufferAllocateInfo.usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst;
  indexBufferAllocateInfo.sharingMode = vk::SharingMode::eExclusive;

  std::vector<utility::BufferAllocateInfo> bufferAllocateInfos = {vertexBufferAllocateInfo, indexBufferAllocateInfo};
  std::vector<logi::VMABuffer> buffers;

  utility::allocateBuffer(vulkanState_, VMA_MEMORY_USAGE_CPU_TO_GPU, bufferAllocateInfos, buffers);
  vertexBuffer_ = buffers[0];
  indexBuffer_ = buffers[1];

  // Create and init matrices UBO buffer.
  vk::BufferCreateInfo matricesBufferInfo;
  matricesBufferInfo.size = sizeof(ubo_);
  matricesBufferInfo.usage = vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst;
  matricesBufferInfo.sharingMode = vk::SharingMode::eExclusive;

  for (size_t i = 0; i < swapchainImages_.size(); i++) {
    matricesUBOBuffers_.emplace_back(vulkanState_.defaultAllocator_->createBuffer(matricesBufferInfo, allocationInfo));
  }

  updateUniformBuffers();

  // Load image.
  loadTextureImage();
}

void TextureExample::loadTextureImage() {
  int32_t texWidth, texHeight, texChannels;
  stbi_uc* pixels = stbi_load("../resources/images/zebra.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha); // Path can cause poroblems!
  VkDeviceSize imageSize = texWidth * texHeight * 4;

  if (!pixels) {
    throw std::runtime_error("failed to load texture image!");
  }

  VmaAllocationCreateInfo allocationInfo = {};
  allocationInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_TO_GPU;

  vk::ImageCreateInfo imageInfo;
  imageInfo.imageType = vk::ImageType::e2D;
  imageInfo.format = vk::Format::eR8G8B8A8Unorm;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.samples = vk::SampleCountFlagBits::e1;
  imageInfo.tiling = vk::ImageTiling::eLinear;
  imageInfo.sharingMode = vk::SharingMode::eExclusive;
  // Set initial layout of the image to undefined
  imageInfo.initialLayout = vk::ImageLayout::ePreinitialized;
  imageInfo.extent = vk::Extent3D(texWidth, texHeight, 1);
  imageInfo.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;

  texture_.image = vulkanState_.defaultAllocator_->createImage(imageInfo, allocationInfo);
  texture_.image.writeToImage(pixels, imageSize);

  logi::CommandBuffer cpyCmdBuffer = vulkanState_.defaultGraphicsCommandPool_->allocateCommandBuffer(vk::CommandBufferLevel::ePrimary);
  cpyCmdBuffer.begin({});

  // Transition the texture image layout to shader read, so it can be sampled from
  vk::ImageMemoryBarrier imageMemoryBarrier;
  imageMemoryBarrier.image = texture_.image;
  imageMemoryBarrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
  imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
  imageMemoryBarrier.subresourceRange.levelCount = 1;
  imageMemoryBarrier.subresourceRange.layerCount = 1;
  imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
  imageMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
  imageMemoryBarrier.oldLayout = vk::ImageLayout::ePreinitialized;
  imageMemoryBarrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

  cpyCmdBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eHost, vk::PipelineStageFlagBits::eFragmentShader, {}, {}, {},
                               imageMemoryBarrier);
  cpyCmdBuffer.end();

  vk::SubmitInfo submitInfo;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &static_cast<const vk::CommandBuffer&>(cpyCmdBuffer);
  vulkanState_.defaultGraphicsQueue_->submit(submitInfo);
  vulkanState_.defaultGraphicsQueue_->waitIdle();

  // Create image view.
  texture_.imageView =
    texture_.image.createImageView({}, vk::ImageViewType::e2D, vk::Format::eR8G8B8A8Unorm, {},
                                   vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

  // Create sampler
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
  samplerInfo.maxAnisotropy = 1.0;
  samplerInfo.anisotropyEnable = VK_FALSE;
  samplerInfo.borderColor = vk::BorderColor::eFloatOpaqueWhite;

  texture_.sampler = vulkanState_.defaultLogicalDevice_->createSampler(samplerInfo);
}

void TextureExample::updateUniformBuffers() {
  // Update matrices
  ubo_.projectionMatrix = glm::perspective(
    glm::radians(60.0f), (float) swapchainImageExtent_.width / (float) swapchainImageExtent_.height, 0.1f, 256.0f);
  glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, zoom));

  ubo_.modelViewMatrix = viewMatrix * glm::translate(glm::mat4(1.0f), cameraPos);
  ubo_.modelViewMatrix = glm::rotate(ubo_.modelViewMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  ubo_.modelViewMatrix = glm::rotate(ubo_.modelViewMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  ubo_.modelViewMatrix = glm::rotate(ubo_.modelViewMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

  ubo_.viewPos = glm::vec4(0.0f, 0.0f, -zoom, 0.0f);

  for (const auto& buffer : matricesUBOBuffers_) {
    buffer.writeToBuffer(&ubo_, sizeof(ubo_));
  }
}

void TextureExample::initializeDescriptorSets() {
  // Create descriptor pool.
  vk::DescriptorPoolSize poolUBOSize;
  poolUBOSize.type = vk::DescriptorType::eUniformBuffer;
  poolUBOSize.descriptorCount = static_cast<uint32_t>(swapchainImages_.size());

  vk::DescriptorPoolSize poolCombinedImageSamplerSize;
  poolCombinedImageSamplerSize.type = vk::DescriptorType::eCombinedImageSampler;
  poolCombinedImageSamplerSize.descriptorCount = static_cast<uint32_t>(swapchainImages_.size());

  std::vector<vk::DescriptorPoolSize> poolSizes = {poolUBOSize, poolCombinedImageSamplerSize};

  vk::DescriptorPoolCreateInfo poolInfo;
  poolInfo.pPoolSizes = poolSizes.data();
  poolInfo.poolSizeCount = poolSizes.size();
  poolInfo.maxSets = static_cast<uint32_t>(swapchainImages_.size() * 2);

  descriptorPool_ = vulkanState_.defaultLogicalDevice_->createDescriptorPool(poolInfo);

  // Create descriptor sets.
  std::vector<vk::DescriptorSetLayout> layouts(swapchainImages_.size(), pipelineLayoutData_.descriptorSetLayouts[0]);
  descriptorSets_ = descriptorPool_.allocateDescriptorSets(layouts);

  for (size_t i = 0; i < descriptorSets_.size(); i++) {
    vk::DescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = matricesUBOBuffers_[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(ubo_);

    std::vector<vk::WriteDescriptorSet> descriptorWrites(2);

    descriptorWrites[0].dstSet = descriptorSets_[i];
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;

    vk::DescriptorImageInfo textureDescriptor;
    textureDescriptor.imageView = texture_.imageView;
    textureDescriptor.sampler = texture_.sampler;
    textureDescriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

    descriptorWrites[1].dstSet = descriptorSets_[i];
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &textureDescriptor;

    vulkanState_.defaultLogicalDevice_->updateDescriptorSets(descriptorWrites);
  }
}

void TextureExample::createRenderPass() {
  vk::AttachmentDescription colorAttachment;
  colorAttachment.format = swapchainImageFormat_;
  colorAttachment.samples = vk::SampleCountFlagBits::e1;
  colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
  colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
  colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eStore;
  colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
  colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

  vk::AttachmentReference colorAttachmentRef;
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

  vk::SubpassDescription subpass;
  subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  vk::SubpassDependency dependency;
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
  dependency.srcAccessMask = {};
  dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
  dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

  vk::RenderPassCreateInfo renderPassCreateInfo;
  renderPassCreateInfo.attachmentCount = 1;
  renderPassCreateInfo.pAttachments = &colorAttachment;
  renderPassCreateInfo.subpassCount = 1;
  renderPassCreateInfo.pSubpasses = &subpass;
  renderPassCreateInfo.dependencyCount = 1;
  renderPassCreateInfo.pDependencies = &dependency;

  renderPass_ = vulkanState_.defaultLogicalDevice_->createRenderPass(renderPassCreateInfo);
}

void TextureExample::createGraphicalPipeline() {
  // Destroy existing pipeline.
  if (pipeline_) {
    pipeline_.destroy();
  }

  // Pipeline
  vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
  vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
  vertShaderStageInfo.module = shaderReflection_.vertexShader;
  vertShaderStageInfo.pName = "main";

  vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
  fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
  fragShaderStageInfo.module = shaderReflection_.fragmentShader;
  fragShaderStageInfo.pName = "main";

  vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

  std::vector<logi::VertexAttributeReflectionInfo> vtxAttributeInfo =
    shaderReflection_.vertexShader.getVertexAttributeReflectionInfo("main");

  std::vector<vk::VertexInputAttributeDescription> vtxAttributeDescriptions;
  vtxAttributeDescriptions.reserve(vtxAttributeInfo.size());

  uint32_t offset = 0u;
  for (const auto& entry : vtxAttributeInfo) {
    vtxAttributeDescriptions.emplace_back(entry.location, kVertexBinding, entry.format, offset);
    offset += entry.elementSize;
  }

  vk::VertexInputBindingDescription vtxBindingDescription(kVertexBinding, sizeof(Vertex), vk::VertexInputRate::eVertex);

  vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
  vertexInputInfo.pVertexBindingDescriptions = &vtxBindingDescription;
  vertexInputInfo.vertexBindingDescriptionCount = 1u;
  vertexInputInfo.pVertexAttributeDescriptions = vtxAttributeDescriptions.data();
  vertexInputInfo.vertexAttributeDescriptionCount = vtxAttributeDescriptions.size();

  vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
  inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  vk::Viewport viewport = {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float) swapchainImageExtent_.width;
  viewport.height = (float) swapchainImageExtent_.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  vk::Rect2D scissor;
  scissor.extent = swapchainImageExtent_;

  vk::PipelineViewportStateCreateInfo viewportState;
  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;

  vk::PipelineRasterizationStateCreateInfo rasterizer;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = vk::PolygonMode::eFill;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = vk::CullModeFlagBits::eNone;
  rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
  rasterizer.depthBiasEnable = VK_FALSE;

  vk::PipelineMultisampleStateCreateInfo multisampling;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
  vk::SampleMask sampleMask(0xFFFFFF);
  multisampling.pSampleMask = &sampleMask;

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

  vk::GraphicsPipelineCreateInfo pipelineInfo;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.layout = pipelineLayoutData_.layout;
  pipelineInfo.renderPass = renderPass_;
  pipelineInfo.subpass = 0;

  pipeline_ = vulkanState_.defaultLogicalDevice_->createGraphicsPipeline(pipelineInfo);
}

void TextureExample::createFrameBuffers() {
  // Destroy previous framebuffers.
  for (const auto& framebuffer : framebuffers_) {
    framebuffer.destroy();
  }
  framebuffers_.clear();

  // Create new framebuffers.
  for (const auto& imageView : swapchainImageViews_) {
    vk::FramebufferCreateInfo createInfo;
    createInfo.renderPass = renderPass_;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &static_cast<const vk::ImageView&>(imageView);
    createInfo.width = swapchainImageExtent_.width;
    createInfo.height = swapchainImageExtent_.height;
    createInfo.layers = 1;

    framebuffers_.emplace_back(vulkanState_.defaultLogicalDevice_->createFramebuffer(createInfo));
  }
}

void TextureExample::recordCommandBuffers() {
  // Destroy old command buffers.
  for (const auto& cmdBuffer : primaryGraphicsCmdBuffers_) {
    cmdBuffer.reset();
  }

  for (size_t i = 0; i < primaryGraphicsCmdBuffers_.size(); i++) {
    vk::CommandBufferBeginInfo beginInfo = {};
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;

    primaryGraphicsCmdBuffers_[i].begin(beginInfo);

    vk::RenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.renderPass = renderPass_;
    renderPassInfo.framebuffer = framebuffers_[i];
    renderPassInfo.renderArea.extent = swapchainImageExtent_;

    vk::ClearValue clearValue;
    clearValue.color.setFloat32({0.0, 0.0, 0.0, 1.0});
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearValue;

    primaryGraphicsCmdBuffers_[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
    primaryGraphicsCmdBuffers_[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline_);

    primaryGraphicsCmdBuffers_[i].bindVertexBuffers(0, static_cast<const vk::Buffer&>(vertexBuffer_), VkDeviceSize(0));
    primaryGraphicsCmdBuffers_[i].bindIndexBuffer(indexBuffer_, 0, vk::IndexType::eUint32);
    primaryGraphicsCmdBuffers_[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayoutData_.layout, 0,
                                                     static_cast<const vk::DescriptorSet&>(descriptorSets_[i]));

    primaryGraphicsCmdBuffers_[i].drawIndexed(indices_.size(), 1, 0, 0, 0);
    primaryGraphicsCmdBuffers_[i].endRenderPass();
    primaryGraphicsCmdBuffers_[i].end();
  }
}

void TextureExample::onViewChanged() {
  updateUniformBuffers();
}

void TextureExample::onSwapChainRecreate() {
  createFrameBuffers();
  createGraphicalPipeline();
  recordCommandBuffers();
}

void TextureExample::initialize() {
  zoom = -2.5f;
  rotation = {-15.0f, 15.0f, 0.0f};

  loadShaders();
  allocateBuffers();
  initializeDescriptorSets();

  createRenderPass();
  createFrameBuffers();
  createGraphicalPipeline();
  recordCommandBuffers();
}

void TextureExample::draw() {}
