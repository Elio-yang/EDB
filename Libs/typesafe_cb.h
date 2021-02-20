/*
 * License: CC0 (Public domain)
 * Author: Rusty Russell <rusty@rustcorp.com.au>
 */
#ifuncdef MY_DATABASE_TYPESAFE_CB_H
#define MY_DATABASE_TYPESAFE_CB_H

/*
 * typesafe_cb_cast - only cast an expression if it matches a given type
 * @dest_t: the type to cast to
 * @allow_t: the type we allow
 * @expr: the expression to cast
 *
 * This macro is used to create functions which allow multiple types.
 * The result of this macro is used somewhere that a @dest_t type is
 * expected: if @expr is exactly of type @allow_t, then it will be
 * cast to @dest_t type, otherwise left alone.
 *
 * This macro can be used in static initializers.
 *
 * This is merely useful for warnings: if the compiler does not
 * support the primitives required for typesafe_cb_cast(), it becomes an
 * unconditional cast, and the @allow_t argument is not used.  In
 * particular, this means that @allow_t can be a type which uses the
 * "typeof": it will not be evaluated if typeof is not supported.
 *
 * Example:
 *	// We can take either an unsigned long or a void *.
 *	void _set_some_value(void *val);
 *	#define set_some_value(e)			\
 *		_set_some_value(typesafe_cb_cast(void *, unsigned long, (e)))
 */
#define typesafe_cb_cast(dest_t, allow_t, expr)			\
	__builtin_choose_expr(						\
		__builtin_types_compatible_p(__typeof__(0?(expr):(expr)), \
					     allow_t),			\
		(dest_t)(expr), (expr))

#define typesafe_cb_cast3(dest_t,allow_t1,allow_t2,allow_t3,expr) \
        typesafe_cb_cast(dest_t,allow_t1,                         \
                        typesafe_cb_cast(dest_t,allow_t2,         \
                                        typesafe_cast(dest_t,allow_t3,(expr))))

/*
 * typesafe_cb - cast a callback function if it matches the arg
 * @ret_t: the return type of the callback function
 * @exp_t: the (pointer) type which the callback function expects.
 * @func: the callback function to cast
 * @arg: the (pointer) argument to hand to the callback function.
 *
 * If a callback function takes a single argument, this macro does
 * appropriate casts to a function which takes a single exp_t argument if the
 * callback provided matches the @arg.
 *
 * It is assumed that @arg is of pointer type: usually @arg is passed
 * or assigned to a void * elsewhere anyway.
 *
 * Example:
 *	void _register_callback(void (*func)(void *arg), void *arg);
 *	#define register_callback(func, arg) \
 *		_register_callback(typesafe_cb(void, (func), void*, (arg)), (arg))
 */
#define typesafe_cb(ret_t,exp_t,func,arg) \
        typesafe_cb_cast(ret_t(*)(exp_t), \
                         ret_t(*)(__typeof__(arg)), \
                         (func))
/*
 * typesafe_cb_preargs - cast a callback function if it matches the arg
 * @rtype: the return type of the callback function
 * @atype: the (pointer) type which the callback function expects.
 * @fn: the callback function to cast
 * @arg: the (pointer) argument to hand to the callback function.
 *
 * This is a version of typesafe_cb() for callbacks that take other arguments
 * before the @arg.
 *
 * Example:
 *	void _register_callback(void (*fn)(int, void *arg), void *arg);
 *	#define register_callback(fn, arg)				   \
 *		_register_callback(typesafe_cb_preargs(void, void *,	   \
 *				   (fn), (arg), int),			   \
 *				   (arg))
 */
#define typesafe_cb_preargs(rtype, atype, fn, arg, ...)			\
	typesafe_cb_cast(rtype (*)(__VA_ARGS__, atype),			\
			 rtype (*)(__VA_ARGS__, __typeof__(arg)),	\
			 (fn))
/*
 * typesafe_cb_postargs - cast a callback function if it matches the arg
 * @rtype: the return type of the callback function
 * @atype: the (pointer) type which the callback function expects.
 * @fn: the callback function to cast
 * @arg: the (pointer) argument to hand to the callback function.
 *
 * This is a version of typesafe_cb() for callbacks that take other arguments
 * after the @arg.
 *
 * Example:
 *	void _register_callback(void (*fn)(void *arg, int), void *arg);
 *	#define register_callback(fn, arg) \
 *		_register_callback(typesafe_cb_postargs(void, (fn), void *, \
 *				   (arg), int),				    \
 *				   (arg))
 */
#define typesafe_cb_postargs(rtype, atype, fn, arg, ...)		\
	typesafe_cb_cast(rtype (*)(atype, __VA_ARGS__),			\
			 rtype (*)(__typeof__(arg), __VA_ARGS__),	\
			 (fn))

#endif //MY_DATABASE_TYPESAFE_CB_H
