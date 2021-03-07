/*
 * Libs/defs.h
 * useful definitions & macros
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/8
 */

#ifndef MY_DATABASE_DEFS_H
#define MY_DATABASE_DEFS_H

#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#define uninitialized 0x3f3f3f3f
#define EXIT_SUCC 0
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define BUFFER_SIZE (4*1024)
#define __always_inline inline __attribute__((always_inline))
#define __noinline __attribute__((noinline))
#define __noreturn __attribute__((noreturn))
#define TABLE_MAX_PAGE 100
#define MAX_FILENAME_LEN 255
#if !defined(bool)
/* Represents true-or-false */
typedef int bool;
#endif

#define nullptr (void*)0
/* Explicitly-sized versions of integer types */
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

/* *
 * Pointers and addresses are 32 bits long.
 * We use pointer types to represent addresses,
 * uintptr_t to represent the numerical values of addresses.
 * */
typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

/* size_t is used for memory object sizes */
#include <stddef.h>

/* off_t is used for file offsets and lengths */
//typedef intptr_t off_t;

/*Are two types/vars the same type(ignoring qualifiers)*/
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a),typeof(b))

/*Force a compilation error if e!=0*/
#define BUILD_BUG_ON_ZERO(e) ( (int)(sizeof(struct{int:(-!!(e));})))

/*Test whther arr is an array*/
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a),&(a)[0]))

/*Get the number of elements in array a*/
#define ARRAY_SIZE(a) ((sizeof(a)/sizeof((a)[0]))+__must_be_array(a))

#define size_of_member(struct,mem) (sizeof(((struct*)0)->mem))

/**
 * Get the offset of a structure member
 * @param type type of the structure
 * @param mem a member of the structure
 */
#define __offsetof__(type, mem) ((size_t)(&(type*)0)->mem)

/**
 * Get the pointer of the whole structure
 * @param ptr a pointer to the member
 * @param type type of the structure
 * @param mem a member of the structure
 */
#define __container_of__(ptr, type, mem)({\
                const typeof( ((type*)0)->mem )* _mptr=(ptr);   \
                (type*)( (char*)_mptr-__offsetof__(type,mem) ) \
                })


#endif //MY_DATABASE_DEFS_H
