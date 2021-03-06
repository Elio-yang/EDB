/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/3/4
 */

#ifndef MY_DATABASE_REPL_H
#define MY_DATABASE_REPL_H
#include <stdio.h>
#include <mm.h>
#include <timetool.h>

typedef struct {
        char *buffer;
        size_t buffer_len;
        ssize_t input_len;
}InputBuffer;
InputBuffer *new_input_buffer(void);
void delete_input_buffer(InputBuffer *inputBuffer);
void read_input(InputBuffer *input_buffer);
void print_help(void);
void print_sign(void);
void print_info(void);
void print_init(void);



#endif //MY_DATABASE_REPL_H
