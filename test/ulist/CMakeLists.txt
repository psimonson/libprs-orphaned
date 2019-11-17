cmake_minimum_required(VERSION 2.6)

# all executables for sockhelp testing
add_executable(ulist_test1 test1.c)
add_executable(ulist_test2 test2.c)
add_executable(ulist_test3 test3.c)
add_executable(ulist_test4 test4.c)
add_executable(ulist_test5 test5.c)

# link executables to libraries
target_link_libraries(ulist_test1 prs)
target_link_libraries(ulist_test2 prs)
target_link_libraries(ulist_test3 prs)
target_link_libraries(ulist_test4 prs)
target_link_libraries(ulist_test5 prs)

# add all executables for testing
add_test(ulist_test1 ulist_test1)
add_test(ulist_test2 ulist_test2)
add_test(ulist_test3 ulist_test3)
add_test(ulist_test4 ulist_test4)
add_test(ulist_test5 ulist_test5)
