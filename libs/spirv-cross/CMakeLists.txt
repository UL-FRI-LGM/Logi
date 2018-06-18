cmake_minimum_required (VERSION 3.6)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.hpp" "include/*.h")

add_library(spirv-cross STATIC ${SOURCES})
target_include_directories(spirv-cross PUBLIC
    "include"
)