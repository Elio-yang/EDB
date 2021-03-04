/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/3/4
 */

#include <REPL.h>
#include <defs.h>
#include <string_db.h>

int main(int argc,char* argv[])
{
        print_info();
        InputBuffer *input_buffer=new_input_buffer();
        while (true){
                print_sign();
                read_input(input_buffer);
                if(strcmp(input_buffer->buffer,".exit")==0){
                        delete_input_buffer(input_buffer);
                        exit(EXIT_SUCC);
                }
                else if(strcmp(input_buffer->buffer,".help")==0){
                        print_help();
                }
                else {
                        printf("Unrecognized command '%s' .\n",input_buffer->buffer);
                }
        }
}