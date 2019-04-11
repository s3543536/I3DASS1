#ifndef SHAPES
#define SHAPES


#include "vector.h"


typedef struct {
	vector c;//centre
	float h;//height
	float w;//width
} box;

typedef struct {
	float r;//radius
	vector c;//centre
} circle;

// this needs to be a superset of circle
// TODO: just make it contain a circle
typedef struct {
	// r = (x/a)^2 + (y/b)^2
	float r;
	vector c;//centre
	float a;
	float b;
} oval;

#endif

