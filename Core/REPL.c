/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/3/4
 */
#include <REPL.h>
#include <mm.h>
#include <defs.h>
#include <debug.h>
#include <time.h>

InputBuffer *new_input_buffer()
{
        InputBuffer *new_buffer = (InputBuffer *) malloc(sizeof(*new_buffer));
        new_buffer->buffer = nullptr;
        new_buffer->buffer_len = 0;
        new_buffer->input_len = 0;
        return new_buffer;
}

void print_sign(void)
{
        printf("db > ");
}

void read_input(InputBuffer *input_buffer)
{
        ssize_t bytes_read =
                getline(&(input_buffer->buffer), &(input_buffer->buffer_len), stdin);
        if (bytes_read < 0) {
                printf("Error reading input\n");
                print_log_with(READERR);
                exit(EXIT_FAILURE);
        }
        input_buffer->input_len = bytes_read - 1;
        input_buffer->buffer[bytes_read - 1] = '\0';
}

void delete_input_buffer(InputBuffer *inputBuffer)
{
        free(inputBuffer->buffer);
        free(inputBuffer);
}

void print_info(void)
{
        char *buf = ordinary_time();
        printf(""
                "*-----------------------------------------*\n"
                "| SQL-db version 0.1 %s  |\n"
                "| MIT License                             |\n"
                "| https://github.com/Elio-yang/ProjectDB  |\n"
                "| Copyright (c) 2021 Elio-yang            |\n"
                "*-----------------------------------------*\n"
                "Enter \".help\" for usage hints.\n", buf);
}

void print_help(void)
{
        printf("Options:\n"
                "  .open <filename>                     : open & load a .db file\n"
                "  .exit                                : exits SQLdb\n"
                "  .const                               : show related constants\n"
                "  .tree                                : tree visualization\n"
                "  select                               : show all rows\n"
                "  insert <id> <username> <email>       : insert a row record\n"
                );
}
void print_init(void)
{
        printf("Must input a database filename(ended with .db) first.\n");
        printf("filename > :");
}
