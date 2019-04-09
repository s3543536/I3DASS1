#ifndef INTERSECT
#define INTERSECT
#endif

#include <float.h>

#include "vector.h"
#include "shapes.h"
#include "mathfunc.h"

char circle_point_is_intersect(circle *c, vector *point);

char circle_is_intersect(circle *c1, circle *c2);

char circle_box_is_intersect(circle *c, box *b);

/** circle intersecting with a function (from mathfunc.h)
 *		within a domain of x E(-1, 1]
 *		requires the derivative function too
 *
 *		@param circle *c      circle that intersects
 *		@param function *f    the function that intersects
 *		@param function *df   derivative of the function that intersects
 *		@param void *data     data for function *f (see mathfunc.h)
 */
char circle_func_is_intersect(circle *c, float (*f)(void *data, float x), float (*df)(void *data, float x), void *data);
