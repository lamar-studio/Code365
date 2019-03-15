/*
 * Desc: 
 *    test the memcpy performance for new plat
 *
 * Build: 
 *    gcc -o memcpy_test memcpy_test.c
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>

/* 测试数据大小 */
#define SIZE_1920_1080 1920*1080

/* 1M数据*/
void test_1M(void)
{
    struct timeval start;
    struct timeval end;
    long delta_time;

    char dst0[SIZE_1920_1080];
    char dst1[SIZE_1920_1080];
    char dst2[SIZE_1920_1080];
    char dst3[SIZE_1920_1080];

    char *array0 = (char *)malloc(SIZE_1920_1080);
    char *array1 = (char *)malloc(SIZE_1920_1080);
    char *array2 = (char *)malloc(SIZE_1920_1080);
    char *array3 = (char *)malloc(SIZE_1920_1080);

    gettimeofday(&start, NULL);
    memcpy(dst0, array0, SIZE_1920_1080);
    memcpy(dst1, array1, SIZE_1920_1080);
    memcpy(dst2, array2, SIZE_1920_1080);
    memcpy(dst3, array3, SIZE_1920_1080);
    gettimeofday(&end, NULL);
    delta_time = (end.tv_sec* 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
    fprintf(stderr, "SIZE_1920_1080:\t%ldus\n", delta_time);

    free(array0);
    free(array1);
    free(array2);
    free(array3);

}


int main(int argc, char **argv)
{
    test_1M();

    return 0;
}































