#include "utility.h"

namespace utility {
    ShaderReflection::ShaderReflection(logi::ShaderModule vertexShader,
                                    logi::ShaderModule fragmentShader,
                                    std::vector<logi::DescriptorSetReflectionInfo> descriptorSetInfo,
                                    std::vector<logi::PushConstantReflectionInfo> pushConstants)
            : vertexShader(std::move(vertexShader)), fragmentShader(std::move(fragmentShader)),
                    descriptorSetInfo(std::move(descriptorSetInfo)), pushConstants(std::move(pushConstants)) {}


    PipelineLayoutData::PipelineLayoutData(logi::PipelineLayout layout,
                                    std::vector<logi::DescriptorSetLayout> descriptorSetLayouts)
                : layout(std::move(layout)), descriptorSetLayouts(std::move(descriptorSetLayouts)) {}


    logi::ShaderModule createShaderModule(const VulkanState& vulkanState, const std::string& shaderPath)
    {   
        
        assert(vulkanState.defaultLogicalDevice_ != nullptr && "Default logical device not initialized!");

        std::ifstream file(shaderPath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> code(fileSize);

        file.seekg(0);
        file.read(code.data(), fileSize);

        file.close();

        vk::ShaderModuleCreateInfo createInfo;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        return vulkanState.defaultLogicalDevice_->createShaderModule(createInfo);
    }

    ShaderReflection loadShaders(const VulkanState& vulkanState, 
                                 const std::string& vertexPath, const std::string& fragmentPath)
    {   
        assert(vulkanState.defaultLogicalDevice_ != nullptr && "Default logical device not initialized!");

        ShaderReflection shaderReflection;
        
        shaderReflection.vertexShader = createShaderModule(vulkanState, vertexPath);
        shaderReflection.fragmentShader = createShaderModule(vulkanState, fragmentPath);

        // Generate descriptor set layouts
        shaderReflection.descriptorSetInfo = 
                logi::reflectDescriptorSets({{shaderReflection.vertexShader, "main"}, {shaderReflection.fragmentShader, "main"}});
        
        // Generate push constants ranges
        shaderReflection.pushConstants = 
                logi::reflectPushConstants({{shaderReflection.vertexShader, "main"}, {shaderReflection.fragmentShader, "main"}});

        return shaderReflection;
    }
    
    
    PipelineLayoutData createPipelineLayout(const VulkanState& vulkanState, const ShaderReflection& shaderReflection)
    {
        PipelineLayoutData pipelineLayoutData;

        pipelineLayoutData.descriptorSetLayouts.reserve(shaderReflection.descriptorSetInfo.size());
            for (const auto& info : shaderReflection.descriptorSetInfo) {
            // Generate binding infos.
            std::vector<vk::DescriptorSetLayoutBinding> bindings(info.bindings.begin(), info.bindings.end());

            vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutInfo;
            descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            descriptorSetLayoutInfo.pBindings = bindings.data();

            pipelineLayoutData.descriptorSetLayouts.emplace_back(
            vulkanState.defaultLogicalDevice_->createDescriptorSetLayout(descriptorSetLayoutInfo));
        }
        
        // Transform to Vulkan namespace
        std::vector<vk::PushConstantRange> pushConstantRanges(shaderReflection.pushConstants.begin(), shaderReflection.pushConstants.end());
        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(pipelineLayoutData.descriptorSetLayouts.begin(),
                                                                        pipelineLayoutData.descriptorSetLayouts.end());

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(pipelineLayoutData.descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
        pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();

        pipelineLayoutData.layout = vulkanState.defaultLogicalDevice_->createPipelineLayout(pipelineLayoutInfo);

        return pipelineLayoutData;   
    }

} // namespace utility