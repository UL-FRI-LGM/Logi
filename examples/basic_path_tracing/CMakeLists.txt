cmake_minimum_required (VERSION 3.6)

include(${PROJECT_SOURCE_DIR}/cmake_modules/CompileShaders.cmake)

add_executable(example_basic_pt main.cpp)
target_link_libraries(example_basic_pt logi glfw example_base glm)

set_property(TARGET example_basic_pt PROPERTY CXX_STANDARD 17)
set_property(TARGET example_basic_pt PROPERTY CXX_STANDARD_REQUIRED ON)


compile_shaders(example_basic_pt shaders)