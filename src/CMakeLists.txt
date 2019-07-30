project(${USR_SOLUTION} CXX)
message("Project " ${PROJECT_NAME}:)

file(GLOB SOURCES
    "*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
	PUBLIC
		
)
target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<INSTALL_INTERFACE:/include>
)
target_compile_options(${PROJECT_NAME}
	PRIVATE
		-Werror
		-Wfatal-errors
		-fconcepts
)
target_compile_features(${PROJECT_NAME}
	PRIVATE
		cxx_std_20
)
