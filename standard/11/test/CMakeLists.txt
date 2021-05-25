project(cpp11_test)

aux_source_directory(. SRCS)

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} gtest gmock cpp11)

set_target_properties(${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})