/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/19
 */

#ifndef MY_DATABASE_BSEARCH_H
#define MY_DATABASE_BSEARCH_H

#include <defs.h>
typedef int (*bsearch_cmp)(const void *,const void *,void *);

/*
 * bsearch - search an array of elements
 * @key: pointer to item being searched for
 * @base: pointer to data to sort
 * @num: number of elements
 * @cmp: pointer to comparison function
 */
#define bsearch(key,base,num,cmp,ctx) \



void * _bsearch(const void *key,const void *base,size_t nmemb,size_t size,
                bsearch_cmp cmp, void *ctx);
#endif //MY_DATABASE_BSEARCH_H
