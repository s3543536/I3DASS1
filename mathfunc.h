#ifndef MATHFUNC
#define MATHFUNC

#include <math.h>

typedef struct {
	float a;
	float b;
	float c;
	float d;
} sin_data;
/** sin function
 * @param x value of x-axis on the sin function
 * @param y ptr to float where y value will be stored
 */
void sin_x(void *data, float x, float *y);

typedef struct {
	float a;
	float c;
} x_cubed_data;
/** sin function
 * @param x value of x-axis on the x^3 function
 * @param y ptr to float where y value will be stored
 */
void x_cubed(void *data, float x, float *y);


#endif
