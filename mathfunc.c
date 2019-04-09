#include "mathfunc.h"


float sin_x(void *data, float x) {
	sin_data _data = *(sin_data*)data;
	return _data.a * sinf(_data.b * x + _data.c) + _data.d;
}

float dsin_x(void *data, float x) {
	sin_data _data = *(sin_data*)data;
	return _data.a * _data.b * cosf(_data.b * x + _data.c);
}

float x_cubed(void *data, float x) {
	x_cubed_data _data = *(x_cubed_data*)data;
	return _data.a*x*x*x + _data.c;
}

float f_dist(void *data, float x) {
	f_dist_data data_= *(f_dist_data*)data;
	float xdist = data_.i - x;
	float ydist = data_.j - data_.f(data_.f_data, x);
	return sqrt(xdist*xdist + ydist*ydist);
}

float f_dist_derivative(void *data, float x) {
	f_dist_data data_ = *(f_dist_data*)data;

	float a = ((sin_data*)data_.f_data)->a;
	float b = ((sin_data*)data_.f_data)->b;
	float c = ((sin_data*)data_.f_data)->c;
	float d = ((sin_data*)data_.f_data)->d;

	float funcval = data_.f(data_.f_data, x);

	float i_minus_x = data_.i - x;
	float j_minus_func = data_.j - funcval;

	/* the derivative of the distance function:
	 * (1/(2*sqrt((i-x)^2 + (j-f(x))^2)))(-2(i - x) + 2(j-f(x))df(x))
	 * \_____________first______________/\________second______________/
	 */
	float first = 1 / (2 * sqrt((i_minus_x*i_minus_x) + (j_minus_func*j_minus_func)));
	float second = -2*i_minus_x + -2 * (data_.j - funcval) * data_.df(data_.f_data, x);

	return first * second;
}

float generic_derivative(float (*f)(void *data, float x), void *f_data, float x, float delta) {
	return f(f_data, x + delta) - f(f_data, x - delta) / (2 * delta);
}

// helper function for newtons method
float newtons_inner(float (*f)(void *data, float x), float (*df)(void *data, float x), void *f_data, size_t n_itter, float x0) {
	float xprev2 = x0;
	float xprev = x0+0.001;
	float xnext = x0;

	for(size_t i = 0; i < n_itter; i++) {
		//printf("x: %f, delta: %f ", xnext, fabs(xprev - xprev2)*2);
		float df_;
		if(df == NULL) {
			df_ = generic_derivative(f, f_data, xprev, fabs(xprev - xprev2));
			//float df_ = generic_derivative(f, f_data, xprev, 0.5);
		} else {
			df_ = df(f_data, xprev);
		}
#if 0
		xnext = (xprev * df_ - f(xprev))/df_;
#else
		xnext = xprev - f(f_data, xprev) / df_;
#endif

		//printf("derivative: %f next: %f\n", df_, xnext);

		if(isfinite(xnext) && xnext != xprev) {
			xprev2 = xprev;
			xprev = xnext;
		} else {
			// divide by 0... take previous value
			return xprev;
		}
	}

	return xnext;
}

void newtons(float (*f)(void *data, float x), float (*df)(void *data, float x), void *f_data, size_t n_itter, float *guesses, size_t n_guess) {
	for(size_t i = 0; i < n_guess; i++) {
		guesses[i] = newtons_inner(f, df, f_data, n_itter, guesses[i]);
	}
}

