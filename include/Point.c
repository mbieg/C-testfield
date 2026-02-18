#include "Point.h"
#include <stdlib.h>



struct Point
{
    int x;
    int y;
};

void Point__init(Point *self, int x, int y)
{
    self->x = x;
    self->y = y;
}


Point *Point__ctor(int x, int y)
{
    Point *result = (Point*) malloc(sizeof(Point));
    Point__init(result, x, y);
    return result;
}


void Point__dtor(Point *point)
{
    if (point )
    {
        free(point);
    }
}

int Point__x(Point *self)
{
    return self->x;
}

int Point__y(Point *self)
{
    return self->y;
}