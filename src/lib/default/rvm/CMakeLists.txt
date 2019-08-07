add_library(DefaultRvmHeader OBJECT header.cpp)
target_include_directories(DefaultRvmHeader
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/rvmparser>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/rvmparser>
)
target_compile_options(DefaultRvmHeader
	PRIVATE
		-fconcepts
)
target_compile_features(DefaultRvmHeader
	PRIVATE
		cxx_std_20
)
