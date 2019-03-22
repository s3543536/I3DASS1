#ifndef GLVECTOR
#define GLVECTOR

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "vector.h"

/** draw a vector on the screen
 * uses the same colour as previously defined
 * must not be called inside a 'glBegin()' block
 *
 * @param vector v			the vector that will be drawn
 * @param vector pos		the starting point to draw from
 * @param float	s			scaling value to stretch the vector
 * @param char normalize	1 if the vector should be normalized before scaled
 */
void drawVector(vector v, vector pos, float s, char normalize);


#endif
