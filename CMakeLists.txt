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

# generate prs.pc
if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
	set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)
configure_file(
	${PROJECT_SOURCE_DIR}/prs.pc.in
	${PROJECT_BINARY_DIR}/prs.pc
	@ONLY
)
if(WIN32)
	add_library(prs_static STATIC src/file.c src/clogger.c src/bitmap.c src/sockhelp.c src/ustack.c)
	add_library(prs SHARED src/file.c src/clogger.c src/bitmap.c src/sockhelp.c src/ustack.c)
	install(TARGETS prs_static ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} CONFIGURATIONS Release)
	install(TARGETS prs LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} CONFIGURATIONS Release)
else(UNIX)
	add_library(prs_static STATIC src/file.c src/clogger.c src/bitmap.c src/sockhelp.c src/ustack.c)
	add_library(prs SHARED src/file.c src/clogger.c src/bitmap.c src/sockhelp.c src/ustack.c)
	install(TARGETS prs_static ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} CONFIGURATIONS Release)
	install(TARGETS prs LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} CONFIGURATIONS Release)
	install(FILES ${CMAKE_BINARY_DIR}/prs.pc DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pkgconfig")
endif()

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

if(UNIX)
	set(MAN_NAMES sockhelp.3 bitmap.3 ustack.3) # file.3 clogger.3
	set(MAN_FILES)
	foreach(m IN LISTS MAN_NAMES)
		set(mf ${CMAKE_INSTALL_PREFIX}/share/man/man3/${m})
		set(ms ${CMAKE_SOURCE_DIR}/man/man3/${m})
		add_custom_command(OUTPUT ${mf}
			COMMAND ${CMAKE_COMMAND} -E copy ${ms} ${mf}
			DEPENDS ${ms}
			WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}
			COMMENT "Copying manpage ${ms} to ${mf}"
			VERBATIM)
		add_custom_command(OUTPUT ${mf}.bz2
			COMMAND bzip2 ${mf}
			DEPENDS ${mf}
			WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}
			COMMENT "Compressing manpage ${mf} to ${mf}.bz2"
			VERBATIM)
		list(APPEND MAN_FILES ${mf}.bz2)
	endforeach()
	add_custom_target(man DEPENDS ${MAN_FILES})
endif(UNIX)
