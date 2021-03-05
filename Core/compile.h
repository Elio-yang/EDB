/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/3/5
 */

#ifndef MY_DATABASE_PARSER_H
#define MY_DATABASE_PARSER_H


#include <REPL.h>
#include <defs.h>


typedef enum {
        META_COMMAND_SUCCESS,
        META_COMMAND_UNRECOGNIZED_COMMAND
} Meta_command_result;

typedef enum {
        PREPARE_SUCCESS,
        PREPARE_UNRECOGNIZED_STATEMENT,
        PREPARE_SYNTAX_ERROR,
        PREPARE_STRING_TOO_LONG,
        PREPARE_NEGATIVE_ID
} Prepare_result;

/*
 *----------*----------*---------*
 |    id    | username |  email  |
 *----------*----------*---------*
 */
typedef struct {
        uint32_t id;
        char username[COLUMN_USERNAME_SIZE+1];
        char email[COLUMN_EMAIL_SIZE+1];
} Row;

typedef enum {
        STATEMENT_INSERT,
        STATEMENT_SELECT
} Statement_type;

typedef enum {
        EXECUTE_SUCCESS,EXECUTE_TABLE_FULL
}Execute_result;

typedef struct {
        Statement_type type;
        //only used when type is insert
        Row row_to_insert;
} Statement;

typedef struct {
        int fd;
        uint32_t file_len;
        /* all 'pages' stored in array */
        void *pages[TABLE_MAX_PAGE];
} Pager;
typedef struct {
        Pager *pager;
        uint32_t num_rows;
} Table;



Meta_command_result do_meta_command(InputBuffer *input_buffer,Table *table);

Prepare_result prepare_statement(InputBuffer *input_buffer, Statement *statement);

Prepare_result prepare_insert(InputBuffer *input_buffer,Statement *statement);

Execute_result execute_insert(Statement *statement,Table *table);

Execute_result execute_select(Statement *statement,Table *table);

Execute_result  execute_statement(Statement *statement,Table *table);

void serialize_row(Row *source, void *destination);

void deserialize_row(void *source, Row *destination);

void *row_slot(Table *table,uint32_t row_num);

void print_row(Row * row);

Pager *pager_open(const char *filename);

Table *db_open(const char *filename);

void *delete_table(Table *table);

#endif //MY_DATABASE_PARSER_H
