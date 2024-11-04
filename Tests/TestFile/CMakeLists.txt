cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0110 OLD) ## for use [] in test names like "EzTools.double.Vec2.[]"

set(PROJECT TestFile)
enable_language(C CXX)
project(${PROJECT} CXX)

file(GLOB_RECURSE PROJECT_TEST_SRC_RECURSE 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/*.h)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX Src FILES ${PROJECT_TEST_SRC_RECURSE})

file(GLOB SRC_RECURSE 
	${EZ_LIBS_INCLUDE_DIR}/EzLibs/EzFile.hpp
	${EZ_LIBS_INCLUDE_DIR}/EzLibs/EzIni.hpp
	${EZ_LIBS_INCLUDE_DIR}/EzLibs/EzLog.hpp
	${EZ_LIBS_INCLUDE_DIR}/EzLibs/EzXmlConfig.hpp
	${EZ_LIBS_INCLUDE_DIR}/EzLibs/EzBmp.hpp
	${EZ_LIBS_INCLUDE_DIR}/EzLibs/EzGif.hpp
	${EZ_LIBS_INCLUDE_DIR}/EzLibs/EzPng.hpp)
source_group(TREE ${EZ_LIBS_INCLUDE_DIR}/EzLibs PREFIX Libs FILES ${SRC_RECURSE})

add_executable(${PROJECT} 
	${SRC_RECURSE}
	${PROJECT_TEST_SRC_RECURSE}
)

set_target_properties(${PROJECT} PROPERTIES FOLDER Tests)

target_include_directories(${PROJECT} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}
)

##########################################################
##### TESTS ##############################################
##########################################################

##########################################################
##### TESTS EzBmp ########################################
##########################################################

AddTest("TestEzBmp_Save_0")
AddTest("TestEzBmp_Save_1")

##########################################################
##### TESTS EzGif ########################################
##########################################################

AddTest("TestEzGif_Save_0")
AddTest("TestEzGif_Save_1")

##########################################################
##### TESTS EzPng ########################################
##########################################################

AddTest("TestEzPng_Save_0")
AddTest("TestEzPng_Save_1")
