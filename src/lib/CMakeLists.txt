project(${USR_SOLUTION} CXX)
message("Project " ${PROJECT_NAME}:)

add_subdirectory(default)

# Define library
add_library(${PROJECT_NAME} IMPORTED)
target_link_libraries(${PROJECT_NAME} 
	PUBLIC
		${OPENBLAS_LIBS}
	PRIVATE
		DefaultRvm::DefaultRvmHeader
)
target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<INSTALL_INTERFACE:/include/rvmparser>
)
target_compile_options(${PROJECT_NAME}
	PRIVATE
		-fconcepts
)
target_compile_features(${PROJECT_NAME}
	PRIVATE
		cxx_std_20
)

install(
	TARGETS
		${PROJECT_NAME}
	EXPORT
		${PROJECT_NAME}Targets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)

install(
	EXPORT
		${PROJECT_NAME}Targets 
	NAMESPACE 
		${PROJECT_NAME}::
	DESTINATION lib/cmake/${PROJECT_NAME}
)
