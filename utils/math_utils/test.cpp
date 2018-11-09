
/***
 * build: gcc test.cpp -o test -std=c++11
 *
 * Disc:
 *     you can use this math utils implement a percentage to a val
 *     && a val to a percentage by given range.
 *
 * @LaMar
 */

#include <stdio.h>
#include "math.h"

int main()
{
    printf("============== TEST ===============\n");
    printf("%d\n", math_util::percentage_to_value(15, 10, 53));
    printf("%d\n", math_util::percentage_to_value(50, 10, 53));
    printf("%d\n", math_util::percentage_to_value(55, 10, 53));
    printf("%d\n", math_util::percentage_to_value(65, 10, 53));
    printf("%d\n", math_util::percentage_to_value(95, 10, 53));
    printf("%d\n", math_util::percentage_to_value(86, 0, 65536));
    printf("============== TEST ===============\n");
    printf("%d\n", math_util::percentage<int, int>(16, 10, 53));
    printf("%d\n", math_util::percentage<int, int>(32, 10, 53));
    printf("%d\n", math_util::percentage<int, int>(34, 10, 53));
    printf("%d\n", math_util::percentage<int, int>(38, 10, 53));
    printf("%d\n", math_util::percentage<int, int>(51, 10, 53));
    printf("%d\n", math_util::percentage<int, int>(56222, 0, 65536));
    printf("============== TEST ===============\n");
}









