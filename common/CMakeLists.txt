project(cpp_common)

set (CMAKE_CXX_STANDARD 20)

add_subdirectory(inheritance)
add_subdirectory(templates)
add_subdirectory(test)
add_subdirectory(container)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE .)