#ifndef MATHFUNC
#define MATHFUNC

#include <stdlib.h>
#include <math.h>

typedef struct {
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


typedef struct {
	float i;
	float j;
	float (*f)(void *data, float x);// function f(x)
	float (*df)(void *data, float x);// derivative of f(x)
	void *f_data;
} f_dist_data;

/** distance between a point (i, j) and f(x)
 *		doesn't require the derivative (f_dist_data.df())
 */
float f_dist(void *data, float x);

/** derivative of f_dist
 *		requires the derivative of f (f_dist_data.df())
 */
float f_dist_derivative(void *data, float x);

/** finds the derivative of any function
 *		by finding the value at 2 close points
 */
float generic_derivative(float (*f)(void *data, float x), void *f_data, float x, float delta);

/** newtons method of finding roots
 *		goes through all guesses and replaces them with the value that guess produced
 *		df() is optional. if its omitted, approximate derivative is used
 */
void newtons(float (*f)(void *data, float x), float (*df)(void *data, float x), void *f_data, size_t n_itter, float *guesses, size_t n_guess);


#endif
