project(${USR_SOLUTION}_tests CXX)
message("Project " ${PROJECT_NAME}:)

### Parse Integer
add_executable(${PROJECT_NAME}_parse_integer
	${CMAKE_CURRENT_SOURCE_DIR}/test_parse_integer.cpp
)

target_include_directories(${PROJECT_NAME}_parse_integer
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/rvmparser/>
)

target_compile_options(${PROJECT_NAME}_parse_integer
	PRIVATE
		-Werror
		-fconcepts
)
target_compile_features(${PROJECT_NAME}_parse_integer
	PRIVATE
		cxx_std_20
)
add_test(NAME parse_integer COMMAND ${PROJECT_NAME}_parse_integer WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


### Parse Floating
add_executable(${PROJECT_NAME}_parse_floating
	${CMAKE_CURRENT_SOURCE_DIR}/test_parse_floating.cpp
)

target_include_directories(${PROJECT_NAME}_parse_floating
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/rvmparser/>
)

target_compile_options(${PROJECT_NAME}_parse_floating
	PRIVATE
		-Werror
		-fconcepts
)
target_compile_features(${PROJECT_NAME}_parse_floating
	PRIVATE
		cxx_std_20
)
add_test(NAME parse_floating COMMAND ${PROJECT_NAME}_parse_floating WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


### Parse String
add_executable(${PROJECT_NAME}_parse_string
	${CMAKE_CURRENT_SOURCE_DIR}/test_parse_string.cpp
)

target_include_directories(${PROJECT_NAME}_parse_string
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/rvmparser/>
)

target_compile_options(${PROJECT_NAME}_parse_string
	PRIVATE
		-Werror
		-fconcepts
)
target_compile_features(${PROJECT_NAME}_parse_string
	PRIVATE
		cxx_std_20
)
add_test(NAME parse_string COMMAND ${PROJECT_NAME}_parse_string WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


### Parse Header
add_executable(${PROJECT_NAME}_parse_header
	${CMAKE_CURRENT_SOURCE_DIR}/test_parse_header.cpp
)

target_include_directories(${PROJECT_NAME}_parse_header
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/rvmparser/>
)

target_compile_options(${PROJECT_NAME}_parse_header
	PRIVATE
		-Werror
		-fconcepts
)
target_compile_features(${PROJECT_NAME}_parse_header
	PRIVATE
		cxx_std_20
)
add_test(NAME parse_header COMMAND ${PROJECT_NAME}_parse_header WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})



