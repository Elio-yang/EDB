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
        META_COMMAND_UNRECOGNIZED_COMMAND,
        META_OPEN_SUCCESS
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
        EXECUTE_SUCCESS,EXECUTE_TABLE_FULL,UNKOWN
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
} Page_pool;

typedef struct {
        /* page management unit */
        Page_pool *page_mu;
        /* 有多少个row */
        uint32_t num_rows;
} Table;

typedef struct {
        Table *table;
        uint32_t row_id;
        /*End Of Table*/
        bool EOT;
}Cursor;



Meta_command_result do_meta_command(InputBuffer *in_buf, Table *tb);

Prepare_result prepare_statement(InputBuffer *in_buf, Statement *statement);

Prepare_result prepare_insert(InputBuffer *in_buf, Statement *statement);

Execute_result execute_insert(Statement *statement,Table *tb);

Execute_result execute_select(Statement *statement,Table *tb);

Execute_result  execute_statement(Statement *statement,Table *tb);

void serialize_row(Row *src, void *dest);

void deserialize_row(void *src, Row *dest);

void *cursor_value(Cursor *cursor);

void print_row(Row * row);

Page_pool *page_pool_open(const char *filename);

Table *db_open(const char *filename);

void db_close(Table *tb);

void pager_flush(Page_pool *page_mu, uint32_t page_id, uint32_t size);

void *get_page(Page_pool *page_mu, uint32_t page_id);

Table *load_file(void);

void logic_repl(Table *tb);

Cursor *table_start(Table * tb);

Cursor *table_end(Table *tb);

void cursor_advance(Cursor *cursor);

#endif //MY_DATABASE_PARSER_H
