#include "mathfunc.h"


float sin_x(void *data, float x) {
	sin_data _data = *(sin_data*)data;
	return _data.a * sinf(_data.b * x + _data.c) + _data.d;
}

float x_cubed(void *data, float x) {
	x_cubed_data _data = *(x_cubed_data*)data;
	return _data.a*x*x*x + _data.c;
}


