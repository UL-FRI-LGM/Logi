cmake_minimum_required (VERSION 3.6)

file(GLOB_RECURSE SRC_LIST "src/*.cpp")
list(REMOVE_ITEM SRC_LIST "src/main.cpp")

add_library(logi STATIC ${SRC_LIST})
target_link_libraries(logi ${VULKAN_LIBRARY} spirv-cross)