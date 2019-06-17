cmake_minimum_required(VERSION 2.6)
project(prs)

# The version number
set(PRS_VERSION_MAJOR 0)
set(PRS_VERSION_MINOR 1)
set(PRS_VERSION "${PRS_VERSION_MAJOR}.${PRS_VERSION_MINOR}")

# configure header
configure_file(
	${PROJECT_SOURCE_DIR}/prs.h.in
	${PROJECT_BINARY_DIR}/prs.h
	@ONLY
)

# generate prs.pc
if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
	set(CMAKE_INSTALL_LIBDIR lib CACHE PATH "Output directories for libraries")
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)
configure_file(
	${PROJECT_SOURCE_DIR}/prs.pc.in
	${PROJECT_BINARY_DIR}/prs.pc
	@ONLY
)

# add binary tree to search path
include_directories(
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}/include"
)

add_library(prs src/file.c src/clogger.c src/bitmap.c src/sockhelp.c src/ustack.c)
#enable testing
option(BUILD_TESTS "Enable testing of library." OFF)
if(BUILD_TESTS)
enable_testing()
add_subdirectory(test)
endif(BUILD_TESTS)

