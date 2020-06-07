cmake_minimum_required(VERSION 2.6)

# all executables for sockhelp testing
add_executable(sockhelp_test1 test1.c)
add_executable(sockhelp_test2 test2.c)
add_executable(sockhelp_test3 test3.c)
add_executable(sockhelp_test4 test4.c)
add_executable(sockhelp_test5 test5.c)

# link executables to libraries
if(WIN32)
	target_link_libraries(sockhelp_test1 prs ws2_32)
	target_link_libraries(sockhelp_test2 prs ws2_32)
	target_link_libraries(sockhelp_test3 prs ws2_32)
	target_link_libraries(sockhelp_test4 prs ws2_32)
	target_link_libraries(sockhelp_test5 prs ws2_32)
else(UNIX)
	target_link_libraries(sockhelp_test1 prs)
	target_link_libraries(sockhelp_test2 prs)
	target_link_libraries(sockhelp_test3 prs)
	target_link_libraries(sockhelp_test4 prs)
	target_link_libraries(sockhelp_test5 prs)
endif(WIN32)

# add all executables for testing
add_test(sockhelp_test1 sockhelp_test1)
add_test(sockhelp_test2 sockhelp_test2)
add_test(sockhelp_test3 sockhelp_test3)
add_test(sockhelp_test4 sockhelp_test4)
add_test(sockhelp_test5 sockhelp_test5)
