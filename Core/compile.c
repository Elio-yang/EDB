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
#include <unistd.h>
#include <fcntl.h>

/*---------------------------------------------------------------------------*/
const uint32_t ID_SIZE = size_of_member(Row, id);
const uint32_t USERNAME_SIZE = size_of_member(Row, username);
const uint32_t EMAIL_SIZE = size_of_member(Row, email);
const uint32_t ID_OFFSET = offsetof(Row, id);
const uint32_t USERNAME_OFFSET = offsetof(Row, username);
const uint32_t EMAIL_OFFSET = offsetof(Row, email);
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;
const uint32_t PAGE_SIZE = 4096;
/*---------------------------------------------------------------------------*/

/*
   Page(leaf node) Layout
 *----------------------*
 | node_type        1B  |----
 * ---------------------*   |
 | is_root          1B  |   |------> common node header
 * ---------------------*   |
 | parent_pointer   4B  |---|
 * ---------------------*
 | num_cell         4B  |
 * ---------------------*
 | key0             4B  |
 * ---------------------*
 | value0         293B  |
 * ---------------------*
 |          ......      |
 * ---------------------*
 | key12            4B  |
 * ---------------------*
 | value12        293B  |
 * ---------------------*
 | wasted space   225B  |
 * ---------------------*
        total    4096B

common node header layout
*/
const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const uint32_t NODE_TYPE_OFFSET = 0;
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
const uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const uint8_t COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;
/* leaf node header layout */
const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;
/* leaf node body layout */
const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;
const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_VALUE_OFFSET =LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const uint32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;
/*---------------------------------------------------------------------------*/

/* how many cells are contained in this page currently */
uint32_t *leaf_node_num_cells(void *node)
{
        return node+LEAF_NODE_NUM_CELLS_OFFSET;
}
/* position <k,v> by cell_num */
void *leaf_node_cell(void *node,uint32_t cell_num)
{
        return node+LEAF_NODE_HEADER_SIZE+cell_num*LEAF_NODE_CELL_SIZE;
}

uint32_t *leaf_node_key(void *node,uint32_t cell_num)
{
        /* change the way of interpret the pointer */
        return leaf_node_cell(node,cell_num);
}

uint32_t *leaf_node_value(void *node,uint32_t cell_num)
{
        return leaf_node_cell(node,cell_num)+LEAF_NODE_KEY_SIZE;
}

void initialize_leaf_node(void *node)
{
        *leaf_node_num_cells(node)=0;
}

void leaf_node_insert(Cursor *cursor,uint32_t key,Row *value)
{
        void *node=get_page(cursor->table->page_mu,cursor->page_num);
        uint32_t num_cells=*leaf_node_num_cells(node);
        if(num_cells>=LEAF_NODE_MAX_CELLS){
                printf("Need to implement splitting a leaf node.\n");
                exit(EXIT_FAILURE);
        }
        if(cursor->cell_num < num_cells){
                for(uint32_t i=num_cells;i>cursor->cell_num;i--){
                        memcpy(leaf_node_cell(node,i),leaf_node_cell(node,i-1),LEAF_NODE_CELL_SIZE);
                }
        }
        ++(*leaf_node_num_cells(node));
        *(leaf_node_key(node,cursor->cell_num))=key;
        serialize_row(value,leaf_node_value(node,cursor->cell_num));
}
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
void print_const(void)
{
        printf("ROW_SIZE: %d\n", ROW_SIZE);
        printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
        printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
        printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
        printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
        printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}

void print_tree(void *node)
{
        uint32_t num_cells=*leaf_node_num_cells(node);
        printf("leaf(size %d)\n",num_cells);
        for(uint32_t i=0;i<num_cells;i++){
                uint32_t key=*leaf_node_key(node,i);
                printf("  - %d : %d\n", i, key);
        }
}

Meta_command_result do_meta_command(InputBuffer *in_buf, Table *tb) {
        char *copyinput = (char *) malloc(sizeof(char) * (strlen(in_buf->buffer)));
        char *meta = strtok(copyinput, " ");
        if (strcmp(in_buf->buffer, ".exit") == 0) {
                printf("Thanks for using sql-db.\n");
                db_close(tb);
                delete_input_buffer(in_buf);
                exit(EXIT_SUCCESS);
        }
        else if (strcmp(in_buf->buffer, ".help") == 0) {
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
        }
        else if(strcmp(in_buf->buffer,".const")==0){
                printf("Constants:\n");
                print_const();
                return META_COMMAND_SUCCESS;

        }
        else if(strcmp(in_buf->buffer,".tree")==0){
                printf("Tree:\n");
                print_tree(get_page(tb->page_mu,0));
                return META_COMMAND_SUCCESS;
        }
        else {
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
        print_sth('-', 100);
        printf("\n");
}

Execute_result execute_insert(Statement *statement, Table *tb) {
        void *node=get_page(tb->page_mu,tb->root_page_num);
        if(*(leaf_node_num_cells(node)) >= LEAF_NODE_MAX_CELLS){
                return EXECUTE_TABLE_FULL;
        }

        Row *row_to_insert = &(statement->row_to_insert);
        Cursor *cursor = table_end(tb);
        leaf_node_insert(cursor,row_to_insert->id,row_to_insert);
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
/*---------------------------------------------------------------------------*/

void *get_page(Page_pool *page_mu, uint32_t page_id) {
        /* 页序号超出了最大可支持的页数 */
        if (page_id > TABLE_MAX_PAGE) {
                printf("Tried to fetch page number out of bound.%d > %d\n",
                       page_id, TABLE_MAX_PAGE);
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
                if(page_id>=page_mu->num_pages){
                        page_mu->num_pages=page_id+1;
                }

        }
        return page_mu->pages[page_id];
}

void *cursor_value(Cursor *cursor) {
        /* 在第几页 */
        uint32_t page_id=cursor->page_num;
        void *page = get_page(cursor->table->page_mu, page_id);
        return leaf_node_value(page,cursor->cell_num);
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
        page_mu->num_pages=(file_len/PAGE_SIZE);
        if(file_len%PAGE_SIZE!=0){
                printf(".db file is not a whole number of pages. Corrupt file.\n");
                exit(EXIT_FAILURE);
        }
        /* 初始化page_pool */
        for (uint32_t i = 0; i < TABLE_MAX_PAGE; i++) {
                page_mu->pages[i] = nullptr;
        }
        return page_mu;
}

/* open the database file */
Table *db_open(const char *filename) {
        Page_pool *page_mu = page_pool_open(filename);
        Table *table = malloc(sizeof(Table));
        if (table == nullptr) {
                print_log_with(MEMORY_ALLOCATED_ERROR);

        }
        table->root_page_num=0;
        table->page_mu = page_mu;
        /* new db file */
        if(page_mu->num_pages==0){
                void *root_node=get_page(page_mu,0);
                initialize_leaf_node(root_node);
        }

        /* 根据文件返回一个数据库表 */
        return table;
}

/* flush 来存到磁盘上，实现永久化，在close时调用 store a page each time*/
void pager_flush(Page_pool *page_mu, uint32_t page_id) {
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
                write(page_mu->fd, page_mu->pages[page_id], PAGE_SIZE);
        if (bytes_written == -1) {
                printf("Error writing: %d\n", errno);
                exit(EXIT_FAILURE);
        }
}

void db_close(Table *tb) {
        Page_pool *page_mu = tb->page_mu;
        /* 占用了多少页 */
        for (uint32_t i = 0; i < page_mu->num_pages ; i++) {
                if (page_mu->pages[i] == nullptr) {
                        continue;
                }
                /* 该页使用过，写回磁盘 */
                pager_flush(page_mu, i);
                free(page_mu->pages[i]);
                page_mu->pages[i] = nullptr;
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
        size_t len = strlen(file);
        *(file + len - 1) = '\0';
        if (file == nullptr) {
                printf("Must supply a database filename.\n");
        }
        Table *table = db_open(file);
        free(Filename);
        return table;
}
/*---------------------------------------------------------------------------*/
Cursor *table_start(Table *tb) {
        Cursor *cursor = malloc(sizeof(*cursor));
        cursor->table = tb;
        cursor->page_num=tb->root_page_num;
        cursor->cell_num=0;
        void *root_node=get_page(tb->page_mu,tb->root_page_num);
        uint32_t num_cells=*leaf_node_num_cells(root_node);
        cursor->EOT=(num_cells==0);
        return cursor;
}

Cursor *table_end(Table *tb) {
        Cursor *cursor = malloc(sizeof(*cursor));
        cursor->table = tb;
        cursor->page_num=tb->root_page_num;
        cursor->EOT = true;
        void *root_node=get_page(tb->page_mu,tb->root_page_num);
        uint32_t num_cells=*leaf_node_num_cells(root_node);
        cursor->cell_num=num_cells;
        return cursor;
}

void cursor_advance(Cursor *cursor) {
        uint32_t page_num=cursor->page_num;
        void *node=get_page(cursor->table->page_mu,page_num);
        ++cursor->cell_num;
        if(cursor->cell_num >=(*leaf_node_num_cells(node))){
                cursor->EOT=true;
        }
}
/*---------------------------------------------------------------------------*/
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