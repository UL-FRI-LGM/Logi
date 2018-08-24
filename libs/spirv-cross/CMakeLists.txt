cmake_minimum_required (VERSION 3.6)

file(GLOB SRC "src/*.cpp")

add_library(spirv-cross STATIC ${SRC})
target_include_directories(spirv-cross PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:lib>
)