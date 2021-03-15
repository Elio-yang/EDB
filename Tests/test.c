#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
// 由于VS-Clang目前不太成熟，
// 因此这里必须让stdnoreturn放在最后包含
#include <stdnoreturn.h>
#ifndef var
#define var     __auto_type
#endif

static void noreturn ProgramTerminate(void) {
	printf("Program terminated...");
	getchar();
	exit(0);
}

static void __attribute__((overloadable)) Foo(void) {
	puts("This is Foo!");
}

static void __attribute__((overloadable)) Foo(int a) {
	printf("Foo a = %d\n", a);
}

int main(void) {
	puts("Hello, Clang!");

	uint32_t a = 100;
	bool b = a > 50;
	printf("b = %d\n", b);

	_Static_assert(sizeof(a) == 4, "Not compatible environment!");

	var s = "Hello, world!";
	printf("The string is: %s\n", s);

	Foo();
	Foo(a);

	ProgramTerminate();
}




