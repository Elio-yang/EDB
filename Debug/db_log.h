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


const unsigned long long log_file_max_size = 5 * 1024 * 1024;
const char * log_file_name = "\\log.txt";

void print_log(const char *fileName, const int lineNum, const char *funcName, const char *fmt, ...);

#endif //MY_DATABASE_DB_LOG_H
