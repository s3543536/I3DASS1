#ifndef GLOBAL
#define GLOBAL

#include <stdlib.h>

struct global_t {
	float time;
	float dt;
	float real_dt;
	float start_time;
	char pause;
	float rotate_angle;
	float velocity_change;
	unsigned int tess;
	char drawfill;
	char flymode;
	char draw_box_collision;
	char OSD;//on screen display
};

struct global_t g;

const float gravity;

const float max_jump;

#endif
