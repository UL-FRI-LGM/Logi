cmake_minimum_required (VERSION 3.6)

include(${PROJECT_SOURCE_DIR}/cmake_modules/CompileShaders.cmake)

add_executable(example_hello_triangle main.cpp)
target_link_libraries(example_hello_triangle logi glfw)

compile_shaders(example_hello_triangle shaders)