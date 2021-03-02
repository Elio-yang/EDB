/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/28
 */

#ifndef MY_DATABASE_LIST_H
#define MY_DATABASE_LIST_H

#include <defs.h>

struct list_entry{
        struct list_entry *prev;
        struct list_entry *next;
};
typedef struct list_entry list_entry_t;

__always_inline void list_init(list_entry_t *elm);
__always_inline void list_add(list_entry_t *listelm,list_entry_t *elm);
__always_inline void list_add_before(list_entry_t *listelm,list_entry_t *elm);
__always_inline void list_add_after(list_entry_t *listelm,list_entry_t *elm);
__always_inline void list_del(list_entry_t *listelm);
__always_inline void list_del_init(list_entry_t *listelm);
__always_inline bool list_empty(list_entry_t *list);
__always_inline list_entry_t *list_prev(list_entry_t *listelm);
__always_inline list_entry_t *list_next(list_entry_t *listelm);

__always_inline void __list_add(list_entry_t *elm,list_entry_t *prev, list_entry_t *next);
__always_inline void __list_del(list_entry_t *prev, list_entry_t *next);









#endif //MY_DATABASE_LIST_H
