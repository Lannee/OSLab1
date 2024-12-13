cmake_minimum_required(VERSION 3.14)
project(oslab1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests EXCLUDE_FROM_ALL)
