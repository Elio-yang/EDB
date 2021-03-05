/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/3/5
 */

#include <compile.h>
#include <cstddef>
#include <stdio.h>
#include <errors.h>
#include <errno.h>
#include <string.h>
#include <debug.h>
#include <mathtool.h>
#include <unistd.h>
#include <fcntl.h>
typedef struct ROW ROW;
const uint32_t ID_SIZE=size_of_member(Row, id);
const uint32_t USERNAME_SIZE= size_of_member(Row,username);
const uint32_t EMAIL_SIZE= size_of_member(Row,email);
const uint32_t ID_OFFSET= offsetof(Row,id);
const uint32_t USERNAME_OFFSET=offsetof(Row,username);
const uint32_t EMAIL_OFFSET=offsetof(Row,email);
const uint32_t ROW_SIZE= ID_SIZE+USERNAME_SIZE+EMAIL_SIZE;
const uint32_t  PAGE_SIZE = 4096;
/* how many rows in one page */
const uint32_t ROWS_PER_PAGE = PAGE_SIZE/ROW_SIZE;
/* how many rows in all pages */
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGE;


Meta_command_result do_meta_command(InputBuffer *input_buffer,Table *table)
{
        if(strcmp(input_buffer->buffer,".exit")==0){
                db_close(table);
                exit(EXIT_SUCCESS);
        }
        else if(strcmp(input_buffer->buffer,".help")==0){
                print_help();
        }
        else{
                return META_COMMAND_UNRECOGNIZED_COMMAND;
        }
}
Prepare_result prepare_insert(InputBuffer *input_buffer,Statement *statement)
{
        statement->type=STATEMENT_INSERT;
        char *keyword=strtok(input_buffer->buffer," ");
        char *id_string=strtok(NULL," ");
        char *username=strtok(NULL," ");
        char *email=strtok(NULL," ");

        if(id_string==nullptr||
                username==nullptr||
                email==nullptr){
                return PREPARE_SYNTAX_ERROR;
        }
        int id=atoi(id_string);
        if(id<0){
                return PREPARE_NEGATIVE_ID;
        }
        if(strlen(username)>COLUMN_USERNAME_SIZE){
                return PREPARE_STRING_TOO_LONG;
        }
        if(strlen(email)>COLUMN_EMAIL_SIZE){
                return PREPARE_STRING_TOO_LONG;
        }

        statement->row_to_insert.id=id;
        strcpy(statement->row_to_insert.email,email);
        strcpy(statement->row_to_insert.username,username);
        return PREPARE_SUCCESS;
}

Prepare_result prepare_statement(InputBuffer*input_buffer,Statement* statement)
{
        /*
         * Note that we use strncmp for “insert” since
         * the “insert” keyword will be followed by data.
         * (e.g. insert 1 cstack foo@bar.com)
         */
        if(strncmp(input_buffer->buffer,"insert",6)==0){
                return prepare_insert(input_buffer,statement);
        }
        if(strcmp(input_buffer->buffer,"select")==0){
                statement->type=STATEMENT_SELECT;
                return PREPARE_SUCCESS;
        }
        return PREPARE_UNRECOGNIZED_STATEMENT;
}

void print_balnk(int n)
{
        for(int i=0;i<n;i++){
                printf(" ");
        }
}
void print_line(int n)
{
        for (int i = 0; i < n; i++) {
                printf("-");
        }
}

/* simple version row  |id|username|email| */
void print_row(Row * row)
{
        char str[20];
        sprintf(str,"%d",row->id);
        int len_id=strlen(str);
        int len_usr=strlen(row->username);
        int len_email=strlen(row->email);


        int default_width=32;

        int left1=default_width - len_id;
        int left2=default_width - len_usr;
        int left3=default_width - len_email;
        int b_l1= left1 / 2;
        int b_l2= left2 / 2;
        int b_l3= left3 / 2;


        if(b_l1>0){
                printf("|");
                print_balnk(b_l1);
                printf("%s",str);
                int left=default_width-b_l1-len_id;
                if(left>0){
                        print_balnk(left);
                }
                printf("|");
        }else{
                printf("|");
                if(left1==1){
                        print_balnk(1);
                }
                printf("%s",str);
                printf("|");

        }

        if(b_l2>0){
                print_balnk(b_l2);
                printf("%s",row->username);
                int left=default_width-b_l2-len_usr;
                if(left>0){
                        print_balnk(left);
                }
                printf("|");
        }else{
                if (left2==1){
                        print_balnk(1);
                }
                printf("%s",row->username);
                printf("|");
        }

        if(b_l3>0){
                print_balnk(b_l3);
                printf("%s",row->email);
                int left=default_width-b_l3-len_email;
                if(left>0){
                        print_balnk(left);
                }
                printf("|");
        }else{
                if (left3==1){
                        print_balnk(1);
                }
                printf("%s",row->email);
                printf("|");
        }
        printf("\n");
}
Execute_result execute_insert(Statement *statement,Table *table)
{
        if(table->num_rows>=TABLE_MAX_ROWS){
                return EXECUTE_TABLE_FULL;
        }
        Row *row_to_insert=&(statement->row_to_insert);
        serialize_row(row_to_insert,row_slot(table,table->num_rows));
        table->num_rows++;
        return EXECUTE_SUCCESS;
}
Execute_result execute_select(Statement *statement,Table *table)
{
        Row row;
        print_line(100);
        printf("\n");
        {
                printf("|");
                print_balnk(15);
                printf("id");
                print_balnk(15);
                printf("|");

                print_balnk(12);
                printf("username");
                print_balnk(12);
                printf("|");

                print_balnk(13);
                printf("email");
                print_balnk(14);
                printf("|\n");
        }
        {
                print_line(100);
                printf("\n");
        }
        for (uint32_t i = 0; i < table->num_rows; i++) {
                deserialize_row(row_slot(table, i), &row);
                print_row(&row);
        }
        {
                print_line(100);
                printf("\n");
        }
        return EXECUTE_SUCCESS;
}

Execute_result  execute_statement(Statement *statement,Table *table)
{
        switch (statement->type) {
                case STATEMENT_INSERT:
                        return execute_insert(statement, table);
                case STATEMENT_SELECT:
                        return execute_select(statement, table);
        }
}

void serialize_row(Row *source,void *destination)
{
        memcpy(destination+ID_OFFSET,&(source->id),ID_SIZE);
        memcpy(destination+USERNAME_OFFSET,&(source->username),USERNAME_OFFSET);
        memcpy(destination+EMAIL_OFFSET,&(source->email),EMAIL_SIZE);

}
void deserialize_row(void *source,Row *destination)
{
        memcpy(&(destination->id),source+ID_OFFSET,ID_SIZE);
        memcpy(&(destination->username),source+USERNAME_OFFSET,USERNAME_SIZE);
        memcpy(&(destination->email),source+EMAIL_OFFSET,EMAIL_SIZE);
}

void *get_page(Pager *pager,uint32_t page_num)
{
	if(pagepage_num>TABLE_MAX_ROWS){
		printf("Tried to fetch page number out of bound.%d > %d\n",
				page_num,TABLE_MAX_ROWS);
		exit(EXIT_FAILURE);
	}
	if(pager->pages[page_num]==nullptr){
		void *page=malloc(PAGE_SIZE);
		uint32_t num_pages=pager->file_len/PAGE_SIZE;
		if(pager->file_len%PAGE_SIZE){
			++num_pages;
		}
		if(page_num<=num_pages){
			lseek(pager->fd,page_num*PAGEPAGE_SIZE,SEEK_SET);
			ssize_t bytes_read=read(pager->fd,page,PAGE_SIZE);
			if(bytes_read==-1){
				printf("Error reading file : %d\n",errno);
				exit(EXIT_FAILURE);
			}
		}
		pager->pages[page_num]=page;
	}
	return pager->pages[page_num];
}

void *row_slot(Table *table,uint32_t row_num)
{
        uint32_t page_num= row_num/ROWS_PER_PAGE;

	void *page= get_page(table->pager, page_num);
        uint32_t row_offset=row_num % ROWS_PER_PAGE;
        uint32_t byte_offset= row_offset * ROW_SIZE;
        /* return the pointer of this row */
        return page+byte_offset;
}

Pager *pager_open(const char *filename)
{
	int fd=open(filename,O_RDWR|O_CREAT,S_IWUSR|S_IRUSR);
	if(fd==-1){
		printf("Unable to open file.\n");
		print_log_with(FILE_OPEN_ERROR);
	}
	build_assert(fd!=-1);
	off_t file_len=lseek(fd,0,SEEK_END);
	Pager *pager = malloc(sizeof(Pager));
	pager->file_len=file_len;
	pager->fd=fd;
	for(uint32_t i=0;i<TABLE_MAX_PAGES;i++){
		pager->pages[i]=nullptr;
	}
	return pager;
}

Table *db_open(const char *filename)
{

	Pager *pager=db_open(filename);
	uint32_t num_rows=pager->file_len/ROW_SIZE;
	Table *table = malloc(sizeof(Table));
	table->num_rows=num_rows;
	table->pager=pager;
	return table;
}
void pager_flush(Pager *pager,uint32_t page_num,uint32_t size)
{
	if(pager->pages[page_num]==nullptr){
		printf("Tried to flush null page.\n");
		exit(EXIT_FAILURE);
	}
	off_t offset = lseek(pager->fd,page_num*PAGE_SIZE,SEEK_SET);
	if(offset==-1){
		printf("Error seeking: %d\n",errno);
		exit(EXIT_FAILURE);
	}
	ssize_t bytes_written=
		write(pager->fd, pager->pages[page_num],size);
	if(bytes_written==-1){
		printf("Error writing: %d\n",errno);
		exit(EXIT_FAILURE);
	}
}

void db_close(Table *table)
{
	Pager *pager = table->pager;
	uint32_t num_full_pages=table->num_rows/ROWS_PER_PAGE;
	for(uint32_t i=0;i<num_full_pages;i++){
		if(pager->pages[i]==nullptr){
			continue;
		}
		pager_flush(pager, i,PAGE_SIZE);
		free(pager->pages[i]);
		pager->pages[i]=nullptr;
	}
	uint32_t num_additional_rows=table->num_rows%ROWS_PER_PAGE;
	if(num_additional_rows>0){
		uint32_t page_num=num_full_pages;
		if(pager->pages[page_num]!=nullptr){
			pager_flush(pager, page_num,
					num_additional_rows*ROW_SIZE);
			free(pager->pages[page_num]);
			pager->pages[page_num]=nullptr;
		}
	}
	int result = close(pager->fd);
	if(result==-1){
		printf("Error closing sql-db file.\n");
		exit(EXIT_FAILURE);
	}
	for(uint32_t i=0;i<TABLE_MAX_PAGES;i++){
		void *page=pager->pages[i];
		if(page){
			free(page);
			pager->pages[i]==nullptr;
		}
	}
	free(pager);
	free(table);
}
void *delete_table(Table *table)
{
        for(int i=0;table->pages[i];i++){
                free(table->pages[i]);
        }
        free(table);
}
