/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/3/5
 */

#ifndef MY_DATABASE_PARSER_H
#define MY_DATABASE_PARSER_H




typedef enum {
        META_COMMAND_SUCCESS,
        META_COMMAND_UNRECOGNIZED_COMMAND
}meta_command_result;

typedef enum {
        PREPARE_SUCCESS,
        PREPAE_UNRECOGNIZED_STATEMENT
}prepare_result;



#endif //MY_DATABASE_PARSER_H
