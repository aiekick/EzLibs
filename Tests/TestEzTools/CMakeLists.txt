cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0110 OLD) ## for use [] in test names like "EzTools.double.Vec2.[]"

set(PROJECT TestEzTools)
enable_language(C CXX)
project(${PROJECT} CXX)
message("EzTools testing enabled")

option(USE_CODE_COVERAGE "Enable code coverage reporting" OFF)

if (USE_CODE_COVERAGE)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		set(COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate")
	elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		add_compile_options(/DEBUG:FULL /COVERAGE)
	endif()
endif()

file(GLOB_RECURSE PROJECT_TEST_SRC_RECURSE
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/*.h 
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX tests FILES ${PROJECT_TEST_SRC_RECURSE})

file(GLOB_RECURSE PROJECT_EZ_TOOLS_SRC_RECURSE
	${CMAKE_CURRENT_SOURCE_DIR}/../../EzTools/*.hpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../../EzTools PREFIX src FILES ${PROJECT_EZ_TOOLS_SRC_RECURSE})

add_executable(${PROJECT} 
    ${PROJECT_TEST_SRC_RECURSE} 
    ${PROJECT_EZ_TOOLS_SRC_RECURSE}
)

set_target_properties(${PROJECT} PROPERTIES FOLDER Tests)

target_include_directories(${PROJECT} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/.. 
)

##########################################################
##########################################################
##########################################################

if (USE_CODE_COVERAGE)
    add_custom_target(coverage
            COMMAND ${CMAKE_CTEST_COMMAND} # Lance les tests
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running tests for code coverage"
    )
    set_target_properties(coverage PROPERTIES FOLDER Tests)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # Cible pour générer un rapport de couverture pour GCC/Clang
        add_custom_command(TARGET coverage POST_BUILD
            COMMAND gcovr -r . --html --html-details -o ${CMAKE_BINARY_DIR}/coverage.html
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating coverage report for GCC/Clang"
        )
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        # Cible pour générer un rapport de couverture pour MSVC
        add_custom_command(TARGET coverage POST_BUILD
            COMMAND "${CMAKE_CMAKE_COMMAND}" -E env "PATH=%VSINSTALLDIR%\\Team Tools\\Dynamic Code Coverage Tools" CodeCoverage.exe collect /output:${CMAKE_BINARY_DIR}/coverage.coverage <path_to_your_test_executable>
            COMMAND "${CMAKE_CMAKE_COMMAND}" -E env "PATH=%VSINSTALLDIR%\\Team Tools\\Dynamic Code Coverage Tools" CodeCoverage.exe analyze /output:${CMAKE_BINARY_DIR}/coverage.xml ${CMAKE_BINARY_DIR}/coverage.coverage
            COMMENT "Generating coverage report for MSVC"
        )
    endif()
endif()

##########################################################
##### TESTS ##############################################
##########################################################

function(AddTest arg)
  add_test("${arg}" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}" "${arg}")
endfunction()

##########################################################
##### EzTools ############################################
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

AddTest("TestEzMathMini<float>")
AddTest("TestEzMathMini<double>")
AddTest("TestEzMathMini<int32_t>")
AddTest("TestEzMathMini<uint32_t>")
AddTest("TestEzMathMini<int64_t>")
AddTest("TestEzMathMini<uint64_t>")

AddTest("TestEzMathMaxi<float>")
AddTest("TestEzMathMaxi<double>")
AddTest("TestEzMathMaxi<int32_t>")
AddTest("TestEzMathMaxi<uint32_t>")
AddTest("TestEzMathMaxi<int64_t>")
AddTest("TestEzMathMaxi<uint64_t>")

AddTest("TestEzMathClamp<float>")
AddTest("TestEzMathClamp<double>")
AddTest("TestEzMathClamp<int32_t>")
AddTest("TestEzMathClamp<uint32_t>")
AddTest("TestEzMathClamp<int64_t>")
AddTest("TestEzMathClamp<uint64_t>")

AddTest("TestEzMathAbs<float>")
AddTest("TestEzMathAbs<double>")
AddTest("TestEzMathAbs<int32_t>")
AddTest("TestEzMathAbs<int64_t>")

AddTest("TestEzMathSign<float>")
AddTest("TestEzMathSign<double>")
AddTest("TestEzMathSign<int32_t>")
AddTest("TestEzMathSign<int64_t>")

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

AddTest("TestEzMathIsDifferent<float>")
AddTest("TestEzMathIsDifferent<double>")
AddTest("TestEzMathIsDifferent<int32_t>")
AddTest("TestEzMathIsDifferent<uint32_t>")
AddTest("TestEzMathIsDifferent<int64_t>")
AddTest("TestEzMathIsDifferent<uint64_t>")

AddTest("TestEzMathIsEqual<float>")
AddTest("TestEzMathIsEqual<double>")
AddTest("TestEzMathIsEqual<int32_t>")
AddTest("TestEzMathIsEqual<uint32_t>")
AddTest("TestEzMathIsEqual<int64_t>")
AddTest("TestEzMathIsEqual<uint64_t>")

##########################################################
##### EzVec2 #############################################
##########################################################

AddTest("TestEzVec2Offset<float>")
AddTest("TestEzVec2Offset<double>")
AddTest("TestEzVec2Offset<int32_t>")
AddTest("TestEzVec2Offset<uint32_t>")
AddTest("TestEzVec2Offset<int64_t>")
AddTest("TestEzVec2Offset<uint64_t>")

AddTest("TestEzVec2Negate<float>")
AddTest("TestEzVec2Negate<double>")
AddTest("TestEzVec2Negate<int32_t>")
AddTest("TestEzVec2Negate<int64_t>")

AddTest("TestEzVec2Length<float>")
AddTest("TestEzVec2Length<double>")

AddTest("TestEzVec2Normalize<float>")
AddTest("TestEzVec2Normalize<double>")

AddTest("TestEzVec2Sum<float>")
AddTest("TestEzVec2Sum<double>")
AddTest("TestEzVec2Sum<int32_t>")
AddTest("TestEzVec2Sum<uint32_t>")
AddTest("TestEzVec2Sum<int64_t>")
AddTest("TestEzVec2Sum<uint64_t>")

AddTest("TestEzVec2OperatorAdd<float>")
AddTest("TestEzVec2OperatorAdd<double>")
AddTest("TestEzVec2OperatorAdd<int32_t>")
AddTest("TestEzVec2OperatorAdd<uint32_t>")
AddTest("TestEzVec2OperatorAdd<int64_t>")
AddTest("TestEzVec2OperatorAdd<uint64_t>")

AddTest("TestEzVec2OperatorSubtract<float>")
AddTest("TestEzVec2OperatorSubtract<double>")
AddTest("TestEzVec2OperatorSubtract<int32_t>")
AddTest("TestEzVec2OperatorSubtract<uint32_t>")
AddTest("TestEzVec2OperatorSubtract<int64_t>")
AddTest("TestEzVec2OperatorSubtract<uint64_t>")

AddTest("TestEzVec2OperatorMultiply<float>")
AddTest("TestEzVec2OperatorMultiply<double>")
AddTest("TestEzVec2OperatorMultiply<int32_t>")
AddTest("TestEzVec2OperatorMultiply<uint32_t>")
AddTest("TestEzVec2OperatorMultiply<int64_t>")
AddTest("TestEzVec2OperatorMultiply<uint64_t>")

AddTest("TestEzVec2OperatorDivide<float>")
AddTest("TestEzVec2OperatorDivide<double>")
AddTest("TestEzVec2OperatorDivide<int32_t>")
AddTest("TestEzVec2OperatorDivide<uint32_t>")
AddTest("TestEzVec2OperatorDivide<int64_t>")
AddTest("TestEzVec2OperatorDivide<uint64_t>")

AddTest("TestEzVec2EmptyAND<float>")
AddTest("TestEzVec2EmptyAND<double>")
AddTest("TestEzVec2EmptyAND<int32_t>")
AddTest("TestEzVec2EmptyAND<uint32_t>")
AddTest("TestEzVec2EmptyAND<int64_t>")
AddTest("TestEzVec2EmptyAND<uint64_t>")

AddTest("TestEzVec2EmptyOR<float>")
AddTest("TestEzVec2EmptyOR<double>")
AddTest("TestEzVec2EmptyOR<int32_t>")
AddTest("TestEzVec2EmptyOR<uint32_t>")
AddTest("TestEzVec2EmptyOR<int64_t>")
AddTest("TestEzVec2EmptyOR<uint64_t>")

AddTest("TestEzVec2Min<float>")
AddTest("TestEzVec2Min<double>")
AddTest("TestEzVec2Min<int32_t>")
AddTest("TestEzVec2Min<uint32_t>")
AddTest("TestEzVec2Min<int64_t>")
AddTest("TestEzVec2Min<uint64_t>")

AddTest("TestEzVec2Max<float>")
AddTest("TestEzVec2Max<double>")
AddTest("TestEzVec2Max<int32_t>")
AddTest("TestEzVec2Max<uint32_t>")
AddTest("TestEzVec2Max<int64_t>")
AddTest("TestEzVec2Max<uint64_t>")

AddTest("TestEzVec2GetNormalized<float>")
AddTest("TestEzVec2GetNormalized<double>")

AddTest("TestEzVec2Floor<float>")
AddTest("TestEzVec2Floor<double>")

AddTest("TestEzVec2Fract<float>")
AddTest("TestEzVec2Fract<double>")

AddTest("TestEzVec2Ceil<float>")
AddTest("TestEzVec2Ceil<double>")

AddTest("TestEzVec2Mini<float>")
AddTest("TestEzVec2Mini<double>")
AddTest("TestEzVec2Mini<int32_t>")
AddTest("TestEzVec2Mini<uint32_t>")
AddTest("TestEzVec2Mini<int64_t>")
AddTest("TestEzVec2Mini<uint64_t>")

AddTest("TestEzVec2Maxi<float>")
AddTest("TestEzVec2Maxi<double>")
AddTest("TestEzVec2Maxi<int32_t>")
AddTest("TestEzVec2Maxi<uint32_t>")
AddTest("TestEzVec2Maxi<int64_t>")
AddTest("TestEzVec2Maxi<uint64_t>")

AddTest("TestEzVec2Dot<float>")
AddTest("TestEzVec2Dot<double>")
AddTest("TestEzVec2Dot<int32_t>")
AddTest("TestEzVec2Dot<uint32_t>")
AddTest("TestEzVec2Dot<int64_t>")
AddTest("TestEzVec2Dot<uint64_t>")

AddTest("TestEzVec2Det<float>")
AddTest("TestEzVec2Det<double>")
AddTest("TestEzVec2Det<int32_t>")
AddTest("TestEzVec2Det<uint32_t>")
AddTest("TestEzVec2Det<int64_t>")
AddTest("TestEzVec2Det<uint64_t>")

AddTest("TestEzVec2Reflect<float>")
AddTest("TestEzVec2Reflect<double>")

AddTest("TestEzVec2Sign<float>")
AddTest("TestEzVec2Sign<double>")
AddTest("TestEzVec2Sign<int32_t>")
AddTest("TestEzVec2Sign<int64_t>")

AddTest("TestEzVec2Sin<float>")
AddTest("TestEzVec2Sin<double>")

AddTest("TestEzVec2Cos<float>")
AddTest("TestEzVec2Cos<double>")

AddTest("TestEzVec2Tan<float>")
AddTest("TestEzVec2Tan<double>")

AddTest("TestEzVec2Atan<float>")
AddTest("TestEzVec2Atan<double>")

##########################################################
##### EzVec3 #############################################
##########################################################

AddTest("TestEzVec3Offset<float>")
AddTest("TestEzVec3Offset<double>")
AddTest("TestEzVec3Offset<int32_t>")
AddTest("TestEzVec3Offset<uint32_t>")
AddTest("TestEzVec3Offset<int64_t>")
AddTest("TestEzVec3Offset<uint64_t>")

AddTest("TestEzVec3Set<float>")
AddTest("TestEzVec3Set<double>")
AddTest("TestEzVec3Set<int32_t>")
AddTest("TestEzVec3Set<uint32_t>")
AddTest("TestEzVec3Set<int64_t>")
AddTest("TestEzVec3Set<uint64_t>")

AddTest("TestEzVec3Negate<float>")
AddTest("TestEzVec3Negate<double>")
AddTest("TestEzVec3Negate<int32_t>")
AddTest("TestEzVec3Negate<int64_t>")

AddTest("TestEzVec3LogicalNot<int32_t>")
AddTest("TestEzVec3LogicalNot<int64_t>")

AddTest("TestEzVec3XY<float>")
AddTest("TestEzVec3XY<double>")
AddTest("TestEzVec3XY<int32_t>")
AddTest("TestEzVec3XY<uint32_t>")
AddTest("TestEzVec3XY<int64_t>")
AddTest("TestEzVec3XY<uint64_t>")

AddTest("TestEzVec3XZ<float>")
AddTest("TestEzVec3XZ<double>")
AddTest("TestEzVec3XZ<int32_t>")
AddTest("TestEzVec3XZ<uint32_t>")
AddTest("TestEzVec3XZ<int64_t>")
AddTest("TestEzVec3XZ<uint64_t>")

AddTest("TestEzVec3YZ<float>")
AddTest("TestEzVec3YZ<double>")
AddTest("TestEzVec3YZ<int32_t>")
AddTest("TestEzVec3YZ<uint32_t>")
AddTest("TestEzVec3YZ<int64_t>")
AddTest("TestEzVec3YZ<uint64_t>")

AddTest("TestEzVec3YZX<float>")
AddTest("TestEzVec3YZX<double>")
AddTest("TestEzVec3YZX<int32_t>")
AddTest("TestEzVec3YZX<uint32_t>")
AddTest("TestEzVec3YZX<int64_t>")
AddTest("TestEzVec3YZX<uint64_t>")

AddTest("TestEzVec3Increment<float>")
AddTest("TestEzVec3Increment<double>")
AddTest("TestEzVec3Increment<int32_t>")
AddTest("TestEzVec3Increment<uint32_t>")
AddTest("TestEzVec3Increment<int64_t>")
AddTest("TestEzVec3Increment<uint64_t>")

AddTest("TestEzVec3Decrement<float>")
AddTest("TestEzVec3Decrement<double>")
AddTest("TestEzVec3Decrement<int32_t>")
AddTest("TestEzVec3Decrement<uint32_t>")
AddTest("TestEzVec3Decrement<int64_t>")
AddTest("TestEzVec3Decrement<uint64_t>")

AddTest("TestEzVec3Length<float>")
AddTest("TestEzVec3Length<double>")

AddTest("TestEzVec3Normalize<float>")
AddTest("TestEzVec3Normalize<double>")

AddTest("TestEzVec3Sum<float>")
AddTest("TestEzVec3Sum<double>")
AddTest("TestEzVec3Sum<int32_t>")
AddTest("TestEzVec3Sum<uint32_t>")
AddTest("TestEzVec3Sum<int64_t>")
AddTest("TestEzVec3Sum<uint64_t>")

AddTest("TestEzVec3SumAbs<float>")
AddTest("TestEzVec3SumAbs<double>")
AddTest("TestEzVec3SumAbs<int32_t>")
AddTest("TestEzVec3SumAbs<int64_t>")

AddTest("TestEzVec3EmptyAND<float>")
AddTest("TestEzVec3EmptyAND<double>")
AddTest("TestEzVec3EmptyAND<int32_t>")
AddTest("TestEzVec3EmptyAND<uint32_t>")
AddTest("TestEzVec3EmptyAND<int64_t>")
AddTest("TestEzVec3EmptyAND<uint64_t>")

AddTest("TestEzVec3EmptyOR<float>")
AddTest("TestEzVec3EmptyOR<double>")
AddTest("TestEzVec3EmptyOR<int32_t>")
AddTest("TestEzVec3EmptyOR<uint32_t>")
AddTest("TestEzVec3EmptyOR<int64_t>")
AddTest("TestEzVec3EmptyOR<uint64_t>")

AddTest("TestEzVec3String<float>")
AddTest("TestEzVec3String<double>")
AddTest("TestEzVec3String<int32_t>")
AddTest("TestEzVec3String<uint32_t>")
AddTest("TestEzVec3String<int64_t>")
AddTest("TestEzVec3String<uint64_t>")

AddTest("TestEzVec3Mini<float>")
AddTest("TestEzVec3Mini<double>")
AddTest("TestEzVec3Mini<int32_t>")
AddTest("TestEzVec3Mini<uint32_t>")
AddTest("TestEzVec3Mini<int64_t>")
AddTest("TestEzVec3Mini<uint64_t>")

AddTest("TestEzVec3Maxi<float>")
AddTest("TestEzVec3Maxi<double>")
AddTest("TestEzVec3Maxi<int32_t>")
AddTest("TestEzVec3Maxi<uint32_t>")
AddTest("TestEzVec3Maxi<int64_t>")
AddTest("TestEzVec3Maxi<uint64_t>")

AddTest("TestEzVec3Equality<float>")
AddTest("TestEzVec3Equality<double>")
AddTest("TestEzVec3Equality<int32_t>")
AddTest("TestEzVec3Equality<uint32_t>")
AddTest("TestEzVec3Equality<int64_t>")
AddTest("TestEzVec3Equality<uint64_t>")

AddTest("TestEzVec3Addition<float>")
AddTest("TestEzVec3Addition<double>")
AddTest("TestEzVec3Addition<int32_t>")
AddTest("TestEzVec3Addition<uint32_t>")
AddTest("TestEzVec3Addition<int64_t>")
AddTest("TestEzVec3Addition<uint64_t>")

AddTest("TestEzVec3Subtraction<float>")
AddTest("TestEzVec3Subtraction<double>")
AddTest("TestEzVec3Subtraction<int32_t>")
AddTest("TestEzVec3Subtraction<uint32_t>")
AddTest("TestEzVec3Subtraction<int64_t>")
AddTest("TestEzVec3Subtraction<uint64_t>")

AddTest("TestEzVec3Multiplication<float>")
AddTest("TestEzVec3Multiplication<double>")
AddTest("TestEzVec3Multiplication<int32_t>")
AddTest("TestEzVec3Multiplication<uint32_t>")
AddTest("TestEzVec3Multiplication<int64_t>")
AddTest("TestEzVec3Multiplication<uint64_t>")

AddTest("TestEzVec3Division<float>")
AddTest("TestEzVec3Division<double>")
AddTest("TestEzVec3Division<int32_t>")
AddTest("TestEzVec3Division<uint32_t>")
AddTest("TestEzVec3Division<int64_t>")
AddTest("TestEzVec3Division<uint64_t>")

##########################################################
##### EzVec4 #############################################
##########################################################

AddTest("TestEzVec4Offset<float>")
AddTest("TestEzVec4Offset<double>")
AddTest("TestEzVec4Offset<int32_t>")
AddTest("TestEzVec4Offset<uint32_t>")
AddTest("TestEzVec4Offset<int64_t>")
AddTest("TestEzVec4Offset<uint64_t>")

AddTest("TestEzVec4Set<float>")
AddTest("TestEzVec4Set<double>")
AddTest("TestEzVec4Set<int32_t>")
AddTest("TestEzVec4Set<uint32_t>")
AddTest("TestEzVec4Set<int64_t>")
AddTest("TestEzVec4Set<uint64_t>")

AddTest("TestEzVec4Negate<float>")
AddTest("TestEzVec4Negate<double>")

AddTest("TestEzVec4LogicalNot<int32_t>")
AddTest("TestEzVec4LogicalNot<int64_t>")

AddTest("TestEzVec4XY<float>")
AddTest("TestEzVec4XY<double>")
AddTest("TestEzVec4XY<int32_t>")
AddTest("TestEzVec4XY<uint32_t>")
AddTest("TestEzVec4XY<int64_t>")
AddTest("TestEzVec4XY<uint64_t>")

AddTest("TestEzVec4XYZ<float>")
AddTest("TestEzVec4XYZ<double>")
AddTest("TestEzVec4XYZ<int32_t>")
AddTest("TestEzVec4XYZ<uint32_t>")
AddTest("TestEzVec4XYZ<int64_t>")
AddTest("TestEzVec4XYZ<uint64_t>")

AddTest("TestEzVec4ZW<float>")
AddTest("TestEzVec4ZW<double>")
AddTest("TestEzVec4ZW<int32_t>")
AddTest("TestEzVec4ZW<uint32_t>")
AddTest("TestEzVec4ZW<int64_t>")
AddTest("TestEzVec4ZW<uint64_t>")

AddTest("TestEzVec4Increment<float>")
AddTest("TestEzVec4Increment<double>")
AddTest("TestEzVec4Increment<int32_t>")
AddTest("TestEzVec4Increment<uint32_t>")
AddTest("TestEzVec4Increment<int64_t>")
AddTest("TestEzVec4Increment<uint64_t>")

AddTest("TestEzVec4Decrement<float>")
AddTest("TestEzVec4Decrement<double>")
AddTest("TestEzVec4Decrement<int32_t>")
AddTest("TestEzVec4Decrement<uint32_t>")
AddTest("TestEzVec4Decrement<int64_t>")
AddTest("TestEzVec4Decrement<uint64_t>")

AddTest("TestEzVec4Length<float>")
AddTest("TestEzVec4Length<double>")

AddTest("TestEzVec4Normalize<float>")
AddTest("TestEzVec4Normalize<double>")

AddTest("TestEzVec4Sum<float>")
AddTest("TestEzVec4Sum<double>")
AddTest("TestEzVec4Sum<int32_t>")
AddTest("TestEzVec4Sum<uint32_t>")
AddTest("TestEzVec4Sum<int64_t>")
AddTest("TestEzVec4Sum<uint64_t>")

AddTest("TestEzVec4SumAbs<float>")
AddTest("TestEzVec4SumAbs<double>")
AddTest("TestEzVec4SumAbs<int32_t>")
AddTest("TestEzVec4SumAbs<int64_t>")

AddTest("TestEzVec4EmptyAND<float>")
AddTest("TestEzVec4EmptyAND<double>")
AddTest("TestEzVec4EmptyAND<int32_t>")
AddTest("TestEzVec4EmptyAND<uint32_t>")
AddTest("TestEzVec4EmptyAND<int64_t>")
AddTest("TestEzVec4EmptyAND<uint64_t>")

AddTest("TestEzVec4EmptyOR<float>")
AddTest("TestEzVec4EmptyOR<double>")
AddTest("TestEzVec4EmptyOR<int32_t>")
AddTest("TestEzVec4EmptyOR<uint32_t>")
AddTest("TestEzVec4EmptyOR<int64_t>")
AddTest("TestEzVec4EmptyOR<uint64_t>")

AddTest("TestEzVec4String<float>")
AddTest("TestEzVec4String<double>")
AddTest("TestEzVec4String<int32_t>")
AddTest("TestEzVec4String<uint32_t>")
AddTest("TestEzVec4String<int64_t>")
AddTest("TestEzVec4String<uint64_t>")

AddTest("TestEzVec4Mini<float>")
AddTest("TestEzVec4Mini<double>")
AddTest("TestEzVec4Mini<int32_t>")
AddTest("TestEzVec4Mini<uint32_t>")
AddTest("TestEzVec4Mini<int64_t>")
AddTest("TestEzVec4Mini<uint64_t>")

AddTest("TestEzVec4Maxi<float>")
AddTest("TestEzVec4Maxi<double>")
AddTest("TestEzVec4Maxi<int32_t>")
AddTest("TestEzVec4Maxi<uint32_t>")
AddTest("TestEzVec4Maxi<int64_t>")
AddTest("TestEzVec4Maxi<uint64_t>")

AddTest("TestEzVec4Equality<float>")
AddTest("TestEzVec4Equality<double>")
AddTest("TestEzVec4Equality<int32_t>")
AddTest("TestEzVec4Equality<uint32_t>")
AddTest("TestEzVec4Equality<int64_t>")
AddTest("TestEzVec4Equality<uint64_t>")

AddTest("TestEzVec4Addition<float>")
AddTest("TestEzVec4Addition<double>")
AddTest("TestEzVec4Addition<int32_t>")
AddTest("TestEzVec4Addition<uint32_t>")
AddTest("TestEzVec4Addition<int64_t>")
AddTest("TestEzVec4Addition<uint64_t>")

AddTest("TestEzVec4Subtraction<float>")
AddTest("TestEzVec4Subtraction<double>")
AddTest("TestEzVec4Subtraction<int32_t>")
AddTest("TestEzVec4Subtraction<uint32_t>")
AddTest("TestEzVec4Subtraction<int64_t>")
AddTest("TestEzVec4Subtraction<uint64_t>")

AddTest("TestEzVec4Multiplication<float>")
AddTest("TestEzVec4Multiplication<double>")
AddTest("TestEzVec4Multiplication<int32_t>")
AddTest("TestEzVec4Multiplication<uint32_t>")
AddTest("TestEzVec4Multiplication<int64_t>")
AddTest("TestEzVec4Multiplication<uint64_t>")

AddTest("TestEzVec4Division<float>")
AddTest("TestEzVec4Division<double>")
AddTest("TestEzVec4Division<int32_t>")
AddTest("TestEzVec4Division<uint32_t>")
AddTest("TestEzVec4Division<int64_t>")
AddTest("TestEzVec4Division<uint64_t>")

##########################################################
##### EzStr ##############################################
##########################################################

AddTest("TestEzStrGetDigitsCountOfAIntegralNumber")
AddTest("TestEzStrSearchForPatternWithWildcards")

##########################################################
##### EzXml ##############################################
##########################################################

AddTest("TestEzXmlParsingOK")
AddTest("TestEzXmlParsingNOK_0")
AddTest("TestEzXmlParsingNOK_1")

##########################################################
##### EzCron #############################################
##########################################################

AddTest("TestEzCron_Format_Invalid_Mins")
AddTest("TestEzCron_Format_Invalid_Hours")
AddTest("TestEzCron_Format_Invalid_MonthDay")
AddTest("TestEzCron_Format_Invalid_Month")
AddTest("TestEzCron_Format_Invalid_WeekDay")
AddTest("TestEzCron_Format_Invalid_ManyErrors_0")
AddTest("TestEzCron_Format_Invalid_ManyErrors_1")
AddTest("TestEzCron_Format_Invalid_FieldCount_0")
AddTest("TestEzCron_Format_Invalid_FieldCount_1")
AddTest("TestEzCron_Format_Valid_Type_Value")
AddTest("TestEzCron_Format_inValid_Type_Value")
AddTest("TestEzCron_Format_Valid_Type_Interval")
AddTest("TestEzCron_Format_InValid_Type_Interval")
AddTest("TestEzCron_Format_Valid_Type_Range")
AddTest("TestEzCron_Format_InValid_Type_Range")
AddTest("TestEzCron_Format_Valid_Type_Values")
AddTest("TestEzCron_Format_InValid_Type_Values")
AddTest("TestEzCron_TimeCheck_Valid")
