cmake_minimum_required (VERSION 3.6)

file(GLOB_RECURSE SRC_LIST "*.cpp")
list(REMOVE_ITEM SRC_LIST "main.cpp")

add_library(logi STATIC ${SRC_LIST})
target_link_libraries(logi vulkan spirv-cross vma json)

add_custom_command(TARGET logi POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        shaders
        $<TARGET_FILE_DIR:logi>/shaders)