# GLFW3
if (NOT TARGET CppGLFW)
    add_subdirectory(libs/CppGLFW)
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