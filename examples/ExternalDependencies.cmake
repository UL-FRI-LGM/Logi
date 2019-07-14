# GLFW3
if (NOT TARGET cpp-glfw)
    add_subdirectory(libs/cpp-glfw)
else ()
    message(STATUS "[Logi] Target glfw is already defined. Using existing target.")
endif ()

# GLM
if (NOT TARGET glm)
    set(GLM_TEST_ENABLE OFF CACHE BOOL "Build unit tests" FORCE)
    add_subdirectory(libs/glm)
else ()
    message(STATUS "[Logi] Target glm is already defined. Using existing target.")
endif ()