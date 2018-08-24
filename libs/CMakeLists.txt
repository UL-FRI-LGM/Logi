cmake_minimum_required (VERSION 3.6)

# Vulkan
find_package(Vulkan REQUIRED)
add_library(vulkan UNKNOWN IMPORTED GLOBAL)
set_target_properties(vulkan PROPERTIES
	IMPORTED_LOCATION "${VULKAN_LIBRARY}"
	INTERFACE_INCLUDE_DIRECTORIES "${VULKAN_INCLUDE_DIR}")

# Spirv-cross
add_subdirectory(spirv-cross)


# VMA
add_subdirectory(vma)
