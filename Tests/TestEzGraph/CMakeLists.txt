cmake_minimum_required(VERSION 3.1)

set(PROJECT TestEzGraph)
enable_language(C CXX)
project(${PROJECT} CXX)

file(GLOB_RECURSE PROJECT_TEST_SRC_RECURSE 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/*.h 
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_TEST_SRC_RECURSE})

add_executable(${PROJECT} ${PROJECT_TEST_SRC_RECURSE})

set_target_properties(${PROJECT} PROPERTIES FOLDER Tests)

#############################################################
#############################################################
#############################################################

target_include_directories(${PROJECT} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/.. 
)

add_definitions(-DTEST_SAMPLES_PATH="${CMAKE_SOURCE_DIR}/test_samples/")

function(SetTest arg)
	add_test("${arg}" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}" "${arg}")
endfunction()

##########################################################
## EzGraph:: #############################################
##########################################################

## Test_Graph
SetTest(Test_Graph_Building_0)
SetTest(Test_Graph_Evaluation_0)
