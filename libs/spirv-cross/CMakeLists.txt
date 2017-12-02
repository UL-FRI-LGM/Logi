cmake_minimum_required (VERSION 3.6)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(spirv-cross STATIC ${SOURCES} ${HEADERS})

target_include_directories(spirv-cross PUBLIC .)

install(TARGETS spirv-cross DESTINATION lib)
install(FILES ${HEADERS} DESTINATION .)