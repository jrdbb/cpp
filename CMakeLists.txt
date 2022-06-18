# enable x.y version format
cmake_policy(SET CMP0048 NEW)
project(cpp VERSION 1.0)

cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*;--quiet")

enable_testing()

add_subdirectory(common)
add_subdirectory(externals)
add_subdirectory(standard)