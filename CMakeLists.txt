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

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -std=c89 -Wall -Wextra -Werror -g")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -std=c89 -Wall -Wextra -Werror -O2")

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
	if(WIN32)
		set(CMAKE_INSTALL_PREFIX "C:\Program Files\PRS-Library")
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

# generate prs.pc
if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
	set(CMAKE_INSTALL_LIBDIR lib)
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)
configure_file(
	${PROJECT_SOURCE_DIR}/prs.pc.in
	${PROJECT_BINARY_DIR}/prs.pc
	@ONLY
)
if(WIN32)
	add_library(prs SHARED src/file.c src/clogger.c src/bitmap.c src/bitfiddle.c src/sockhelp.c src/ustack.c)
	target_link_libraries(prs ws2_32)
	set_target_properties(prs PROPERTIES PREFIX "")
	install(TARGETS prs ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR} COMPONENT library)
else(UNIX)
	add_library(prs_static STATIC src/file.c src/clogger.c src/bitmap.c src/bitfiddle.c src/sockhelp.c src/ustack.c)
	add_library(prs SHARED src/file.c src/clogger.c src/bitmap.c src/bitfiddle.c src/sockhelp.c src/ustack.c)
	install(TARGETS prs prs_static ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR} COMPONENT library)
	install(FILES ${CMAKE_BINARY_DIR}/prs.pc DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pkgconfig")
endif(WIN32)

file(GLOB PRS_HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)
foreach(include ${PRS_HEADERS})
install(FILES ${include} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/prs")
endforeach()

#enable testing
option(BUILD_TESTS "Enable testing of library." OFF)
if(BUILD_TESTS)
enable_testing()
add_subdirectory(test)
endif(BUILD_TESTS)

### Building documentation ###
option(BUILD_DOCS "Build documentation" OFF)

if(CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
	set(BUILD_DOCS ON)
endif()

# check if Doxygen is installed
find_package(Doxygen)
if(DOXYGEN_FOUND)
	if(BUILD_DOCS)
		# set input and output files
		set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile.in)
		set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

		# request to configure the file
		configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
		message("Doxygen build started")

		# note the option ALL which allows building docs with project
		add_custom_target(doc_doxygen ALL
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/include
			COMMENT "Generating API Documentation with Doxygen"
			VERBATIM)
		if(WIN32)
			install(DIRECTORY doc DESTINATION "${CMAKE_INSTALL_PREFIX}")
		else(UNIX)
			install(DIRECTORY doc DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/${CMAKE_PROJECT_NAME}-${PRS_VERSION}")
		endif(WIN32)
	endif(BUILD_DOCS)
else(DOXYGEN_FOUND)
	message("Doxygen not found")
endif(DOXYGEN_FOUND)


