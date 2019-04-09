#ifndef GLSHAPES
#define GLSHAPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#include "shapes.h"

void drawAxes(float length, int draw_negative);


#ifndef PI
// nasa-approved 15 digits (fits in a double)
#define PI 3.141592653589793
#endif
void draw_circle(circle *c, size_t nvertex, char filled);

void draw_box(box *b, char filled);



#endif
