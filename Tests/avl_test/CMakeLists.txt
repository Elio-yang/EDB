CMAKE_MINIMUM_REQUIRED( VERSION 3.10)
project(avl_test)
add_definitions(-std=gnu99)
set(CMAKE_C_STANDARD 99)
add_executable(
        avl_test
        
        avl_test.c
        avl.h
        avl.c
)