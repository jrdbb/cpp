project(cpp_common_template)

aux_source_directory(. SRCS)

add_library(${PROJECT_NAME} ${SRCS})