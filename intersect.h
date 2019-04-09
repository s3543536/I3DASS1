#ifndef INTERSECT
#define INTERSECT
#endif

#include <float.h> // needed for FLT_MAX

#include "mathfunc.h"
#include "vector.h"
#include "shapes.h"

char circle_point_is_intersect(circle *c, vector *point);

char circle_is_intersect(circle *c1, circle *c2);

char circle_box_is_intersect(circle *c, box *b);

/** check if a circle is intersecting with an arbitrary function
 *		applies within the domain x:(-1,1]
 *		requires function and derivative of function
 *
 *		@param circle *c     the circle to intersect
 *		@param func *f       the function to intersect
 *		@param func *df      derivative function
 *		@param void *data    data for function (see mathfunc.h)
 */
char circle_func_is_intersect(circle *c, float (*f)(void *data, float x), float (*df)(void *data, float x), void *data);
