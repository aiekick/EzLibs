cmake_minimum_required(VERSION 3.1)
project(EzLibs)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

set(EzLibs)

enable_testing()
add_definitions(-DTEST_ENABLED)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Tests)
