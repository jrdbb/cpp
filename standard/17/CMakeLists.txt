project(cpp17)

set (CMAKE_CXX_STANDARD 17)


add_subdirectory(variant)
add_subdirectory(test)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE .)