/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/19
 */

#ifndef MY_DATABASE_ARRAY_SIZE_H
#define MY_DATABASE_ARRAY_SIZE_H

#include <build_assert.h>
#define must_be_array(arr) \
        build_assert_or_zero( \
                !__builtin_types_compatible_p(typeof(arr),typeof(&(arr)[0])) \
                        )

#define array_size(arr)(sizeof(arr)/sizeof((arr)[0])+must_be_array(arr))

#endif //MY_DATABASE_ARRAY_SIZE_H
