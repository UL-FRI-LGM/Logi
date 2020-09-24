#ifndef IMGUI_BASE_H
#define IMGUI_BASE_H

#include "example_base.h"
#include "utility.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

// ImGUI vulkan error handling
static void checkVkResult(VkResult err) {
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

class ImGUIBase : public ExampleBase {
 protected:
  void imGUI_initOverlay();

  void imGUI_createUI() override;

  ~ImGUIBase();
  
 private:
  void imGUI_createDescriptorPool();

  void imGUI_createRenderPass();

  void imGUI_createCommandBuffers();

  void imGUI_createFrameBuffers();

  void recreateSwapChain() override;

  logi::CommandBuffer* imGUI_createOverlay(const uint32_t& i) override;

  void mainLoop() override;


  ImGuiIO io_;

  logi::CommandPool imGUI_commandPool_;
  std::vector<logi::CommandBuffer> imGUI_commandBuffers_;

  std::vector<logi::Framebuffer> imGUI_framebuffers_;

  logi::DescriptorPool imGUI_descriptorPool_;
  logi::RenderPass imGUI_renderPass_;
};

#endif