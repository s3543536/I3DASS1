#ifndef GAMEOBJECTS
#define GAMEOBJECTS

#include "vector.h"
#include "shapes.h"
#include "mathfunc.h"


typedef struct {
	vector pos;
	float height;
	float width;
} e_car;

typedef struct {
	float radius;
} e_log;

typedef struct {
	vector bottom_left;
	vector top_right;
	float depth;
	sin_data shape;
	size_t nlogs;
	e_log logs[0];
} e_water;

typedef struct {
	char is_collision;
	size_t n_boxes;
	box *box_collision;
	size_t n_vertices;
	vector vertices[0];
} e_wall;



#endif
