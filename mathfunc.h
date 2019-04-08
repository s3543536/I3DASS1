#ifndef MATHFUNC
#define MATHFUNC

#include <math.h>

typedef struct {
	float (*derivative)(void *data, float x);
	float a;
	float b;
	float c;
	float d;
} sin_data;
/** sin function
 *
 * @param sin_data *data    constains the co-efficients of the function
 *                            y = asin(bx + c) + d;
 * @param x                 x value of function
 */
float sin_x(void *data, float x);

// 1st derivative of sin function (including vars)
float dsin_x(void *data, float x);


typedef struct {
	float a;
	float c;
} x_cubed_data;
/** x^3 function
 *
 * @param x_cubed_data *data    constains the co-efficients of the function
 *                                y = ax^3 + c
 * @param x                     x value of function
 */
float x_cubed(void *data, float x);


#endif
