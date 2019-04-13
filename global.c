#include "global.h"

const float gravity = 0.2f;

const float max_jump = 0.25;

#ifndef PI
#define PI 3.14159
#endif

struct global_t g = {
	.rotate_angle = 20 * PI/180,//20 degres per sec
	.velocity_change = 0.3,//0.3 mps per sec
	.flymode = 1,
	.i_mode = numerical,
	.drawfill = 0,
	.tess = 20,
	.pause = 0,
};
