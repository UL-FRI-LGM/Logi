#ifndef VULKAN_STATE
#define VULKAN_STATE

#include "logi/logi.hpp"

enum PipelineType {Graphics, Compute, Present};

class VulkanState 
{

 public:
    VulkanState() {}
    VulkanState(const logi::VulkanInstance& vulkanInstance) : instance_(vulkanInstance), physicalDevice_(nullptr), defaultLogicalDevice_(nullptr),
                                          defaultAllocator_(nullptr), defaultGraphicsCommandPool_(nullptr), defaultComputeCommandPool_(nullptr), 
                                             defaultPresentCommandPool_(nullptr), defaultGraphicsQueue_(nullptr), defaultComputeQueue_(nullptr),
                                                defaultPresentQueue_(nullptr) {}


    void addLogicalDevice(const std::string& key, logi::LogicalDevice& logicalDevice)
    {
       // Returns iterator if key is already present in map
       auto const result = logicalDevices_.insert(std::pair<std::string, logi::LogicalDevice>(key, logicalDevice));
       if(!result.second) {result.first->second = logicalDevice;} // Update if key already exists
    }
 
    void addAllocator(const std::string& key, logi::MemoryAllocator& allocator)
    {
       auto const result = allocators_.insert(std::pair<std::string, logi::MemoryAllocator>(key, allocator));
       if(!result.second) {result.first->second = allocator;}
    }
   
    void addQueueFamiliy(const std::string& key, logi::QueueFamily& queueFamiliy) 
    {
       auto const result = queueFamilies_.insert(std::pair<std::string, logi::QueueFamily>(key, queueFamiliy));
       if(!result.second) {result.first->second = queueFamiliy;}
    }

    void addQueue(const std::string& key, logi::Queue& queue) 
    {
       auto const result = queues_.insert(std::pair<std::string, logi::Queue>(key, queue));
       if(!result.second) {result.first->second = queue;}
    }
 
    void addCommandPool(const std::string& key, logi::CommandPool& commandPool) 
    {
       auto const result = commandPools_.insert(std::pair<std::string, logi::CommandPool>(key, commandPool));
       if(!result.second) {result.first->second = commandPool;}
    }
   
    void setInstance(const logi::VulkanInstance& instance) {instance_ = instance;}
    void setPhysicalDevice(const logi::PhysicalDevice& physicalDevice) {physicalDevice_ = physicalDevice;}
    void setDefaultLogicalDevice(const std::string& key) {defaultLogicalDevice_ = &logicalDevices_.at(key);}
    void setDefaultAllocator(const std::string& key) {defaultAllocator_ = &allocators_.at(key);}

    void setDefaultQueue(const std::string& key, const PipelineType& pipelineType) 
    {
       if(pipelineType == Graphics) {setDefaultGraphicsQueue(key);}
       else if(pipelineType == Compute) {setDefaultComputeQueue(key);}
       else {setDefaultPresentQueue(key);}
    }
    void setDefaultGraphicsQueue(const std::string& key) {defaultGraphicsQueue_ = &queues_.at(key);}
    void setDefaultComputeQueue(const std::string& key) {defaultComputeQueue_ = &queues_.at(key);}
    void setDefaultPresentQueue(const std::string& key) {defaultPresentQueue_ = &queues_.at(key);}    

   
    void setDefaultCommandPool(const std::string& key, const PipelineType& pipelineType) 
    {
       if(pipelineType == Graphics) {setDefaultGraphicsCommandPool(key);}
       else if(pipelineType == Compute) {setDefaultComputeCommandPool(key);}
       else {setDefaultPresentCommandPool(key);}
    }
    void setDefaultGraphicsCommandPool(const std::string& key) {defaultGraphicsCommandPool_ = &commandPools_.at(key);}
    void setDefaultComputeCommandPool(const std::string& key) {defaultComputeCommandPool_ = &commandPools_.at(key);}
    void setDefaultPresentCommandPool(const std::string& key) {defaultPresentCommandPool_ = &commandPools_.at(key);}

    void removeLogicalDevice(const std::string& key) {logicalDevices_.erase(key);}
    void removeAllocator(const std::string& key) {allocators_.erase(key);}
    void removeQueueFamily(const std::string& key) {queueFamilies_.erase(key);}
    void removeQueue(const std::string& key) {queues_.erase(key);}
    void removeCommandPool(const std::string& key) {commandPools_.erase(key);}


    logi::VulkanInstance instance_;
    logi::PhysicalDevice physicalDevice_;
    logi::QueueFamily graphicsFamily_;
    logi::QueueFamily computeFamily_;
    logi::QueueFamily presentFamily_;
    
    logi::LogicalDevice* defaultLogicalDevice_;
    logi::MemoryAllocator* defaultAllocator_;
    logi::CommandPool* defaultGraphicsCommandPool_;
    logi::CommandPool* defaultComputeCommandPool_;
    logi::CommandPool* defaultPresentCommandPool_;
    logi::Queue* defaultGraphicsQueue_;
    logi::Queue* defaultComputeQueue_;
    logi::Queue* defaultPresentQueue_;

 private:
    std::map<std::string, logi::LogicalDevice> logicalDevices_;
    std::map<std::string, logi::MemoryAllocator> allocators_;

    std::map<std::string, logi::QueueFamily> queueFamilies_;
    std::map<std::string, logi::Queue> queues_;
    std::map<std::string, logi::CommandPool> commandPools_;
};

#endif
