cmake_minimum_required (VERSION 3.6)

# GLFW Library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
link_libraries(glfw ${GLFW_LIBRARIES})

# GLM
include_directories(glm)

# Json
include_directories(json)

# VMA - Vulkan Memory Allocator
include_directories(vma)

include_directories(spirv-cross)
add_subdirectory(spirv-cross)