#include "fractals.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

void Fractals::loadShaders() {
  shaderReflection_ = utility::loadShaders(vulkanState_,
                                           "./shaders/rayMarch.vert.spv",
                                           "./shaders/rayMarch.frag.spv");

  pipelineLayoutData_ = utility::createPipelineLayout(vulkanState_, shaderReflection_);
}

void Fractals::allocateBuffers() {
  // Set allocator
  logi::MemoryAllocator allocator = vulkanState_.defaultLogicalDevice_->createMemoryAllocator();
  vulkanState_.addAllocator("MainAlloc", allocator);
  vulkanState_.setDefaultAllocator("MainAlloc");

  // Create vertex buffer
  utility::BufferAllocateInfo vertexBufferAllocate = {};
  vertexBufferAllocate.data = vertices.data();
  vertexBufferAllocate.size = vertices.size() * sizeof(Vertex);
  vertexBufferAllocate.usage = vk::BufferUsageFlagBits::eVertexBuffer;
  vertexBufferAllocate.sharingMode = vk::SharingMode::eExclusive;

  std::vector<utility::BufferAllocateInfo> bufferAllocateInfo = {vertexBufferAllocate};
  std::vector<logi::VMABuffer> buffers;

  utility::allocateBuffer(vulkanState_, VMA_MEMORY_USAGE_CPU_TO_GPU, bufferAllocateInfo, buffers);\
  vertexBuffer_ = buffers[0];

  // Create and init matrices UBO buffer
  VmaAllocationCreateInfo allocationInfo = {};
  allocationInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_TO_GPU;

  vk::BufferCreateInfo matricesBufferInfo;
  matricesBufferInfo.size = sizeof(shaderSettings);
  matricesBufferInfo.usage = vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst;
  matricesBufferInfo.sharingMode = vk::SharingMode::eExclusive;

  for (size_t i = 0; i < swapchainImages_.size(); i++) {
    matricesUBOBuffers_.emplace_back(vulkanState_.defaultAllocator_->createBuffer(matricesBufferInfo, allocationInfo));
  }

  updateMatrixBuffers();
}

void Fractals::updateMatrixBuffers() {
  shaderSettings.view = glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  shaderSettings.view = glm::rotate(shaderSettings.view, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  
  shaderSettings.zoom = zoom;

  for (const auto& buffer : matricesUBOBuffers_) {
    buffer.writeToBuffer(&shaderSettings, sizeof(shaderSettings));
  }
}

void Fractals::initializeDescriptorSets() {
  // Create descriptor pool.
  vk::DescriptorPoolSize poolSize;
  poolSize.type = vk::DescriptorType::eUniformBuffer;
  poolSize.descriptorCount = 2 * static_cast<uint32_t>(swapchainImages_.size());

  vk::DescriptorPoolCreateInfo poolInfo;
  poolInfo.pPoolSizes = &poolSize;
  poolInfo.poolSizeCount = 1u;
  poolInfo.maxSets = static_cast<uint32_t>(swapchainImages_.size());

  descriptorPool_ = vulkanState_.defaultLogicalDevice_->createDescriptorPool(poolInfo);

  // Create UBO matrices descriptors
  std::vector<vk::DescriptorSetLayout> layouts(swapchainImages_.size(), pipelineLayoutData_.descriptorSetLayouts[0]);
  descriptorSets_ = descriptorPool_.allocateDescriptorSets(layouts);

  for (size_t i = 0; i < descriptorSets_.size(); i++) {
    vk::DescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = matricesUBOBuffers_[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(shaderSettings);

    vk::WriteDescriptorSet descriptorWrite;
    descriptorWrite.dstSet = descriptorSets_[i];
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vulkanState_.defaultLogicalDevice_->updateDescriptorSets(descriptorWrite);
  }
}

void Fractals::createRenderPass() {
  vk::AttachmentDescription colorAttachment;
  colorAttachment.format = swapchainImageFormat_;
  colorAttachment.samples = vk::SampleCountFlagBits::e1;
  colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
  colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
  colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eStore;
  colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
  colorAttachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal; // Input to ImGUI render pass!

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

void Fractals::createGraphicalPipeline() {
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
  rasterizer.frontFace = vk::FrontFace::eClockwise;
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

void Fractals::createFrameBuffers() {
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

void Fractals::recordCommandBuffers() {
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

    primaryGraphicsCmdBuffers_[i].bindVertexBuffers(0, static_cast<vk::Buffer>(vertexBuffer_), static_cast<VkDeviceSize>(0ul));

    primaryGraphicsCmdBuffers_[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayoutData_.layout, 0,
                                                     static_cast<vk::DescriptorSet>(descriptorSets_[i]));
                                                
    // primaryGraphicsCmdBuffers_[i].pushConstants(pipelineLayoutData_.layout, vk::ShaderStageFlagBits::eFragment,
    //                                             0, static_cast<vk::ArrayProxy<const Settings>>(shader_settings));


    primaryGraphicsCmdBuffers_[i].draw((uint32_t)(vertices.size()), 1, 0, 0);
    primaryGraphicsCmdBuffers_[i].endRenderPass();
    primaryGraphicsCmdBuffers_[i].end();
  }
}

void Fractals::imGUI_createUI() {
  ImGuiWindowFlags fpsWindowFlags = ImGuiWindowFlags_NoTitleBar | 
                                    ImGuiWindowFlags_NoBackground | 
                                    ImGuiWindowFlags_NoResize;
  
  ImGui::Begin("FPS", NULL, fpsWindowFlags); 
  ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();


  ImGuiWindowFlags settingWindowFlags = 0;
  if (!ImGui::Begin("Settings", NULL, settingWindowFlags)) { // Optimization if window is collapsed
        ImGui::End();
        return;
  }

  // ImGuiInputTextFlags floatInput = ImGuiInputTextFlags_EnterReturnsTrue;
  if (ImGui::CollapsingHeader("Camera")) {
    ImGui::InputFloat("fov", &shaderSettings.fov, 0.1f, 0.1f, "%.1f");
  }

  if (ImGui::CollapsingHeader("Light")) {
    if (ImGui::Button("Dir light shading")) {
      shaderSettings.dirLightShading = std::abs(shaderSettings.dirLightShading - 1.0);
    }

    ImGui::ColorEdit3("Dir color", glm::value_ptr(shaderSettings.dirLightColor));
    ImGui::InputFloat("Dir intensity", &shaderSettings.dirIntensity, 0.01f, 0.01f, "%.3f");

    ImGui::ColorEdit3("Specular color", glm::value_ptr(shaderSettings.specularColor));
    ImGui::InputFloat("Specular intensity", &shaderSettings.specularIntensity, 0.01f, 0.01f, "%.3f");

    ImGui::ColorEdit3("Background color", glm::value_ptr(shaderSettings.backgroundColor));
    ImGui::InputFloat("Shadow sharpness", &shaderSettings.shadowSharpness, 0.1f, 0.1f, "%.1f");
  }

  if (ImGui::CollapsingHeader("Effects")) {
    ImGui::ColorEdit3("Glow color", glm::value_ptr(shaderSettings.glowColor));
    ImGui::InputFloat("Glow intensity", &shaderSettings.glowIntensity, 0.001f, 0.001f, "%.3f");

    ImGui::ColorEdit3("AO color", glm::value_ptr(shaderSettings.AOColor));
    ImGui::InputFloat("AO intensity", &shaderSettings.AOIntensity, 0.001f, 0.001f, "%.3f");

    ImGui::InputFloat("Fog curve", &shaderSettings.fogCurve, 0.1f, 0.1f, "%.1f");
    ImGui::InputFloat("Fog start", &shaderSettings.fogStartDist, 0.1f, 0.1f, "%.1f");
  }

  if (ImGui::CollapsingHeader("Fractals")) {
    ImGui::InputFloat("Min dist", &shaderSettings.minDist, 0.0001f, 0.0001f, "%.4f");
    ImGui::InputInt("Iterations", &shaderSettings.iterations);
    ImGui::InputFloat("Power(mandel bulb)", &shaderSettings.fractalPower, 0.01f, 0.01f, "%.3f");
    ImGui::Combo("Fractal type", &shaderSettings.fractalType, "Meneger box\0Mandel box\0Mandel bulb\0Meneger sponge\0Serpinski triangle\0\0");
    
    if (ImGui::Button("Reset to default") || (shaderSettings.fractalType != previousFractalType)) {
      zoom = 0.0f;
      previousFractalType = shaderSettings.fractalType;

      switch(shaderSettings.fractalType) {
        case 0:
          shaderSettings.iterations = 8;
          shaderSettings.cameraOrigin.z = 10.0f;
          break;
        case 1:
        shaderSettings.iterations = 8;
          shaderSettings.cameraOrigin.z = 30.0f;
          break;
        case 2:
          shaderSettings.iterations = 12;
          shaderSettings.fractalPower = 8.0;
          shaderSettings.cameraOrigin.z = 4.0f;
          break;
        case 3:
          shaderSettings.iterations = 4;
          shaderSettings.cameraOrigin.z = 10.0f;
          break;
        case 4:
          shaderSettings.iterations = 14;
          shaderSettings.cameraOrigin.z = 10.0f;
          break;      
      }
    }
  }


  ImGui::End();

  updateMatrixBuffers(); // Better solution?
}

void Fractals::onViewChanged() {
  updateMatrixBuffers();
}

void Fractals::onSwapChainRecreate() {
  createFrameBuffers();
  createGraphicalPipeline();

  shaderSettings.viewportWidth = (float)config_.windowWidth;
  shaderSettings.viewportHeight = (float)config_.windowHeight;
  shaderSettings.aspectRatio = (float)swapchainImageExtent_.height / (float)swapchainImageExtent_.width;

  recordCommandBuffers();
}

void Fractals::initialize() {
  // Init push constant
  zoom = 0.0f;
  shaderSettings.viewportWidth = (float)config_.windowWidth;
  shaderSettings.viewportHeight = (float)config_.windowHeight;
  shaderSettings.aspectRatio = (float)config_.windowHeight / (float)config_.windowWidth;

  loadShaders();
  allocateBuffers();
  initializeDescriptorSets();

  createRenderPass();
  createFrameBuffers();
  createGraphicalPipeline();
  recordCommandBuffers();

  imGUI_initOverlay();
}

void Fractals::draw() {}


