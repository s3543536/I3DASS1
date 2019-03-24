
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "vector.h"
#include "glvector.h"


/** sin function
 * @param x value of x-axis on the sin function
 * @param y ptr to float where y value will be stored
 */
void sin_x(void *data, float x, float *y);
/** sin function
 * @param x value of x-axis on the x^3 function
 * @param y ptr to float where y value will be stored
 */
void x_cubed(void *data, float x, float *y);

