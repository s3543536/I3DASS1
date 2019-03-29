#ifndef VECTOR
#define VECTOR


#include <math.h>

typedef struct vector {
	float x, y, z;
} vector;

#define LENGTHVEC(V) sqrt((V).x * (V).x + (V).y * (V).y + (V).z * (V).z)


/** cross product of 2 vectors
 *	calculates the cross product and leaves it in the return vector
 * |i	j	k|
 * |a	b	c| = i(b*f - c*e) - j(a*f - d*c) + k(a*e - d*b)
 * |d	e	f|
 *
 * @param vector v1 the first vector (a,b,c) in cross product
 * @param vector v2 the second vector (d,e,f) in cross product
 * @param *vector ret the memory where the vector will be stored
 */
void cross_vectors(vector v1, vector v2, vector *ret);

void init_vector(vector *vec, float x, float y, float z);


#endif
