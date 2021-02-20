#include "typesafe_cb.h"
#include <stdio.h>
#include <stdlib.h>

/*callback函数链*/
struct callback{
	struct callback *next;
	int value;
	int(*callback)(int value,void *arg);
	void *arg;
};

static struct callback *callbacks;

/*给定值与参数注册回调函数*/
static void _register_callback(int value,int(*cb)(int,void*),void *arg)
{
	struct callback *new=malloc(sizeof(*new));
	new->next=callbacks;
	new->value=value;
	new->callback=cb;
	new->arg=arg;
	callbacks=new;
}

/*
 * exp_t:int(*)(__VA_ARGS__,void*)
 * allow_t:int(*)(__VA_ARGS__,__typeof__(arg))
 */
#define register_callback(value,cb,arg)\
	_register_callback(value,\
			typesafe_cb_preargs(int ,void*,(cb),(arg),int),(arg))

/*通过value,查找callback函数*/
static struct callback *find_callback(int value)
{
	struct callback * i;
	for(i=callbacks;i;i=i->next){
		if(i->value==value){
			return i;
		}
	}
	return NULL;
}

/* 定义回调函数的宏，注意此处并没有用void*指针
 * 回调函数的类型
 * int(*cb)(int,int*arg)
 * */
#define def_callback(name,op)\
	static int name(int val,int *arg)\
	{\
		printf("%s",#op);\
		return val op *arg;\
	}	

def_callback(multiply,*);
def_callback(add, +);
def_callback(divide, /);
def_callback(sub, -);
def_callback(or,|);
def_callback(and,&);
def_callback(xor, ^);
def_callback(assign, =);

int main(int argc,char*argv[])
{
	int i,run=1,num= argc > 1 ? atoi(argv[1]) : 0 ;
	for (i = 1; i < 1024;) {
	                 /*
                          * replacement: _register_callback(i++, (int (*)(int, void *)) (((add))), (&run))
                          * exp_t:   int (*)(int, void *)
                          * allow_t: int (*)(int,__typeof__(arg))--->int (*)(int,int*)
                          * cb_t:    int (*)(int,int*)
                          */
 			register_callback(i++, add, &run);
 			register_callback(i++, divide, &run);
 			register_callback(i++, sub, &run);
 			register_callback(i++, multiply, &run);
 			register_callback(i++, or, &run);
 			register_callback(i++, and, &run);
 			register_callback(i++, xor, &run);
 			register_callback(i++, assign, &run);
 		}

 		printf("%i ", num);
 		while (run < 56) {
 			struct callback *cb = find_callback(num % i);
 			if (!cb) {
 				printf("-> STOP\n");
 				return 1;
 			}
 			num = cb->callback(num, cb->arg);
 			printf("->%i ", num);
 			run++;
 		}
 		printf("-> Winner!\n");
 		return 0;
}
