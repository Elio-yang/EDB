/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/28
 */

#include "list.h"

/*
 * init a double link list
 */
__always_inline void list_init(list_entry_t *elm)
{
        elm->prev = elm->next = elm;
}
/*
 * default : add after listelm
 */
__always_inline void list_add(list_entry_t *listelm, list_entry_t *elm)
{
        list_add_after(listelm, elm);
}
/*
 * insert an element before listelm
 */
__always_inline void list_add_before(list_entry_t *listelm, list_entry_t *elm)
{
        __list_add(elm, listelm->prev, listelm);
}
/*
 * insert an element after listelm
 */
__always_inline void list_add_after(list_entry_t *listelm, list_entry_t *elm)
{
        __list_add(elm,listelm,listelm->next);
}
/*
 * delete an element
 */
__always_inline void list_del(list_entry_t *listelm)
{
        __list_del(listelm->prev,listelm->next);
}

__always_inline void list_del_init(list_entry_t *listelm)
{
        list_del(listelm);
        list_init(listelm);
}
__always_inline bool list_empty(list_entry_t *list)
{
       return list->next==list;
}
__always_inline list_entry_t *list_prev(list_entry_t *listelm)
{
        return listelm->prev;
}

__always_inline list_entry_t *list_next(list_entry_t *listelm)
{
        return listelm->next;
}

__always_inline void __list_add(list_entry_t *elm, list_entry_t *prev, list_entry_t *next)
{
        prev->next = next->prev = elm;
        elm->next = next;
        elm->prev = prev;
}
__always_inline void __list_del(list_entry_t *prev, list_entry_t *next)
{
        prev->next = next;
        next->prev = prev;
}