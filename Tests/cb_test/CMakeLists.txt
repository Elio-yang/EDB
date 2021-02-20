cmake_minimum_required(VERSION 3.10)
PROJECT(CB_test)
add_definitions(-g3 -ggdb -Wall -Wundef -Wmissing-prototypes -Wmissing-declarations -Wstrict-prototypes -Wold-style-definition)
add_definitions(-std=gnu99)
add_executable(
	cb_test
	cb_test.c
	typesafe_cb.h
	)
