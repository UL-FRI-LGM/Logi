cmake_minimum_required (VERSION 3.6)

add_library(vma INTERFACE)
target_include_directories(vma INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:lib>
)