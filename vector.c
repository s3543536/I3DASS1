#include "vector.h"



void init_vector(vector *vec, float x, float y, float z) {
	vec->x = x;
	vec->y = y;
	vec->z = z;
}


void cross_vectors(vector v1, vector v2, vector *ret) {
	/*
		|i	j	k|
		|a	b	c| = i(b*f - c*e) - j(a*f - d*c) + k(a*e - d*b)
		|d	e	f|
	*/
	ret->x = v1.y * v2.z - v1.z * v2.y;
	ret->y = -1 * (v1.x * v2.z - v1.z * v2.x);
	ret->z = v1.x * v2.y - v1.y * v2.x;
}

#define LENGTHVEC(V) sqrt((V).x * (V).x + (V).y * (V).y + (V).z * (V).z)
