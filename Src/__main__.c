/*
 * Src/database_main.c
 * The main interface of the whole project
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/9
 */
#include <array_size.h>
#include <stdio.h>

int main()
{
        int arr[4];
        printf("%d",array_size(arr));

#include <malloc.h>
        int *A=(int*)malloc(sizeof(int)*4);
        printf("%ld",array_size(A));


        return 0;
}