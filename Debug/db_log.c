/* Debug/db_log.c
 * a simple log system
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/9
 */

#include "db_log.h"
#include <errors.h>
#include <defs.h>

#define __NO_MINGW_LFS

void print_log(const char *fileName, const int lineNum, const char *funcName, const char *fmt, ...)
{
        FILE *log_file = NULL;
        size_t cur_size = 0;
        int len = 0;
        char path[260] = {0};

        va_list va;
        va_start(va, fmt);

        char *buffer_write = (char *) malloc(sizeof(char) * 10240);
        if (buffer_write == NULL) {
                fprintf(stderr, "%s in [file:%s][function:%s][line:%d]", MEMORY_ALLOCATED_ERROR,__FILE__,__FUNCTION__ ,__LINE__);
                abort();
        }
        memset(buffer_write, 0, sizeof(char) * 10240);


        time_t Time=time(NULL);
        struct tm* t=localtime(&Time);
        len += strftime(buffer_write, 32, "[%Y-%m-%d %H:%M:%S]", t);

        fileName = strrchr(fileName, '\\') + 1;
        len += sprintf(buffer_write + len, "[File:%s]", fileName);
        len += sprintf(buffer_write + len, "[Line:%d]", lineNum);
        len += sprintf(buffer_write + len, "[Function:%s] ", funcName);
        len += vsprintf(buffer_write + len, fmt, va);
        len += sprintf(buffer_write + len, "\n");
#include <unistd.h>
        getcwd(path,260);
        strcat(path, log_file_name);
        log_file = fopen(path, "r");
        if (log_file != NULL) {
                fseek(log_file, 0L, SEEK_END);
                cur_size = ftell(log_file);
                fclose(log_file);
                //TODO : when the log is full just copy & save this file then create a new one.
                if (cur_size > log_file_max_size) {
                        remove(path);
                }
        }
        log_file = fopen(path, "ab+");
        if (log_file == NULL) {
                fprintf(stderr, "%s in [file:%s][function:%s][line:%d]", MEMORY_ALLOCATED_ERROR,__FILE__,__FUNCTION__ ,__LINE__);
        }
        fwrite(buffer_write, len, 1, log_file);
        fclose(log_file);

        if (buffer_write != NULL) {
                free(buffer_write);
                buffer_write = NULL;
        }
}

__always_inline void print_log_with(error_t error)
{
        print_log(get_file,get_line,get_func,error);
}