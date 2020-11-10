cmake_minimum_required(VERSION 3.10)

# Metadata
project(Logi VERSION 0.1.0 LANGUAGES CXX)

# Add path to CMake custom modules.
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")


# C++ Google Test
option(LOGI_BUILD_TESTS "Build tests." ON)
option(LOGI_BUILD_DOC "Build documentation" ON)
option(LOGI_BUILD_EXAMPLES "Build examples" ON)

##############################################
# BUILD LOGI LIBRARY
##############################################

#add_definitions(-DVK_USE_PLATFORM_WIN32_KHR -DVK_USE_PLATFORM_XCB_KHR -DVK_USE_PLATFORM_XLIB_KHR -DVK_USE_PLATFORM_WAYLAND_KHR)

# Libraries path
include(ExternalDependencies.cmake)

file(GLOB_RECURSE SRC_LIST "src/*.cpp")

add_library(logi STATIC ${SRC_LIST})
set_property(TARGET logi PROPERTY CXX_STANDARD 17)
set_property(TARGET logi PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(logi
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

target_link_libraries(logi Vulkan::Vulkan spirv-cross-core vma)

# TEST -> before did not work
if (LOGI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif (LOGI_BUILD_EXAMPLES)

##########################################################
####################### DOXYGEN ##########################
##########################################################

if (LOGI_BUILD_DOC)
    # Check if Doxygen is installed
    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        # Set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        # Request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # Note the option ALL which allows to build the docs together with the application
        add_custom_target(doc_doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM)
    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif (LOGI_BUILD_DOC)

##############################################
# TESTS
##############################################

if (LOGI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif (LOGI_BUILD_TESTS)
