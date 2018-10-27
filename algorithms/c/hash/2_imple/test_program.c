/*
    author: Christian Bender
    This file contains a simple test program for each hash-function.
*/

#include <stdio.h>
#include "hash.h"

int main(void)
{
    char s[] = "name";
	char t[] = "linzr";

    /* actual tests */
    printf("sdbm: %s --> %lld\n", s, sdbm(s));
	printf("sdbm: %s --> %lld\n", t, sdbm(t));
    printf("djb2: %s --> %lld\n", s, djb2(s));
    printf("xor8: %s --> %i\n", s, xor8(s)); /* 8 bit */
    printf("adler_32: %s --> %i\n", s, adler_32(s)); /* 32 bit */

    return 0;
}
