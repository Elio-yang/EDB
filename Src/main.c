/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/3/4
 */

#include <REPL.h>
#include <compile.h>
#include <defs.h>

int main(int argc,char* argv[])
{
        print_info();
        print_init();
        Table *table=load_file();
        logic_repl(table);
}