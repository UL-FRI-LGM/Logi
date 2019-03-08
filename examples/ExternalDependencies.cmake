# GLFW3
if (NOT TARGET glfw)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	add_subdirectory(libs/glfw)
else()
	message(STATUS "[Logi] Target glfw is already defined. Using existing target.")
endif()

# GLM
if (NOT TARGET glm)
	set(GLM_TEST_ENABLE OFF CACHE BOOL "Build unit tests" FORCE)
	add_subdirectory(libs/glm)
else()
	message(STATUS "[Logi] Target glm is already defined. Using existing target.")
endif()