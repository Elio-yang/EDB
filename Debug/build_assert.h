/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/19
 */

#ifndef MY_DATABASE_BUILD_ASSERT_H
#define MY_DATABASE_BUILD_ASSERT_H


/**
 * only be used within a function
 * @e the condition which must be true
 *
 */
#define build_assert(e) \
        do{(void) sizeof(char [1-2*!(e)]);}while(0)

/**
 * can be used as an expression
 * @e the condition which must be true
 */
#define build_assert_or_zero(e) \
        (sizeof(char [1-2*!(e)])-1)

#endif //MY_DATABASE_BUILD_ASSERT_H
