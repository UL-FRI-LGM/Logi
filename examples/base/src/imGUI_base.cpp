#include "imGUI_base.h"

void ImGUIBase::imGUI_initOverlay() {

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io_ = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup GLFW binding
    ImGui_ImplGlfw_InitForVulkan(window_.glfwHandle(), true);

    // Setup Vulkan biniding
    imGUI_createDescriptorPool();
    imGUI_createRenderPass();
    imGUI_createCommandBuffers();
    imGUI_createFrameBuffers();

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = static_cast<vk::Instance>(vulkanState_.instance_);
    init_info.PhysicalDevice = static_cast<vk::PhysicalDevice>(vulkanState_.physicalDevice_);
    init_info.Device = static_cast<vk::Device>(*vulkanState_.defaultLogicalDevice_);
    init_info.QueueFamily = vulkanState_.graphicsFamily_;
    init_info.Queue = static_cast<vk::Queue>(*vulkanState_.defaultGraphicsQueue_);
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = static_cast<vk::DescriptorPool>(imGUI_descriptorPool_);
    init_info.Allocator = nullptr;
    init_info.MinImageCount = 2;
    init_info.ImageCount = imageCount_;
    init_info.CheckVkResultFn = checkVkResult;

    ImGui_ImplVulkan_Init(&init_info, static_cast<vk::RenderPass>(imGUI_renderPass_));

    // Upload fonts
    logi::CommandBuffer loadFontCommandBuffer = utility::beginSingleTimeCommand(vulkanState_, utility::Graphics);
    ImGui_ImplVulkan_CreateFontsTexture(static_cast<vk::CommandBuffer>(loadFontCommandBuffer));
    utility::endSingleTimeCommand(vulkanState_, utility::Graphics, loadFontCommandBuffer);
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void ImGUIBase::imGUI_createDescriptorPool() {

    vk::DescriptorPoolSize poolSizes[] =
        {
            { vk::DescriptorType::eSampler, 1000 },
            { vk::DescriptorType::eCombinedImageSampler, 1000 },
            { vk::DescriptorType::eSampledImage, 1000 },
            { vk::DescriptorType::eStorageImage, 1000 },
            { vk::DescriptorType::eUniformTexelBuffer, 1000 },
            { vk::DescriptorType::eStorageTexelBuffer, 1000 },
            { vk::DescriptorType::eUniformBuffer, 1000 },
            { vk::DescriptorType::eStorageBuffer, 1000 },
            { vk::DescriptorType::eUniformBufferDynamic, 1000 },
            { vk::DescriptorType::eStorageBufferDynamic, 1000 },
            { vk::DescriptorType::eInputAttachment, 1000 }
        };

    vk::DescriptorPoolCreateInfo poolInfo = {};
    poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
    poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
    poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
    poolInfo.pPoolSizes = poolSizes;

    imGUI_descriptorPool_ = vulkanState_.defaultLogicalDevice_->createDescriptorPool(poolInfo);
}

void ImGUIBase::imGUI_createRenderPass() {

    vk::AttachmentDescription attachment;
    attachment.format = swapchainImageFormat_;
    attachment.samples = vk::SampleCountFlagBits::e1;
    attachment.loadOp = vk::AttachmentLoadOp::eLoad;
    attachment.storeOp = vk::AttachmentStoreOp::eStore;
    attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    attachment.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
    attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentReference attachmentReference;
    attachmentReference.attachment = 0;
    attachmentReference.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &attachmentReference;

    vk::SubpassDependency dependency;
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

    vk::RenderPassCreateInfo createInfo;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &attachment;
    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subpass;
    createInfo.dependencyCount = 1;
    createInfo.pDependencies = &dependency;

    imGUI_renderPass_ = vulkanState_.defaultLogicalDevice_->createRenderPass(createInfo);
}

void ImGUIBase::imGUI_createCommandBuffers() {
    imGUI_commandPool_ = 
        vulkanState_.graphicsFamily_.createCommandPool(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
    
    imGUI_commandBuffers_ = 
        imGUI_commandPool_.allocateCommandBuffers(vk::CommandBufferLevel::ePrimary, swapchainImages_.size());
}

void ImGUIBase::imGUI_createFrameBuffers() {
    // Destroy previous framebuffers
    for (const auto& framebuffer : imGUI_framebuffers_) {
        framebuffer.destroy();
    }
    imGUI_framebuffers_.clear();

    // Create new framebuffers.
    for (const auto& imageView : swapchainImageViews_) {
        vk::FramebufferCreateInfo createInfo;
        createInfo.renderPass = imGUI_renderPass_;
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = &static_cast<const vk::ImageView&>(imageView);
        createInfo.width = swapchainImageExtent_.width;
        createInfo.height = swapchainImageExtent_.height;
        createInfo.layers = 1;

        imGUI_framebuffers_.emplace_back(vulkanState_.defaultLogicalDevice_->createFramebuffer(createInfo));
    }
}

void ImGUIBase::imGUI_createUI() {
    ImGui::ShowDemoWindow();
}

void ImGUIBase::recreateSwapChain() {
    vulkanState_.defaultLogicalDevice_->waitIdle();

    ImGui_ImplVulkan_SetMinImageCount(imageCount_);

    initializeSwapChain();
    imGUI_createFrameBuffers();
    onSwapChainRecreate();
}

// Only needed command buffer is generated
logi::CommandBuffer* ImGUIBase::imGUI_createOverlay(const uint32_t& i) {

    vk::CommandBufferBeginInfo beginInfo = {};
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;

    imGUI_commandBuffers_[i].begin(beginInfo);

    vk::RenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.renderPass = imGUI_renderPass_;
    renderPassInfo.framebuffer = imGUI_framebuffers_[i];
    renderPassInfo.renderArea.extent = swapchainImageExtent_;

    vk::ClearValue clearValue;
    clearValue.color.setFloat32({0.0, 0.0, 0.0, 1.0});
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearValue;

    imGUI_commandBuffers_[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), static_cast<vk::CommandBuffer>(imGUI_commandBuffers_[i]));
    
    imGUI_commandBuffers_[i].endRenderPass();
    imGUI_commandBuffers_[i].end();

    return &imGUI_commandBuffers_[i];
}

void ImGUIBase::mainLoop() {

  while (!window_.shouldClose()) {
      
    glfwPollEvents();

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();  
    imGUI_createUI(); 
    ImGui::Render();

    drawFrame();
  }

  vulkanState_.defaultLogicalDevice_->waitIdle();    
}

ImGUIBase::~ImGUIBase() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
