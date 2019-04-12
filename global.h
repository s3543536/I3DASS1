#ifndef GLOBAL
#define GLOBAL

#include <stdlib.h>

typedef enum { analytical, numerical } integrationMode;

struct global_t {
	float time;
	float dt;
	float start_time;
	float rotate_angle;
	size_t tess;
	char drawfill;
	char flymode;
	char draw_box_collision;
	integrationMode i_mode;
	char OSD;//on screen display
};

struct global_t g;

const float gravity;

const float max_jump;

#endif
