cmake_minimum_required(VERSION 2.6)

# add executables to build
add_executable(test_test test.c)
target_link_libraries(test_test prs)
add_executable(test_test2 test2.c)
target_link_libraries(test_test2 prs)
add_executable(test_test3 test3.c)
target_link_libraries(test_test3 prs)
add_executable(test_test4 test4.c)
target_link_libraries(test_test4 prs)
add_executable(test_test5 test5.c)
target_link_libraries(test_test5 prs)
add_executable(test_test6 test6.c)
target_link_libraries(test_test6 prs)

# testing
add_test(NAME test_test
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
	COMMAND test_test)
add_test(NAME test_test2
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
	COMMAND test_test2)
add_test(NAME test_test3
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
	COMMAND test_test3)
add_test(NAME test_test4
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
	COMMAND test_test4)
add_test(NAME test_test5
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
	COMMAND test_test5)
add_test(NAME test_test6
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
	COMMAND test_test6)

# add sockhelp tests
add_subdirectory(sockhelp)
add_subdirectory(ulist)
