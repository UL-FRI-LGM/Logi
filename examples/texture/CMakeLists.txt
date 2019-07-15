cmake_minimum_required(VERSION 3.6)

set(TARGET_NAME "texture")

include(${PROJECT_SOURCE_DIR}/cmake_modules/CompileShaders.cmake)

file(GLOB_RECURSE SRC_LIST "src/*.cpp")

add_executable(${TARGET_NAME} ${SRC_LIST})
target_link_libraries(${TARGET_NAME} example_base)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(${TARGET_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

compile_shaders(${TARGET_NAME} shaders)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})