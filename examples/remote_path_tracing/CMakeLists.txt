cmake_minimum_required (VERSION 3.6)

include(${PROJECT_SOURCE_DIR}/cmake_modules/CompileShaders.cmake)

add_executable(example_remote_pt main.cpp)
target_link_libraries(example_remote_pt logi glfw glm)

set_property(TARGET example_remote_pt PROPERTY CXX_STANDARD 17)
set_property(TARGET example_remote_pt PROPERTY CXX_STANDARD_REQUIRED ON)


compile_shaders(example_remote_pt shaders)