cmake_minimum_required(VERSION 3.6)

file(GLOB_RECURSE SRC_LIST "src/*.cpp")

add_library(example_base STATIC ${SRC_LIST})
target_link_libraries(example_base logi CppGLFW glm)
set_property(TARGET example_base PROPERTY CXX_STANDARD 17)
set_property(TARGET example_base PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(example_base PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )