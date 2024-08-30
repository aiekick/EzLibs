cmake_minimum_required(VERSION 3.1)
project(EzExpr)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

enable_testing()
add_definitions(-DTEST_ENABLED)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_include_directories(EzExpr_Test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

