cmake_minimum_required(VERSION 2.8.2)
include(CTest)

# Build Google test framework
set( gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" )
add_subdirectory(./libs/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE BASE_SRC_LIST "base/*.cpp")

add_executable(base_test ${BASE_SRC_LIST})
target_link_libraries(base_test gtest gtest_main ${VULKAN_LIBRARY} spirv-cross vkr)
add_test(NAME BaseTest COMMAND base_test)