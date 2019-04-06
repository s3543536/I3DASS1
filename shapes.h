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


#endif

