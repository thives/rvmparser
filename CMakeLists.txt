cmake_minimum_required(VERSION 3.15)

list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")
set(CMAKE_INSTALL_PREFIX /usr/local)
set(CMAKE_BUILD_TYPE Debug)
set(THREADS_PREFER_PTHREAD_FLAG)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(USR_SOLUTION "thives-rvmparser")

include(CTest)
include(FindPkgConfig)
include(FindThreads)
include(GNUInstallDirs)

find_package(RapidJSON REQUIRED CONFIG)
find_package(OpenBLAS REQUIRED CONFIG)

#pkg_check_modules(RABBITMQ REQUIRED librabbitmq)

add_subdirectory(tests)
#add_subdirectory(src)
