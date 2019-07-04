cmake_minimum_required (VERSION 3.6)

file(GLOB SOURCES "src/*.cpp")

add_library(microws STATIC ${SOURCES})

target_include_directories(microws PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
