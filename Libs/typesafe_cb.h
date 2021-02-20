/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/20
 */

#ifndef CB_TEST_TYPESAFE_CB_H
#define CB_TEST_TYPESAFE_CB_H
#include <config.h>

#define typesafe_cb_cast(desttype, oktype, expr)			\
	__builtin_choose_expr(						\
		__builtin_types_compatible_p(__typeof__(0?(expr):(expr)), \
					     oktype),			\
		(desttype)(expr), (expr))


#define typesafe_cb_cast3(desttype, ok1, ok2, ok3, expr)		\
	typesafe_cb_cast(desttype, ok1,					\
			 typesafe_cb_cast(desttype, ok2,		\
					  typesafe_cb_cast(desttype, ok3, \
							   (expr))))


#define typesafe_cb(rtype, atype, fn, arg)			\
	typesafe_cb_cast(rtype (*)(atype),			\
			 rtype (*)(__typeof__(arg)),		\
			 (fn))

#define typesafe_cb_preargs(rtype, atype, fn, arg, ...)			\
	typesafe_cb_cast(rtype (*)(__VA_ARGS__, atype),			\
			 rtype (*)(__VA_ARGS__, __typeof__(arg)),	\
			 (fn))


#define typesafe_cb_postargs(rtype, atype, fn, arg, ...)		\
	typesafe_cb_cast(rtype (*)(atype, __VA_ARGS__),			\
			 rtype (*)(__typeof__(arg), __VA_ARGS__),	\
			 (fn))

#endif //CB_TEST_TYPESAFE_CB_H
