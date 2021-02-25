#ifndef CCAN_AVL_H
#define CCAN_AVL_H

#include <stdbool.h>
#include <stddef.h>

typedef int (*cmp_t)(const void *, const void *);

typedef struct AVL           AVL;
typedef struct AvlNode       AvlNode;
typedef struct AvlIter       AvlIter;

AVL *avl_new(cmp_t compare);
	/* Create a new AVL tree sorted with the given comparison function. */

void avl_free(AVL *avl);
	/* Free an AVL tree. */

void *avl_lookup(const AVL *avl, const void *key);
	/* O(log n). Lookup a value at a key.  Return NULL if the key is not present. */

#define avl_member(avl, key) (!!avl_lookup_node(avl, key))
	/* O(log n). See if a key is present. */

size_t avl_count(const AVL *avl);
	/* O(1). Return the number of elements in the tree. */

bool avl_insert(AVL *avl, const void *key, const void *value);
	/*
	 * O(log n). Insert a key/value pair, or replace it if already present.
	 *
	 * Return false if the insertion replaced an existing key/value.
	 */

bool avl_remove(AVL *avl, const void *key);
	/*
	 * O(log n). Remove a key/value pair (if present).
	 *
	 * Return true if it was removed.
	 */

bool avl_check_invariants(AVL *avl);
	/* For testing purposes.  This function will always return true :-) */


/************************* Traversal *************************/

#define avl_foreach(iter, avl)         avl_traverse(iter, avl, FORWARD)
	/*
	 * O(n). Traverse an AVL tree in order.
	 *
	 * Example:
	 *
	 * AvlIter i;
	 *
	 * avl_foreach(i, avl)
	 *     printf("%s -> %s\n", i.key, i.value);
	 */

#define avl_foreach_reverse(iter, avl) avl_traverse(iter, avl, BACKWARD)
	/* O(n). Traverse an AVL tree in reverse order. */

typedef enum AvlDirection {FORWARD = 0, BACKWARD = 1} AvlDirection;

struct AvlIter {
	void         *key;
	void         *value;
	AvlNode      *node;
	
	/* private */
	AvlNode      *stack[100];
	int           stack_index;
	AvlDirection  direction;
};

void avl_iter_begin(AvlIter *iter, AVL *avl, AvlDirection dir);
void avl_iter_next(AvlIter *iter);
#define avl_traverse(iter, avl, direction)        \
	for (avl_iter_begin(&(iter), avl, direction); \
	     (iter).node != NULL;                     \
	     avl_iter_next(&iter))


/***************** Internal data structures ******************/

struct AVL {
	cmp_t compare;
	AvlNode    *root;
	size_t      count;
};

struct AvlNode {
	const void *key;
	const void *value;
	
	AvlNode    *lr[2];
	int         balance; /* -1, 0, or 1 */
};

AvlNode *avl_lookup_node(const AVL *avl, const void *key);
/* O(log n). Lookup an AVL node by key.  Return NULL if not present. */

void _inorder( AvlNode *node);
void inorder(AVL *avl_tree);

#endif
