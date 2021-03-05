/* Debug/db_log.h
 * a simple log system
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/9
 */

#ifndef MY_DATABASE_DB_LOG_H
#define MY_DATABASE_DB_LOG_H


#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
typedef  char* error_t;
#define get_line __LINE__
#define get_func __FUNCTION__
#define get_file __FILE__

static const unsigned long long log_file_max_size = 5 * 1024 * 1024;

void print_log(const char *fileName, const int lineNum, const char *funcName, const char *fmt, ...);
void print_log_with(error_t error);

#endif //MY_DATABASE_DB_LOG_H
