file(GLOB LOCALSOURCES
    "*.cpp"
)

list(APPEND ${SOURCES} ${LOCALSOURCES} PARENT_SCOPE)

add_library(DefaultRvmHeader header.h header.cpp)
install(
	TARGETS 
		DefaultRvmHeader 
	EXPORT
		DefaultRvmTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
install(
	EXPORT
		DefaultRvmTargets
	NAMESPACE
		DefaultRvm::
	DESTINATION
		lib/cmake/DefaultRvm
)
