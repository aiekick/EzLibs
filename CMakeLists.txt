cmake_minimum_required(VERSION 3.1)
project(EzLibs)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

enable_testing()
add_definitions(-DTEST_ENABLED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/EzGL)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/EzGui)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/EzXml)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/EzIni)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/EzExpr)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/EzMath)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/EzGraph)
