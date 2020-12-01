#include "ray_tracing.h"

int main(int argc, char* argv[]) {

  // Enable ray tracing 
  ExampleConfiguration config;
  config.instanceExtensions = {"VK_KHR_get_physical_device_properties2"};
  // config.deviceExtensions = {"VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME", "VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME", 
  //                            "VK_KHR_MAINTENANCE3_EXTENSION_NAME", "VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME", "VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME",
  //                            "VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME"};
  config.deviceExtensions = {"VK_NV_ray_tracing", "VK_KHR_maintenance3", "VK_KHR_get_memory_requirements2"};

  RayTracing example(config);
  example.run();
}
