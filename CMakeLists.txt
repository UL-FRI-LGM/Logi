cmake_minimum_required (VERSION 3.6)
cmake_policy(SET CMP0048 NEW)

# Metadata
project(Logi VERSION 0.1.0 LANGUAGES CXX)

# C++ 17 SUPPORT
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add path to CMake custom modules.
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/cmake_modules")

# C++ Google Test
option(BUILD_TESTS "Build tests." ON)
option(BUILD_DOC "Build documentation" ON)
option(BUILD_EXAMPLES "Build examples" ON)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

##############################################
# BUILD LOGI LIBRARY
############################################## 

# Libraries path
add_subdirectory(${PROJECT_SOURCE_DIR}/libs)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

if (BUILD_EXAMPLES)
	add_subdirectory(examples)
endif (BUILD_EXAMPLES)

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

##############################################
# TESTS
##############################################

if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif (BUILD_TESTS)