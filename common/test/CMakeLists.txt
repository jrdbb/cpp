project(cpp_common_test)

aux_source_directory(. TEST_SRCS)

add_executable(${PROJECT_NAME} ${TEST_SRCS})
target_link_libraries(${PROJECT_NAME} 
    cpp_common
    cpp_common_inheritance
    gmock
    gtest
    )

set_target_properties(${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})