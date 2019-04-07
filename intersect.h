#ifndef INTERSECT
#define INTERSECT
#endif

#include "vector.h"
#include "shapes.h"

char circle_point_is_intersect(circle *c, vector *point);

char circle_is_intersect(circle *c1, circle *c2);

char circle_box_is_intersect(circle *c, box *b);
