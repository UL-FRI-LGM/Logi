cmake_minimum_required (VERSION 3.6)

# Vulkan
find_package(Vulkan REQUIRED)
add_library(vulkan UNKNOWN IMPORTED GLOBAL)
set_target_properties(vulkan PROPERTIES
	IMPORTED_LOCATION "${VULKAN_LIBRARY}"
	INTERFACE_INCLUDE_DIRECTORIES "${VULKAN_INCLUDE_DIR}")

# Spirv-cross
add_subdirectory(spirv-cross)

# GLM
add_library(glm INTERFACE)
target_include_directories(glm
INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glm>
    $<INSTALL_INTERFACE:/glm>
)

# Json
add_library(json INTERFACE)
target_include_directories(json
INTERFACE
        $<INSTALL_INTERFACE:/json>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/json>
)

# VMA
add_library(vma INTERFACE)
target_include_directories(vma
INTERFACE
        $<INSTALL_INTERFACE:/vma>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vma>
)

# GLFW3
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)