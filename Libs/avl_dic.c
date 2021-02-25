/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/24
 */
#include <avl_dic.h>

__always_inline avl *avl_init(cmp_t cmp)
{
        avl *avl_tree = malloc(sizeof(*avl_tree));
        if (avl_tree == nullptr) {
                print_log_with(MEMORY_ALLOCATED_ERROR);
        }
        build_assert(avl_tree != nullptr);

        avl_tree->root = nullptr;
        avl_tree->cmp = cmp;
        avl_tree->count = 0;

        return avl_tree;
}

__always_inline avl_node *new_node(const void *key, const void *value)
{
        avl_node *node = malloc(sizeof(*node));
        if (node == nullptr) {
                print_log_with(MEMORY_ALLOCATED_ERROR);
        }
        build_assert(node != nullptr);

        {
                node->rc = nullptr;
                node->lc = nullptr;
                node->father = nullptr;
                node->value = value;
                node->key = key;
                node->lh = -1;
                node->rh = -1;
                node->h = uninitialized;
                node->balance = 0;
        }

        return node;

}

void free_avl_node(avl_node *node)
{
        if (node) {
                free_avl_node(node->lc);
                free_avl_node(node->rc);
                free(node);
        }
}

void free_avl(avl *avl_tree)
{
        free_avl_node(avl_tree->root);
        free(avl_tree);
}


__always_inline fs *search(avl_node *node, const void *key, cmp_t cmp_f)
{
        /* father node will always pointed to the father of node with key */

        fs *recd = malloc(sizeof(*recd));
        if (recd == nullptr) {
                print_log_with(MEMORY_ALLOCATED_ERROR);
        }
        build_assert(recd != nullptr);

        avl_node *this_father = malloc(sizeof(*this_father));
        if (this_father == nullptr) {
                print_log_with(MEMORY_ALLOCATED_ERROR);
        }
        build_assert(this_father != nullptr);

        if (node == nullptr || cmp_f(node->key, key) == 0) {
                recd->father = nullptr;
                recd->this = node;
                return recd;
        }

        int cmp;
        avl_node *tmp = nullptr;
        for (this_father = node;;) {
                cmp = cmp_f(key, this_father->key);
                /* at begin cmp must not be zero */
                if (cmp < 0) {
                        tmp = this_father->lc;
                } else if (cmp > 0) {
                        tmp = this_father->rc;
                }

                if (tmp == nullptr || cmp == 0) {
                        recd->father = this_father;
                        recd->this = tmp;
                        return recd;
                }
                this_father = tmp;
        }
}

fs *avl_search(const avl *avl_tree, const void *key)
{
        return search(avl_tree->root,key,avl_tree->cmp);
}

__always_inline size_t avl_count(const avl *avl_tree)
{
        return avl_tree->count;
}

/* get height && update h,lh,rh,bf*/
size_t get_update_height(avl_node *node)
{
        if (node == NULL) {
                return 0;
        }
        int left_h = get_update_height(node->lc);
        int righ_h = get_update_height(node->rc);

        node->lh = left_h;
        node->rh = righ_h;
        node->balance = node->rh - node->lh;
        return node->h = 1 + max(left_h, righ_h);

}

__always_inline size_t get_height(avl_node *node)
{
        return node->h;
}

bool balance_judge_order(avl_node *node)
{
        if (node) {
                if (balance_judge_order(node->lc) && balance_judge_order(node->rc)) {
                        return true;
                }
                if (!(-2 < node->balance && node->balance < 2)) {
                        return false;
                }
        }
        return true;
}

bool is_balanced(avl *avl_tree)
{
        if (avl_tree->root) {
                return balance_judge_order(avl_tree->root);
        }
        return true;
}

__always_inline bool balanced_at(avl_node *node)
{
        if (-2 < node->balance && node->balance < 2) {
                return true;
        }
        return false;
}

/* right height - left height */
__always_inline int get_bf(avl_node *node)
{
        return node->rh - node->lh;
}

__always_inline void update_height(avl_node *node)
{
        get_update_height(node);
}

__always_inline void update_height_above(avl_node *node)
{
        avl_node *tmp = node;
        while (tmp) {
                update_height(tmp);
                tmp = tmp->father;
        }
}

void rr_rotate(avl *avl_tree, avl_node *alpha, avl_node *alpha_r)
{

        if (alpha_r->lc) {
                alpha_r->lc->father = alpha;
        }
        alpha_r->father = alpha->father;
        if (alpha->father) {
                int cmp_r_af = avl_tree->cmp(alpha_r->key,
                                             alpha->father->key);
                if (cmp_r_af < 0) {
                        alpha->father->lc = alpha_r;
                } else {
                        alpha->father->rc = alpha_r;
                }
        }
        alpha->father = alpha_r;
        alpha->rc = alpha_r->lc;
        alpha_r->lc = alpha;


        if (avl_tree->root == alpha) {
                avl_tree->root = alpha_r;
        }
}

void ll_rotate(avl *avl_tree, avl_node *alpha, avl_node *alpha_l)
{
        if (alpha_l->rc) {
                alpha_l->rc->father = alpha;
        }
        alpha_l->father = alpha->father;
        if (alpha->father) {
                int cmp_l_af = avl_tree->cmp(alpha_l->key, alpha->father->key);

                if (cmp_l_af < 0) {
                        alpha->father->lc = alpha_l;
                } else {
                        alpha->father->rc = alpha_l;
                }
        }
        alpha->father = alpha_l;
        alpha->lc = alpha_l->rc;
        alpha_l->rc = alpha;

        if (avl_tree->root == alpha) {
                avl_tree->root = alpha_l;
        }
}

void rl_rotate(avl *avl_tree, avl_node *alpha, avl_node *alpha_r, avl_node *beta_stay)
{
        beta_stay->father = alpha->father;
        avl_node *tmp_bs_lc = beta_stay->lc;
        avl_node *tmp_bs_rc = beta_stay->rc;
        beta_stay->lc = alpha;
        avl_node *tmp_father = alpha->father;
        alpha->father = beta_stay;
        beta_stay->rc = alpha_r;
        alpha_r->father = beta_stay;
        if (tmp_father) {
                int cmp_l_af = avl_tree->cmp(beta_stay->key,
                                             tmp_father->key);
                if (cmp_l_af < 0) {
                        tmp_father->lc = beta_stay;
                }
                if (cmp_l_af > 0) {
                        tmp_father->rc = beta_stay;
                }
        }

        alpha->rc = tmp_bs_lc;
        if (tmp_bs_lc) {
                tmp_bs_lc->father = alpha;
        }
        alpha_r->lc = tmp_bs_rc;
        if (tmp_bs_rc) {
                tmp_bs_rc->father = alpha;
        }

        if (avl_tree->root == alpha) {
                avl_tree->root = beta_stay;
        }
}

void lr_rotate(avl *avl_tree, avl_node *alpha, avl_node *alpha_l, avl_node *beta_stay)
{
        beta_stay->father = alpha->father;
        avl_node *tmp_bs_lc = beta_stay->lc;
        avl_node *tmp_bs_rc = beta_stay->rc;

        beta_stay->lc = alpha_l;
        avl_node *tmp_father = alpha->father;
        alpha->father = beta_stay;
        beta_stay->rc = alpha;
        alpha_l->father = beta_stay;
        if (tmp_father) {
                int cmp_r_af = avl_tree->cmp(beta_stay->key,
                                             tmp_father->key);
                if (cmp_r_af < 0) {
                        tmp_father->lc = beta_stay;
                }
                if (cmp_r_af > 0) {
                        tmp_father->rc = beta_stay;
                }
        }
        alpha->lc = tmp_bs_rc;
        if (tmp_bs_rc) {
                tmp_bs_rc->father = alpha;
        }
        alpha_l->rc = tmp_bs_lc;
        if (tmp_bs_lc) {
                tmp_bs_lc->father = alpha_l;
        }

        if (avl_tree->root == alpha) {
                avl_tree->root = beta_stay;
        }
}

void avl_insert(avl *avl_tree, const void *key, const void *value)
{
        fs *tar = search(avl_tree->root, key, avl_tree->cmp);
        if (tar->this) {
                return;
        }
        avl_node *beta = new_node(key, value);
        /* update lh&rh&h&balance of node */
        get_update_height(beta);
        beta->father = tar->father;
        if (tar->father) {
                int cmp = avl_tree->cmp(beta->key, tar->father->key);
                if (cmp < 0) {
                        tar->father->lc = beta;
                } else if (cmp > 0) {
                        tar->father->rc = beta;
                }
        } else {
                /*root node*/
                avl_tree->root = beta;
        }
        avl_tree->count++;
        get_update_height(avl_tree->root);
        /* update h and lh & rh from node->father */
        for (avl_node *alpha = beta->father; alpha; alpha = alpha->father) {
                /* balance is broken at tmp by the insertion of node
                 * do rotation here
                 *
                 * alpha-node: tmp
                 * beta-node:  node
                 */
                if (!balanced_at(alpha)) {
                        int bf = get_bf(alpha);
                        switch (bf) {
                                case 2: {
                                        /*not null*/
                                        avl_node *alpha_r = alpha->rc;
                                        if (alpha_r == nullptr) {
                                                print_log_with(NULLPTR);
                                        }
                                        build_assert(alpha_r != nullptr);
                                        /* RR rotate*/
                                        int cmp_rr = avl_tree->cmp(beta->key, alpha_r->key);
                                        if (cmp_rr > 0) {
                                                rr_rotate(avl_tree, alpha, alpha_r);
                                                {
//                                                if (alpha_r->lc) {
//                                                        alpha_r->lc->father = alpha;
//                                                }
//                                                alpha_r->father = alpha->father;
//                                                if (alpha->father) {
//                                                        int cmp_r_af = avl_tree->cmp(alpha_r->key,
//                                                                                     alpha->father->key);
//                                                        if (cmp_r_af < 0) {
//                                                                alpha->father->lc = alpha_r;
//                                                        } else {
//                                                                alpha->father->rc = alpha_r;
//                                                        }
//                                                }
//                                                alpha->father = alpha_r;
//                                                alpha->rc = alpha_r->lc;
//                                                alpha_r->lc = alpha;
//
//
//                                                if (avl_tree->root == alpha) {
//                                                        avl_tree->root = alpha_r;
//                                                }
                                                }
                                                get_update_height(avl_tree->root);
                                                goto done2;
                                        }
                                        /* RL rotate */
                                        rl_rotate(avl_tree, alpha, alpha_r, alpha_r->lc);
                                        {
//                                                avl_node *beta_stay = alpha_r->lc;
//                                                beta_stay->father = alpha->father;
//                                                avl_node *tmp_bs_lc =beta_stay->lc;
//                                                avl_node *tmp_bs_rc=beta_stay->rc;
//                                                beta_stay->lc = alpha;
//                                                avl_node *tmp_father = alpha->father;
//                                                alpha->father = beta_stay;
//                                                beta_stay->rc = alpha_r;
//                                                alpha_r->father = beta_stay;
//                                                if (tmp_father) {
//                                                        int cmp_l_af = avl_tree->cmp(beta_stay->key,
//                                                                                     tmp_father->key);
//                                                        if (cmp_l_af < 0) {
//                                                                tmp_father->lc = beta_stay;
//                                                        }
//                                                        if (cmp_l_af > 0) {
//                                                                tmp_father->rc = beta_stay;
//                                                        }
//                                                }
//
//                                                alpha->rc = tmp_bs_lc;
//                                                if (tmp_bs_lc) {
//                                                        tmp_bs_lc->father = alpha;
//                                                }
//                                                alpha_r->lc = tmp_bs_rc;
//                                                if (tmp_bs_rc) {
//                                                        tmp_bs_rc->father = alpha;
//                                                }
//
//                                                if (avl_tree->root == alpha) {
//                                                        avl_tree->root = beta_stay;
//                                                }
                                        }
                                        get_update_height(avl_tree->root);
                                        done2:
                                        break;
                                }
                                case -2: {
                                        avl_node *alpha_l = alpha->lc;
                                        if (alpha_l == nullptr) {
                                                print_log_with(NULLPTR);
                                        }
                                        build_assert(alpha_l != nullptr);
                                        /* LL rotate */
                                        int cmp_ll = avl_tree->cmp(beta->key, alpha_l->key);
                                        if (cmp_ll < 0) {
                                                ll_rotate(avl_tree, alpha, alpha_l);
                                                {
//                                                if (alpha_l->rc) {
//                                                        alpha_l->rc->father = alpha;
//                                                }
//                                                alpha_l->father = alpha->father;
//                                                if (alpha->father) {
//                                                        int cmp_l_af = avl_tree->cmp(alpha_l->key, alpha->father->key);
//
//                                                        if (cmp_l_af < 0) {
//                                                                alpha->father->lc = alpha_l;
//                                                        } else {
//                                                                alpha->father->rc = alpha_l;
//                                                        }
//                                                }
//                                                alpha->father = alpha_l;
//                                                alpha->lc = alpha_l->rc;
//                                                alpha_l->rc = alpha;
//
//                                                if (avl_tree->root == alpha) {
//                                                        avl_tree->root = alpha_l;
//                                                }
                                                }
                                                get_update_height(avl_tree->root);
                                                goto done_2;
                                        }
                                        /* LR rotate */
                                        lr_rotate(avl_tree, alpha, alpha_l, alpha_l->rc);
                                        {
//                                                avl_node *beta_stay = alpha_l->rc;
//                                                beta_stay->father = alpha->father;
//                                                avl_node *tmp_bs_lc =beta_stay->lc;
//                                                avl_node *tmp_bs_rc=beta_stay->rc;
//
//                                                beta_stay->lc = alpha_l;
//                                                avl_node *tmp_father = alpha->father;
//                                                alpha->father = beta_stay;
//                                                beta_stay->rc = alpha;
//                                                alpha_l->father = beta_stay;
//                                                if (tmp_father) {
//                                                        int cmp_r_af = avl_tree->cmp(beta_stay->key,
//                                                                                     tmp_father->key);
//                                                        if (cmp_r_af < 0) {
//                                                                tmp_father->lc = beta_stay;
//                                                        }
//                                                        if (cmp_r_af > 0) {
//                                                                tmp_father->rc = beta_stay;
//                                                        }
//                                                }
//                                                alpha->lc = tmp_bs_rc;
//                                                if (tmp_bs_rc) {
//                                                        tmp_bs_rc->father = alpha;
//                                                }
//                                                alpha_l->rc = tmp_bs_lc;
//                                                if (tmp_bs_lc) {
//                                                        tmp_bs_lc->father = alpha_l;
//                                                }
//
//                                                if (avl_tree->root == alpha) {
//                                                        avl_tree->root = beta_stay;
//                                                }
                                        }
                                        get_update_height(avl_tree->root);
                                        done_2:
                                        break;
                                }
                        }
                } else {
                        update_height_above(alpha);
                }
        }
        free(tar);
}


bool avl_remove(avl *avl_tree, const void *key);

bool avl_check_invariants(avl *avl_tree);

void _inorder(avl_node *node)
{
        if (node) {
                _inorder(node->lc);
                printf("|\tkey:%-4d\t|\tvalue:%10lld\n",
                       *((int *) node->key), *((long long *) node->value));
                _inorder(node->rc);
        }
}

void inorder(avl *avl_tree)
{
        if (avl_tree->root) {
                _inorder(avl_tree->root);
        }
}
