aux_source_directory(. TEST_SRCS)

add_executable(cpp11_test ${TEST_SRCS})
target_link_libraries(cpp11_test gtest gmock)

set_target_properties(cpp11_test
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
)

include(GoogleTest)
gtest_discover_tests(cpp11_test)