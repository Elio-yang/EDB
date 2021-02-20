/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/20
 */


#include <bsearch.h>
#include <limits.h>
static int test_cmp(const int *key, const int *elt)
{
        if (*key < *elt)
                return -1;
        else if (*key > *elt)
                return 1;
        return 0;
}

int main(void)
{
        const int arr[] = { INT_MIN, 0, 1, 2, 3, 4, 5, 6, INT_MAX };
        unsigned int start, num, i, total = 0;
        const int key=3;
        void * p=bsearch(&key,arr, sizeof(arr)/ sizeof(arr[0]),test_cmp);
        printf("%d",*(int*)p);
        return 0;

