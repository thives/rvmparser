add_subdirectory(default)

# Define library
add_library(${PROJECT_NAME} SHARED dummy.cpp)
target_link_libraries(${PROJECT_NAME} 
	PUBLIC
		${OPENBLAS_LIBS}
		DefaultRvmHeader
	PRIVATE
		
)
target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/rvmparser>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/rvmparser>
)
target_compile_options(${PROJECT_NAME}
	PRIVATE
		-fconcepts
)
target_compile_features(${PROJECT_NAME}
	PRIVATE
		cxx_std_20
)
