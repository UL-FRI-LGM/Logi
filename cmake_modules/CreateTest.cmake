##############################################
#
# Create a CTest test from the given name,
# sources, includes and dependencies.
#
# Parameters:
# - TEST_NAME       Test name.
# - SOURCES         List of test sources.
# - INCLUDES        List of test includes.
# - DEPENDENCIES    List of test dependencies.
#
##############################################
macro(create_test TEST_NAME SOURCES INCLUDES DEPENDENCIES)
    add_executable(${TEST_NAME} ${SOURCES})
    target_include_directories(${TEST_NAME} PRIVATE
            "$<BUILD_INTERFACE:${INCLUDES}>"
            $<INSTALL_INTERFACE:include>
            )
    target_link_libraries(${TEST_NAME} ${DEPENDENCIES} gtest gmock gtest_main)

    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/testdata)

        add_custom_command(TARGET ${TEST_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${TEST_NAME}>/testdata"
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/testdata
                $<TARGET_FILE_DIR:${TEST_NAME}>/testdata
                )
    endif ()

    add_test(NAME ${TEST_NAME}
            COMMAND ${TEST_NAME}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    set_property(TARGET ${TEST_NAME} PROPERTY CXX_STANDARD 17)
    set_property(TARGET ${TEST_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

endmacro()