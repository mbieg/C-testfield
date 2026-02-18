#ifndef Point_H
#define Point_H

/**
 * 
 */
typedef struct Point Point;

/**
 * 
 */
Point *Point__ctor(int x, int y);

/**
 * 
 */
void Point__dtor(Point *self);

/**
 * 
 */
int Point__x(Point *self);

/**
 * 
 */
int Point__y(Point *self);

#endif