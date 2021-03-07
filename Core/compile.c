/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/3/5
 */

#include <compile.h>
#include <defs.h>
#include <stdio.h>
#include <errors.h>
#include <errno.h>
#include <string.h>
#include <debug.h>
#include <mathtool.h>
#include <unistd.h>
#include <fcntl.h>

const uint32_t ID_SIZE = size_of_member(Row, id);
const uint32_t USERNAME_SIZE = size_of_member(Row, username);
const uint32_t EMAIL_SIZE = size_of_member(Row, email);
const uint32_t ID_OFFSET = offsetof(Row, id);
const uint32_t USERNAME_OFFSET = offsetof(Row, username);
const uint32_t EMAIL_OFFSET = offsetof(Row, email);
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;
const uint32_t PAGE_SIZE = 4096;

/* how many rows in one page */
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;

/* how many rows in all pages */
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGE;


Meta_command_result do_meta_command(InputBuffer *in_buf, Table *tb) {
        char *copyinput = (char *) malloc(sizeof(char) * (strlen(in_buf->buffer)));
        char *meta = strtok(copyinput, " ");
        if (strcmp(in_buf->buffer, ".exit") == 0) {
                printf("Thanks for using sql-db.\n");
                db_close(tb);
                delete_input_buffer(in_buf);
                exit(EXIT_SUCCESS);
        } else if (strcmp(in_buf->buffer, ".help") == 0) {
                print_help();
                return META_COMMAND_SUCCESS;
        }
                //TODO: logic of .open
        else if (strcmp(meta, ".open") == 0) {
                char *filename = strtok(NULL, " ");
                if (filename == NULL) {
                        printf("Must supply a database filename.\n");
                        exit(EXIT_FAILURE);
                }
                return META_OPEN_SUCCESS;
        } else {
                return META_COMMAND_UNRECOGNIZED_COMMAND;
        }

}

Prepare_result prepare_insert(InputBuffer *in_buf, Statement *statement) {
        statement->type = STATEMENT_INSERT;
        char *keyword = strtok(in_buf->buffer, " ");
        char *id_string = strtok(NULL, " ");
        char *username = strtok(NULL, " ");
        char *email = strtok(NULL, " ");

        if (keyword == nullptr ||
            id_string == nullptr ||
            username == nullptr ||
            email == nullptr) {
                return PREPARE_SYNTAX_ERROR;
        }
        int id = atoi(id_string);
        if (id < 0) {
                return PREPARE_NEGATIVE_ID;
        }
        if (strlen(username) > COLUMN_USERNAME_SIZE) {
                return PREPARE_STRING_TOO_LONG;
        }
        if (strlen(email) > COLUMN_EMAIL_SIZE) {
                return PREPARE_STRING_TOO_LONG;
        }

        statement->row_to_insert.id = id;
        strcpy(statement->row_to_insert.email, email);
        strcpy(statement->row_to_insert.username, username);
        return PREPARE_SUCCESS;
}

Prepare_result prepare_statement(InputBuffer *in_buf, Statement *statement) {
        /*
         * Note that we use strncmp for “insert” since
         * the “insert” keyword will be followed by data.
         * (e.g. insert 1 cstack foo@bar.com)
         */
        if (strncmp(in_buf->buffer, "insert", 6) == 0) {
                return prepare_insert(in_buf, statement);
        }
        if (strcmp(in_buf->buffer, "select") == 0) {
                statement->type = STATEMENT_SELECT;
                return PREPARE_SUCCESS;
        }
        return PREPARE_UNRECOGNIZED_STATEMENT;
}

void print_sth(char ch, int n) {
        for (int i = 0; i < n; i++) {
                printf("%c", ch);
        }
}

/* simple version row  |id|username|email| */
void print_row(Row *row) {
        char str[20];
        sprintf(str, "%d", row->id);
        int len_id = (int) strlen(str);
        int len_usr = (int) strlen(row->username);
        int len_email = (int) strlen(row->email);
        int default_width = 32;
        int left1 = default_width - len_id;
        int left2 = default_width - len_usr;
        int left3 = default_width - len_email;
        int b_l1 = left1 / 2;
        int b_l2 = left2 / 2;
        int b_l3 = left3 / 2;
        if (b_l1 > 0) {
                printf("|");
                print_sth(' ', b_l1);
                printf("%s", str);
                int left = default_width - b_l1 - len_id;
                if (left > 0) {
                        print_sth(' ', left);
                }
                printf("|");
        } else {
                printf("|");
                if (left1 == 1) {
                        print_sth(' ', 1);
                }
                printf("%s", str);
                printf("|");

        }
        if (b_l2 > 0) {
                print_sth(' ', b_l2);
                printf("%s", row->username);
                int left = default_width - b_l2 - len_usr;
                if (left > 0) {
                        print_sth(' ', left);
                }
                printf("|");
        } else {
                if (left2 == 1) {
                        print_sth(' ', 1);
                }
                printf("%s", row->username);
                printf("|");
        }
        if (b_l3 > 0) {
                print_sth(' ', b_l3);
                printf("%s", row->email);
                int left = default_width - b_l3 - len_email;
                if (left > 0) {
                        print_sth(' ', left);
                }
                printf("|");
        } else {
                if (left3 == 1) {
                        print_sth(' ', 1);
                }
                printf("%s", row->email);
                printf("|");
        }
        printf("\n");
        print_sth('-',100);
        printf("\n");
}

Execute_result execute_insert(Statement *statement, Table *tb) {
        if (tb->num_rows >= TABLE_MAX_ROWS) {
                return EXECUTE_TABLE_FULL;
        }
        Row *row_to_insert = &(statement->row_to_insert);
        Cursor *cursor = table_end(tb);
        serialize_row(row_to_insert, cursor_value(cursor));
        tb->num_rows++;
        free(cursor);
        return EXECUTE_SUCCESS;
}

Execute_result execute_select(Statement *statement, Table *tb) {
        system("clear");
        Cursor *cursor = table_start(tb);
        Row row;
        print_sth('=', 100);
        printf("\n");
        {
                printf("||");
                print_sth(' ', 15 - 1);
                printf("id");
                print_sth(' ', 15 - 1);
                printf("||");

                print_sth(' ', 12);
                printf("username");
                print_sth(' ', 12 - 1);
                printf("||");

                print_sth(' ', 13);
                printf("email");
                print_sth(' ', 14 - 1);
                printf("||\n");
        }
        {
                print_sth('=', 100);
                printf("\n");
        }
        while (!(cursor->EOT)) {
                deserialize_row(cursor_value(cursor), &row);
                print_row(&row);
                cursor_advance(cursor);
        }
        return EXECUTE_SUCCESS;
}

Execute_result execute_statement(Statement *statement, Table *tb) {
        switch (statement->type) {
                case STATEMENT_INSERT:
                        return execute_insert(statement, tb);
                case STATEMENT_SELECT:
                        return execute_select(statement, tb);
        }
        return UNKOWN;
}

void serialize_row(Row *src, void *dest) {
        memcpy(dest + ID_OFFSET, &(src->id), ID_SIZE);
        memcpy(dest + USERNAME_OFFSET, &(src->username), USERNAME_OFFSET);
        memcpy(dest + EMAIL_OFFSET, &(src->email), EMAIL_SIZE);

}

void deserialize_row(void *src, Row *dest) {
        memcpy(&(dest->id), src + ID_OFFSET, ID_SIZE);
        memcpy(&(dest->username), src + USERNAME_OFFSET, USERNAME_SIZE);
        memcpy(&(dest->email), src + EMAIL_OFFSET, EMAIL_SIZE);
}

void *get_page(Page_pool *page_mu, uint32_t page_id) {
        /* 页序号超出了最大可支持的页数 */
        if (page_id > TABLE_MAX_ROWS) {
                printf("Tried to fetch page number out of bound.%d > %d\n",
                       page_id, TABLE_MAX_ROWS);
                exit(EXIT_FAILURE);
        }
        /* 该页未使用 */
        if (page_mu->pages[page_id] == nullptr) {
                /* 申请新的一页 */
                void *page = malloc(PAGE_SIZE);
                /* 当前文件代表的数据库表占了多少页 */
                uint32_t num_pages = page_mu->file_len / PAGE_SIZE;
                /* 可能在文件末尾需要保存部分页 */
                if (page_mu->file_len % PAGE_SIZE) {
                        ++num_pages;
                }

                if (page_id <= num_pages) {
                        /*
                         * 文件的偏移量设置到距离文件开始 page_id*PAGE_SIZE 处
                         * 即到了 page_id 页开始存储的数据处
                         */
                        lseek(page_mu->fd, page_id * PAGE_SIZE, SEEK_SET);

                        /* 从当前位置读取一个PAGE_SIZE大小到page中 */
                        ssize_t bytes_read = read(page_mu->fd, page, PAGE_SIZE);
                        if (bytes_read == -1) {
                                printf("Error reading file : %d\n", errno);
                                exit(EXIT_FAILURE);
                        }
                }
                /* 设置page_pool 的映射关系 */
                page_mu->pages[page_id] = page;
        }
        return page_mu->pages[page_id];
}

void *cursor_value(Cursor *cursor) {
        /* 在第几页 */
        uint32_t row_num = cursor->row_id;
        uint32_t page_id = row_num / ROWS_PER_PAGE;

        void *page = get_page(cursor->table->page_mu, page_id);
        uint32_t row_offset = row_num % ROWS_PER_PAGE;
        uint32_t byte_offset = row_offset * ROW_SIZE;
        /* return the pointer of this row */
        return page + byte_offset;
}

Page_pool *page_pool_open(const char *filename) {
        /*
         * 对于二进制文件的打开与查看，可以使用vim :!xxd
         */
        /*
         * O_RDWR 读写打开
         * O_CREAT 若文件不存在则创建，需要制定mode
         * S_IWUSR|S_IRUSR :Set ID read&write by owner  -rw- --- --- 即0600
         */
        int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
        if (fd == -1) {
                printf("Unable to open file.\n");
                print_log_with(FILE_OPEN_ERROR);
        }
        build_assert(fd != -1);
        /* 该偏移量即为文件长度 */
        off_t file_len = lseek(fd, 0, SEEK_END);
        /* 页池: 管理当前文件与其所有的页 */
        Page_pool *page_mu = malloc(sizeof(Page_pool));
        page_mu->file_len = file_len;
        page_mu->fd = fd;
        /* 初始化page_pool */
        for (uint32_t i = 0; i < TABLE_MAX_PAGE; i++) {
                page_mu->pages[i] = nullptr;
        }
        return page_mu;
}

/* open the database file */
Table *db_open(const char *filename) {
        Page_pool *page_mu = page_pool_open(filename);
        /* 数据库.db文件包含多少个row */
        uint32_t num_rows = page_mu->file_len / ROW_SIZE;
        Table *table = malloc(sizeof(Table));
        if (table == nullptr) {
                print_log_with(MEMORY_ALLOCATED_ERROR);

        }

        table->num_rows = num_rows;
        table->page_mu = page_mu;
        /* 根据文件返回一个数据库表 */
        return table;
}

/* flush 来存到磁盘上，实现永久化，在close时调用 */
void pager_flush(Page_pool *page_mu, uint32_t page_id, uint32_t size) {
        if (page_mu->pages[page_id] == nullptr) {
                printf("Tried to flush null page.\n");
                exit(EXIT_FAILURE);
        }
        /*
         * 文件的偏移量设置到距离文件开始 page_id*PAGE_SIZE 处
         * 即到了 page_id 页开始存储的数据处
         */
        off_t offset = lseek(page_mu->fd, page_id * PAGE_SIZE, SEEK_SET);
        if (offset == -1) {
                printf("Error seeking: %d\n", errno);
                exit(EXIT_FAILURE);
        }
        /* 将这一页写到文件中去 */
        ssize_t bytes_written =
                write(page_mu->fd, page_mu->pages[page_id], size);
        if (bytes_written == -1) {
                printf("Error writing: %d\n", errno);
                exit(EXIT_FAILURE);
        }
}

void db_close(Table *tb) {
        Page_pool *page_mu = tb->page_mu;
        /* 占用了多少页 */
        uint32_t num_full_pages = tb->num_rows / ROWS_PER_PAGE;
        for (uint32_t i = 0; i < num_full_pages; i++) {
                if (page_mu->pages[i] == nullptr) {
                        continue;
                }
                /* 该页使用过，写回磁盘 */
                pager_flush(page_mu, i, PAGE_SIZE);
                free(page_mu->pages[i]);
                page_mu->pages[i] = nullptr;
        }
        /* 文件末尾需要的部分页 see: get_page() 使用B树以后不需要这个 */
        uint32_t num_additional_rows = tb->num_rows % ROWS_PER_PAGE;
        if (num_additional_rows > 0) {
                uint32_t page_num = num_full_pages;
                if (page_mu->pages[page_num] != nullptr) {
                        pager_flush(page_mu, page_num,
                                    num_additional_rows * ROW_SIZE);
                        free(page_mu->pages[page_num]);
                        page_mu->pages[page_num] = nullptr;
                }
        }
        /*
         * 关闭当前.db文件，在此前，该文件一直保持打开，
         * 其文件描述符并保存在page_pool中
         */
        int result = close(page_mu->fd);
        if (result == -1) {
                printf("Error closing sql-db file.\n");
                exit(EXIT_FAILURE);
        }
        for (uint32_t i = 0; i < TABLE_MAX_PAGE; i++) {
                void *page = page_mu->pages[i];
                if (page) {
                        free(page);
                        page_mu->pages[i] = nullptr;
                }
        }
        free(page_mu);
        free(tb);
}

Table *load_file(void) {
        size_t name_len;
        char *Filename = (char *) malloc(sizeof(char) * MAX_FILENAME_LEN);
        if (Filename == nullptr) {
                print_log_with(MEMORY_ALLOCATED_ERROR);
                exit(EXIT_FAILURE);
        }
        getline(&Filename, &name_len, stdin);
        char *file = strtok(Filename, " ");
        size_t len=strlen(file);
        *(file+len-1)='\0';
        if (file == nullptr) {
                printf("Must supply a database filename.\n");
        }
        Table *table = db_open(file);
        free(Filename);
        return table;
}

void logic_repl(Table *tb) {
        InputBuffer *input_buffer = new_input_buffer();
        while (true) {
                print_sign();
                read_input(input_buffer);
                /* started with '.' called meta command */
                if (input_buffer->buffer[0] == '.') {
                        switch (do_meta_command(input_buffer, tb)) {
                                case META_OPEN_SUCCESS:
                                        //TODO
                                case (META_COMMAND_SUCCESS):
                                        continue;
                                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                                        printf("Unrecognized command '%s'\n", input_buffer->buffer);
                                        continue;
                                default:
                                        continue;
                        }
                }
                Statement statement;
                switch (prepare_statement(input_buffer, &statement)) {
                        case (PREPARE_SUCCESS):
                                break;
                        case (PREPARE_UNRECOGNIZED_STATEMENT):
                                printf("Unrecognized keyword at start of '%s'.\n ", input_buffer->buffer);
                                continue;
                        case PREPARE_SYNTAX_ERROR:
                                printf("Syntax error.Could not parse statement.\n");
                                continue;
                        case PREPARE_STRING_TOO_LONG:
                                printf("String too long.\n");
                                continue;
                        case PREPARE_NEGATIVE_ID:
                                printf("ID must be positive.\n");
                                continue;
                        default:
                                continue;
                }
                switch (execute_statement(&statement, tb)) {
                        case EXECUTE_SUCCESS:
                                //printf("EXECUTE_SUCCESS\n");
                                break;
                        case EXECUTE_TABLE_FULL:
                                printf("Error:Table full .\n");
                                break;
                        default:
                                break;
                }
        }
}

Cursor *table_start(Table *tb) {
        Cursor *cursor = malloc(sizeof(*cursor));
        cursor->table = tb;
        cursor->row_id = 0;
        cursor->EOT = (tb->num_rows == 0);
        return cursor;
}

Cursor *table_end(Table *tb) {
        Cursor *cursor = malloc(sizeof(*cursor));
        cursor->table = tb;
        cursor->row_id = tb->num_rows;
        cursor->EOT = true;
        return cursor;
}

void cursor_advance(Cursor *cursor) {
        ++cursor->row_id;
        if (cursor->row_id >= cursor->table->num_rows) {
                cursor->EOT = true;
        }
}