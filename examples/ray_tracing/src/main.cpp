#include "ray_tracing.h"

int main(int argc, char* argv[]) {

  // Enable ray tracing 
  ExampleConfiguration config;
  config.instanceExtensions = {"VK_KHR_get_physical_device_properties2"};
  // config.deviceExtensions = {"VK_KHR_maintenance3", "VK_EXT_descriptor_indexing", 
  //                            "VK_KHR_pipeline_library", "VK_KHR_deferred_host_operations", "VK_KHR_buffer_device_address",
  //                            "VK_KHR_acceleration_structure", "VK_KHR_ray_tracing_pipeline"};
  config.deviceExtensions = {"VK_NV_ray_tracing", "VK_KHR_maintenance3", "VK_KHR_get_memory_requirements2"};

  RayTracing example(config);
  example.run();
}
