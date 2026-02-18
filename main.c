#include <stdio.h>
#include "include/Point.h"

int main(void)
{
    Point *p = Point__ctor(0, 0);

    printf("point: %i", Point__x(p));
    printf("hello cmake\n");

    return 0;
}