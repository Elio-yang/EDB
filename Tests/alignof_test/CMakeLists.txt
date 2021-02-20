CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(alignoftest)
SET(CMAKE_C_STANDARD 99)
add_executable(
        alignof_test
        run.c
        alignof.h
        compiler.h
        config.h
        tap.c
        tap.h
)
