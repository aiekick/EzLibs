cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0110 OLD) ## for use [] in test names like "EzMath.double.Vec2.[]"

set(PROJECT TestEzMath)
enable_language(C CXX)
project(${PROJECT} CXX)

message("EzMath testing enabled")

file(GLOB_RECURSE PROJECT_TEST_SRC_RECURSE 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/*.h 
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_TEST_SRC_RECURSE})

include_directories(${EzMath_INCLUDE_DIR})
add_executable(${PROJECT} ${PROJECT_TEST_SRC_RECURSE})
target_link_libraries(${PROJECT} ${EzMath_LIBRARIES})
set_target_properties(${PROJECT} PROPERTIES FOLDER Tests)

target_include_directories(${PROJECT} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/.. 
)

function(AddTest arg)
  add_test("${arg}" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}" "${arg}")
endfunction()

##########################################################
##### EzMath #############################################
##########################################################

AddTest("TestEzMathRound<float>")
AddTest("TestEzMathRound<double>")
AddTest("TestEzMathFloor<float>")
AddTest("TestEzMathFloor<double>")
AddTest("TestEzMathCeil<float>")
AddTest("TestEzMathCeil<double>")
AddTest("TestEzMathFract<float>")
AddTest("TestEzMathFract<double>")
AddTest("TestEzMathCos<float>")
AddTest("TestEzMathCos<double>")
AddTest("TestEzMathAcos<float>")
AddTest("TestEzMathAcos<double>")
AddTest("TestEzMathSin<float>")
AddTest("TestEzMathSin<double>")
AddTest("TestEzMathAsin<float>")
AddTest("TestEzMathAsin<double>")
AddTest("TestEzMathTan<float>")
AddTest("TestEzMathTan<double>")
AddTest("TestEzMathAtan<float>")
AddTest("TestEzMathAtan<double>")
AddTest("TestEzMathMini<int32_t>")
AddTest("TestEzMathMini<uint32_t>")
AddTest("TestEzMathMini<float>")
AddTest("TestEzMathMini<double>")
AddTest("TestEzMathMaxi<int32_t>")
AddTest("TestEzMathMaxi<uint32_t>")
AddTest("TestEzMathMaxi<float>")
AddTest("TestEzMathMaxi<double>")
AddTest("TestEzMathClamp<int32_t>")
AddTest("TestEzMathClamp<uint32_t>")
AddTest("TestEzMathClamp<float>")
AddTest("TestEzMathClamp<double>")
AddTest("TestEzMathAbs<int32_t>")
AddTest("TestEzMathAbs<float>")
AddTest("TestEzMathAbs<double>")
AddTest("TestEzMathSign<int32_t>")
AddTest("TestEzMathSign<float>")
AddTest("TestEzMathSign<double>")
AddTest("TestEzMathStep<float>")
AddTest("TestEzMathStep<double>")
AddTest("TestEzMathMod<float>")
AddTest("TestEzMathMod<double>")
AddTest("TestEzMathInvMix<float>")
AddTest("TestEzMathInvMix<double>")
AddTest("TestEzMathLerp<float>")
AddTest("TestEzMathLerp<double>")
AddTest("TestEzMathEerp<float>")
AddTest("TestEzMathEerp<double>")
AddTest("TestEzMathMix<float>")
AddTest("TestEzMathMix<double>")
AddTest("TestEzMathRoundN")
AddTest("TestEzMathFloatIsValid")
AddTest("TestEzMathIsFloatDifferent")
AddTest("TestEzMathIsFloatEqual")
