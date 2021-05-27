project(cpp_common_container)

aux_source_directory(. SRCS)

add_library(${PROJECT_NAME} ${SRCS})