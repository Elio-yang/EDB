/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/3/4
 */

#include <REPL.h>
#include <compile.h>
#include <defs.h>
#include <string_db.h>

int main(int argc,char* argv[])
{
        print_info();
        Table *table= db_open();
        InputBuffer *input_buffer=new_input_buffer();
        while (true){
                print_sign();
                read_input(input_buffer);
                /* started with '.' called meta command */
                if (input_buffer->buffer[0]=='.'){
                        switch (do_meta_command(input_buffer)) {
                                case (META_COMMAND_SUCCESS):
                                        continue;
                                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                                        printf("Unrecognized command '%s'\n",input_buffer->buffer);
                                        continue;
                        }
                }
                Statement statement;
                switch (prepare_statement(input_buffer,&statement)) {
                        case (PREPARE_SUCCESS):
                                break;
                        case (PREPARE_UNRECOGNIZED_STATEMENT):
                                printf("Unrecognized keyword at start of '%s'.\n ",input_buffer->buffer);
                                continue;
                        case PREPARE_SYNTAX_ERROR:
                                printf("Syntax error.Could not parse statement.\n");
                                continue;
                        case PREPARE_STRING_TOO_LONG:
                                printf("String too long.\n");
                                continue;
                        case PREPARE_NEGATIVE_ID:
                                printf("ID must be positive.\n");
                                continue;
                }
                switch (execute_statement(&statement,table)) {
                        case EXECUTE_SUCCESS:
                                //printf("EXECUTE_SUCCESS\n");
                                break;
                        case EXECUTE_TABLE_FULL:
                                printf("Error:Table full .\n");
                                break;
                }
        }
}