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

float distance_vector(vector *v1, vector *v2) {
	float dx = v1->x - v2->x;
	float dy = v1->y - v2->y;
	float dz = v1->z - v2->z;
	return sqrt(dx*dx + dy*dy + dz*dz);
}


void vector_normalize(vector *v) {
	float len = LENGTHVEC(*v);
	v->x /= len;
	v->y /= len;
	v->z /= len;
}

void vector_scale(vector *v, float scale) {
	v->x *= scale;
	v->y *= scale;
	v->z *= scale;
}

void vector_rotate_xy(vector *v, float angle) {
	float temp_x = v->x;
	v->x = v->x*cos(angle) - v->y*sin(angle);
	v->y = temp_x*sin(angle) + v->y*cos(angle);
}
