cmake_minimum_required(VERSION 3.1)

set(PROJECT TestEzGL)
enable_language(C CXX)
project(${PROJECT} CXX)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
  find_package(X11 REQUIRED)
  if (NOT X11_Xi_FOUND)
    message(FATAL_ERROR "X11 Xi library is required")
  endif ()
endif ()

file(GLOB_RECURSE PROJECT_TEST_SRC_RECURSE 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/*.h 
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_TEST_SRC_RECURSE})

add_executable(${PROJECT} ${PROJECT_TEST_SRC_RECURSE})

if(NOT GLAD_LIBRARIES)
	set(GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad/include)
	file(GLOB GLAD_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad/src/glad.c)
	file(GLOB GLAD_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad/glad.h)
	add_library(glad STATIC ${GLAD_HEADERS} ${GLAD_SOURCES})
	set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)
	target_include_directories(glad PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad/include
	)
	if(WIN32)
		set(GLAD_LIBRARIES glad)
	else()
		set(GLAD_LIBRARIES glad dl)
	endif()
endif() 

target_link_libraries(${PROJECT} PRIVATE ${GLAD_LIBRARIES})

target_include_directories(${PROJECT} PRIVATE 
	${GLAD_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad/include
)

set_target_properties(${PROJECT} PROPERTIES FOLDER Tests)

target_compile_definitions(${PROJECT} PRIVATE 
	-DOPENGL_LOADER="${GLAD_INCLUDE_DIR}/glad/glad.h"
)

#############################################################
#############################################################
#############################################################

function(SetTest arg)
  add_test("${arg}" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}" "${arg}")
endfunction()

##########################################################
##########################################################

## ez::gl::Uniforms
SetTest("Test_EzGL_Uniforms_Parsing_Good_Syntax_0")
SetTest("Test_EzGL_Uniforms_Parsing_Good_Syntax_1")
SetTest("Test_EzGL_Uniforms_Parsing_Good_Syntax_2")
SetTest("Test_EzGL_Uniforms_Parsing_Good_Syntax_3")
SetTest("Test_EzGL_Uniforms_Parsing_Good_Syntax_4")
SetTest("Test_EzGL_Uniforms_Parsing_Good_Syntax_5")
SetTest("Test_EzGL_Uniforms_Parsing_Good_Syntax_6")
SetTest("Test_EzGL_Uniforms_Parsing_Good_Syntax_7")
SetTest("Test_EzGL_Uniforms_Parsing_Good_Syntax_8")
SetTest("Test_EzGL_Uniforms_Parsing_Good_Syntax_9")
SetTest("Test_EzGL_Uniforms_Parsing_Good_Syntax_10")
SetTest("Test_EzGL_Uniforms_Parsing_Good_Syntax_11")
SetTest("Test_EzGL_Uniforms_Parsing_Bad_Syntax_0")
SetTest("Test_EzGL_Uniforms_Parsing_Bad_Syntax_1")
SetTest("Test_EzGL_Uniforms_Parsing_Bad_Syntax_2")
SetTest("Test_EzGL_Uniforms_Parsing_Bad_Syntax_3")
SetTest("Test_EzGL_Uniforms_Parsing_Bad_Syntax_4")
SetTest("Test_EzGL_Uniforms_Parsing_Bad_Syntax_5")
SetTest("Test_EzGL_Uniforms_Parsing_Bad_Syntax_6")
SetTest("Test_EzGL_Uniforms_Parsing_Bad_Syntax_7")
SetTest("Test_EzGL_Uniforms_Parsing_Time_Widget_0")
SetTest("Test_EzGL_Uniforms_Parsing_Buffer_Widget_0")
SetTest("Test_EzGL_Uniforms_Parsing_Slider_Widget_0")
