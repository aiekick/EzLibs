cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0135 OLD) ## for ExternalProject_Add

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CmakeTargets")

option(TESTING_APP "Enable testing of Time related libs" ON)
option(TESTING_FILE "Enable testing of File related libs" ON)
option(TESTING_MATH "Enable testing of Math related libs" ON)
option(TESTING_MISC "Enable testing of Misc related libs" ON)
option(TESTING_OPENGL "Enable testing of Opengl related libs" ON)
option(TESTING_TIME "Enable testing of Time related libs" ON)

if (UNIX)
	option(USE_CODE_COVERAGE "Enable the Code Coverage reporting" OFF)
endif(UNIX)

if (USE_CODE_COVERAGE)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_compile_options( --coverage)
		add_link_options(--coverage)
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
		add_link_options(-fprofile-instr-generate)
	endif()
	function(generate_gcov target)
		add_custom_target(${target}_Coverage
			COMMAND ${CMAKE_COMMAND} -E echo "Génération de la couverture avec gcov"
			COMMAND find $<TARGET_FILE_DIR:${target}> -name "*.gcno" -exec gcov {} +
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			COMMENT "Exécution de gcov pour générer la couverture de code"
		)
	endfunction(AddTest)
endif()

function(AddTest arg)
  add_test("${arg}" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}" "${arg}")
endfunction(AddTest)

if (TESTING_APP)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TestApp)
endif()

if (TESTING_FILE)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TestFile)
endif()

if (TESTING_MATH)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TestMath)
endif()

if (TESTING_MISC)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TestMisc)
endif()

if (TESTING_OPENGL)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TestOpengl)
endif()

if (TESTING_TIME)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TestTime)
endif()

if (USE_CODE_COVERAGE)
	add_custom_target(COVERAGE_HTML
		COMMAND lcov --capture --directory . --output-file coverage.info
		COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage.info
		COMMAND genhtml coverage.info --output-directory COVERAGE_HTML
		COMMENT "Generating HTML coverage report with lcov and genhtml"
	)
	set_target_properties(COVERAGE_HTML PROPERTIES FOLDER CmakeTargets)
	add_custom_target(RUN_TESTS_WITH_COVERAGE
		COMMAND ctest --output-on-failure
		COMMAND ${CMAKE_COMMAND} --build . --target coverage_html
		COMMENT "Running tests and generating coverage report"
	)
	set_target_properties(RUN_TESTS_WITH_COVERAGE PROPERTIES FOLDER CmakeTargets)
endif()
