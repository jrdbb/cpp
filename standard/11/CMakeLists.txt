project(cpp11)

set (CMAKE_CXX_STANDARD 11)


add_subdirectory(smart_pointer)
add_subdirectory(type_traits)
add_subdirectory(test)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE .)