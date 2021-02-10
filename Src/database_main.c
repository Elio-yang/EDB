/*
 * database_main.c
 * The main interface of the whole project
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/9
 */

#include <adlist.h>
#include <db_log.h>
#include <print_color.h>
#include <unistd.h>
int main(void)
{

        printf("This is a character control test!\n" );
        sleep(3);
        printf("[%2u]" CLEAR "CLEAR\n" NONE, __LINE__);

        printf("[%2u]" BLACK "BLACK " L_BLACK "L_BLACK\n" NONE, __LINE__);
        printf("[%2u]" RED "RED " L_RED "L_RED\n" NONE, __LINE__);
        printf("[%2u]" GREEN "GREEN " L_GREEN "L_GREEN\n" NONE, __LINE__);
        printf("[%2u]" BROWN "BROWN " YELLOW "YELLOW\n" NONE, __LINE__);
        printf("[%2u]" BLUE "BLUE " L_BLUE "L_BLUE\n" NONE, __LINE__);
        printf("[%2u]" PURPLE "PURPLE " L_PURPLE "L_PURPLE\n" NONE, __LINE__);
        printf("[%2u]" CYAN "CYAN " L_CYAN "L_CYAN\n" NONE, __LINE__);
        printf("[%2u]" GRAY "GRAY " WHITE "WHITE\n" NONE, __LINE__);

        printf("[%2u]\e[1;31;40m Red \e[0m\n",  __LINE__);

        printf("[%2u]" BOLD "BOLD\n" NONE, __LINE__);
        printf("[%2u]" UNDERLINE "UNDERLINE\n" NONE, __LINE__);
        printf("[%2u]" BLINK "BLINK\n" NONE, __LINE__);
        printf("[%2u]" REVERSE "REVERSE\n" NONE, __LINE__);
        printf("[%2u]" HIDE "HIDE\n" NONE, __LINE__);

        printf("Cursor test begins!\n" );
        printf("=======!\n" );
        sleep(10);
        printf("[%2u]" "\e[2ACursor up 2 lines\n" NONE, __LINE__);
        sleep(10);
        printf("[%2u]" "\e[2BCursor down 2 lines\n" NONE, __LINE__);
        sleep(5);
        printf("[%2u]" "\e[?25lCursor hide\n" NONE, __LINE__);
        sleep(5);
        printf("[%2u]" "\e[?25hCursor display\n" NONE, __LINE__);
        sleep(5);

        printf("Test ends!\n" );
        sleep(3);
        printf("[%2u]" "\e[2ACursor up 2 lines\n" NONE, __LINE__);
        sleep(5);
        printf("[%2u]" "\e[KClear from cursor downward\n" NONE, __LINE__);

        return 0 ;
}
