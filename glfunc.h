#ifndef GLFUNC
#define GLFUNC

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "glvector.h"
#include "vector.h"

// draws a function to the screen with opengl
void draw_2d_function(void (*f)(void *data, float x, float *y), void *pass_thr, float x_scale, float y_scale);

// draws the normals of a function to the screen with opengl
void draw_2d_function_normals(void (*f)(void *data, float x, float *y), void *pass_thr, float x_scale, float y_scale);


#endif
