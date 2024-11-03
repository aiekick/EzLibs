function(enable_coverage target)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -g -O0 --coverage)
        target_link_libraries(${target} PRIVATE --coverage)
		add_custom_target(coverage_html
			COMMAND lcov --capture --directory . --output-file coverage.info
			COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage.info
			COMMAND genhtml coverage.info --output-directory coverage_html
			COMMENT "Generating HTML coverage report with lcov and genhtml"
		)
    elseif(MSVC)
        target_compile_options(${target} PRIVATE /Zi /Ob0 /Od /RTC1)
        target_link_options(${target} PRIVATE /PROFILE)
		add_custom_target(coverage_html
			COMMAND "ReportGenerator.exe" -reports:*.coverage -targetdir:coverage_html
			COMMENT "Generating HTML coverage report with ReportGenerator"
		)
    endif()
	add_custom_target(run_tests
		COMMAND ctest --output-on-failure
		COMMAND ${CMAKE_COMMAND} --build . --target coverage_html
		DEPENDS ${PROJECT}
		COMMENT "Running tests and generating coverage report"
	)
endfunction()

