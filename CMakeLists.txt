cmake_minimum_required (VERSION 3.6)

# Metadata
project(VulkanEngine)

# C++ 17 SUPPORT
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# C++ Google Test
set(BUILD_TESTS ON)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

option(BUILD_DOC "Build documentation" ON)

##########################################################
##################### CMAKE MODULES ######################
##########################################################

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

##########################################################
####################### LIBRARIES ########################
##########################################################

# Libraries path
set(LIB_DIR ${PROJECT_SOURCE_DIR}/libs)

# GLFW Library
#set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#add_subdirectory(${LIB_DIR}/glfw)
#link_libraries(glfw ${GLFW_LIBRARIES})

# GLM
include_directories(${LIB_DIR}/glm)

# Vulkan
find_package(Vulkan REQUIRED)
include_directories(${VULKAN_INCLUDE_DIR})

# Json
include_directories(${LIB_DIR}/json)

# VMA - Vulkan Memory Allocator
include_directories(${LIB_DIR}/vma)

include_directories(${LIB_DIR}/spirv-cross)
add_subdirectory(${LIB_DIR}/spirv-cross)

##########################################################
####################### SOURCES ##########################
##########################################################

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_LIST "src/*.cpp")
list(REMOVE_ITEM SRC_LIST "src/main.cpp")

# Build VKR library
add_library(vkr STATIC ${SRC_LIST})
target_link_libraries(vkr ${VULKAN_LIBRARY} spirv-cross)

add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} ${VULKAN_LIBRARY} vkr)

##########################################################
###################### RESOURCES #########################
##########################################################

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)


##########################################################
####################### DOXYGEN ##########################
##########################################################

if (BUILD_DOC)
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
endif (BUILD_DOC)

##########################################################
####################### DOXYGEN ##########################
##########################################################
if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif (BUILD_TESTS)