/*
 * Libs/adlist.h
 * implementation of a doubly link list
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/8
 */

#ifndef MY_DATABASE_ADLIST_H
#define MY_DATABASE_ADLIST_H

#include <defs.h>
#include <mm.h>
#include <string_db.h>
#include <file_sys.h>
#include <db_log.h>
#include <errors.h>
/*link part*/
struct list_entry {
        struct list_entry *prev, *next;
};

typedef struct list_entry list_entry_t;

/*
 * GCC extension —— Arrays of zero length
 * <https://gcc.gnu.org/onlinedocs/gcc-4.6.2/gcc/Zero-Length.html#Zero-Length>:
 * struct line {
 *       int length;
 *       char contents[0];
 * };
 * struct line *thisline = (struct line *)
 * malloc (sizeof (struct line) + this_length);
 * thisline->length = this_length;
 */
typedef struct List {
        list_entry_t* head;
        list_entry_t* tail;
        uint32_t len;
        uint32_t name_len;

        void (*l_free)(void);
        void (*le_free)(void *ptr);
        void *(*le_dup)(void *ptr);
        uint32_t (*le_match)(void *ptr, void *key);

        char l_name[0];
} List_t;
static __always_inline List_t *listInit(const char*list_name);




static __always_inline List_t *listInit(const char*list_name)
{
        List_t * L =(List_t*)malloc(sizeof(List_t)+ sizeof(list_name)+1);
        if(L==nullptr){
                print_log(__FILE__,__LINE__,__FUNCTION__ ,"%s",MEMORY_ALLOCATED_ERROR);
                fprintf(stderr,"%s",MEMORY_ALLOCATED_ERROR);
                abort();
        }

        L->name_len= sizeof(list_name);
        strcpy(L->l_name,list_name);
        {
                L->head = nullptr;
                L->tail = nullptr;
                L->l_free = nullptr;
                L->le_free = nullptr;
                L->le_dup = nullptr;
                L->le_match = nullptr;
                L->len = 0;
        }
        return L;
}

#endif //MY_DATABASE_ADLIST_H
