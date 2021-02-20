CMAKE_MINIMUM_REQUIRED( VERSION 3.10)
project( bsearch_test)
add_definitions(-std=gnu99)
add_executable(
        bsearch_test
        bsearch_test.c
        bsearch.h
)