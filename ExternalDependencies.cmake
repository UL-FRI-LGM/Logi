# Vulkan
if (NOT TARGET Vulkan::Vulkan)
	find_package(Vulkan REQUIRED)
else()
	message(STATUS "[Logi] Target Vulkan::Vulkan is already defined. Using existing target.")
endif()

# Spirv-cross
if (NOT TARGET spirv-cross)
	add_subdirectory(libs/spirv-cross)
else()
	message(STATUS "[Logi] Target spirv-cross is already defined. Using existing target.")
endif()

# VMA
if (NOT TARGET vma)
	add_subdirectory(libs/vma)
else()
	message(STATUS "[Logi] Target vma is already defined. Using existing target.")
endif()