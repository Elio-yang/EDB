/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/25
 */

#include "avl.h"

/*
 * return >0 : a>b
 * return <0 : a<b
 */
int cmp_test(const void*a,const void *b)
{
        return *((int*)a)-*((int*)b);
}


int main()
{
        AVL *avl_tree=avl_new(cmp_test);
        int test_data[]={
                1,2,3,4,5,6,7,8,9,23,
                34,67,24,55,87,99,56,
                12,98,33,25,62,78,92
        };
        size_t test_size= sizeof(test_data)/sizeof(test_data[0]);
        long long value[test_size];
        for(int i=0;i<test_size;i++){
                value[i]=(((test_data[i]<<12)%101)<<5)%107;
        }

        for(int i=0;i<test_size;i++){
                avl_insert(avl_tree,&test_data[i],&value[i]);
        }
        inorder(avl_tree);
        return 0;
}