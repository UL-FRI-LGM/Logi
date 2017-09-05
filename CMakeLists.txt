cmake_minimum_required (VERSION 3.6)

project(VulkanEngine)

option(BUILD_DOC "Build documentation" OFF)

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
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${LIB_DIR}/glfw)
link_libraries(glfw ${GLFW_LIBRARIES})

# GLM
include_directories(${LIB_DIR}/glm)

# Vulkan
find_package(Vulkan REQUIRED)
include_directories(${VULKAN_INCLUDE_DIR})

# VMA - Vulkan Memory Allocator
include_directories(${LIB_DIR}/vma)

##########################################################
####################### SOURCES ##########################
##########################################################

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SRC_LIST "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_LIST} ${RES_DIR})
target_link_libraries(${PROJECT_NAME} ${VULKAN_LIBRARY} glfw)

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

		file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc_doxygen)

		# Note the option ALL which allows to build the docs together with the application
		add_custom_target(doc_doxygen ALL
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM)
	else (DOXYGEN_FOUND)
	  message("Doxygen need to be installed to generate the doxygen documentation")
	endif (DOXYGEN_FOUND)
endif (BUILD_DOC)