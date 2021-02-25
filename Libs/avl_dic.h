/* a dictionary based on AVL tree
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/24
 */

#ifndef AVL_TEST_AVL_DIC_H
#define AVL_TEST_AVL_DIC_H

#include <defs.h>
#include <mathtool.h>
#include <debug.h>

#undef bool

#include <stdbool.h>

typedef int (*cmp_t)(const void *, const void *);


typedef struct AvlNode {
        /* <K,V>pair */
        const void *key;
        const void *value;
        struct AvlNode *father;
        struct AvlNode *lc;
        struct AvlNode *rc;
        /* -1 for lh/rh means height haven't been counted */
        int lh;
        int rh;
        /* uninitialized means height haven't been counted && 0 means null-node */
        int h;
        int balance;
} avl_node;

typedef struct AVL {
        cmp_t cmp;
        avl_node *root;
        size_t count;
} avl;
/* structure of father & son */
typedef struct fs {

        avl_node *father;
        avl_node *this;
} fs;

avl *avl_init(cmp_t cmp);

avl_node *new_node(const void *key, const void *value);


void free_avl(avl *avl_tree);

fs *avl_search(const avl *avl_tree, const void *key);

#define avl_member_exist(avl, key) (!!avl_lookup_node(avl, key))

size_t avl_count(const avl *avl_tree);

size_t get_update_height(avl_node *node);

size_t count_child(avl_node *node);

size_t count_lc(avl_node *node);

size_t count_rc(avl_node *node);

bool is_balanced(avl *avl_tree);

void avl_insert(avl *avl_tree, const void *key, const void *value);

bool avl_remove(avl *avl_tree, const void *key);

bool avl_check_invariants(avl *avl_tree);


/*test part*/
void _inorder(avl_node *node);

void inorder(avl *avl_tree);


#endif //AVL_TEST_AVL_DIC_H
