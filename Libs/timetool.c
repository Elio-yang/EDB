/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/3/4
 */
#include <timetool.h>
char *ordinary_time(void)
{
        char *buf = (char *) malloc(sizeof(*buf * 32));
        time_t Time = time(NULL);
        struct tm *t = localtime(&Time);
        strftime(buf,32, "%Y-%m-%d %H:%M:%S", t);
        return buf;
}