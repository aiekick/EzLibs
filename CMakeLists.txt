cmake_minimum_required(VERSION 3.1)
project(EzLibs)

option(USE_EZ_LIBS_TESTING "Enable testing of EzLibs" OFF)

set(CMAKE_CXX_STANDARD 11)

set(EZ_LIBS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# if there is a parent project
if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	set(EZ_LIBS_INCLUDE_DIR ${EZ_LIBS_INCLUDE_DIR} PARENT_SCOPE)
endif()

include_directories(${EZ_LIBS_INCLUDE_DIR})

if(USE_EZ_LIBS_TESTING)
	enable_testing()
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Tests)
endif()
