cmake_minimum_required(VERSION 3.1)

option(TESTING_EZ_EXPR "Enable testing of EzExpr" OFF)
option(TESTING_EZ_GL "Enable testing of EzGL" OFF)
option(TESTING_EZ_TOOLS "Enable testing of EzTools" OFF)

if (TESTING_EZ_EXPR)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TestEzExpr)
endif()

if (TESTING_EZ_GL)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TestEzGL)
endif()

if (TESTING_EZ_TOOLS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TestEzTools)
endif()
