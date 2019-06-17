cmake_minimum_required(VERSION 2.6)
project(prs)

set(CMAKE_BUILD_TYPE Release)

# The version number
set(PRS_VERSION_MAJOR 0)
set(PRS_VERSION_MINOR 1)
set(PRS_VERSION "${PRS_VERSION_MAJOR}.${PRS_VERSION_MINOR}")

# add binary tree to search path
include_directories(
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}/include"
)

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
	if(WIN32)
		set(CMAKE_INSTALL_PREFIX "\Program Files\PRS")
	elseif(UNIX)
		set(CMAKE_INSTALL_PREFIX "/usr")
	else()
		set(CMAKE_INSTALL_PREFIX "")
	endif()
endif(NOT DEFINED CMAKE_INSTALL_PREFIX)

# configure header
configure_file(
	${PROJECT_SOURCE_DIR}/prs.h.in
	${PROJECT_BINARY_DIR}/prs.h
	@ONLY
)

if(WIN32)
set(PRS_LIBDIR "\Program Files\libprs")
else()
set(PRS_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib")
endif()

# generate prs.pc
if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
	set(CMAKE_INSTALL_LIBDIR ${PRS_LIBDIR})
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)
configure_file(
	${PROJECT_SOURCE_DIR}/prs.pc.in
	${PROJECT_BINARY_DIR}/prs.pc
	@ONLY
)
if(WIN32)
	add_library(prs_static STATIC src/file.c src/clogger.c src/bitmap.c src/sockhelp.c src/ustack.c)
	add_library(prs SHARED src/file.c src/clogger.c src/bitmap.c src/sockhelp.c src/ustack.c)
	install(TARGETS prs_static ARCHIVE DESTINATION ${PRS_LIBDIR} CONFIGURATIONS Release)
	install(TARGETS prs LIBRARY DESTINATION ${PRS_LIBDIR} CONFIGURATIONS Release)
else(UNIX)
	add_library(prs_static STATIC src/file.c src/clogger.c src/bitmap.c src/sockhelp.c src/ustack.c)
	add_library(prs SHARED src/file.c src/clogger.c src/bitmap.c src/sockhelp.c src/ustack.c)
	install(TARGETS prs_static ARCHIVE DESTINATION ${PRS_LIBDIR} CONFIGURATIONS Release)
	install(TARGETS prs LIBRARY DESTINATION ${PRS_LIBDIR} CONFIGURATIONS Release)
endif()

file(GLOB files ${PROJECT_SOURCE_DIR}/include/*.h)
foreach(file ${files})
install(TARGETS ${i}
	DESTINATION ${CMAKE_INSTALL_PREFIX}/include/prs)
endforeach()

#enable testing
option(BUILD_TESTS "Enable testing of library." OFF)
if(BUILD_TESTS)
enable_testing()
add_subdirectory(test)
endif(BUILD_TESTS)

