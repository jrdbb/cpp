# enable x.y version format
cmake_policy(SET CMP0048 NEW)
project(cpp VERSION 1.0)

cmake_minimum_required(VERSION 3.16)

enable_testing()

add_subdirectory(externals)
add_subdirectory(standard)