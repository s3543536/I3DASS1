#ifndef GLGAMEOBJECTS
#define GLGAMEOBJECTS

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <float.h>

#include "gameobjects.h"
#include "shapes.h"
#include "glshapes.h"
#include "mathfunc.h"
#include "glfunc.h"

void draw_car(e_car *car, char filled);

typedef enum {
	wd_water = 1,
	wd_closest = 2,
	wd_distance = 4,
	wd_deriv = 8
} water_distance_opts;

// draws the water and the various functions used to calculate distance
void draw_water_distance(e_water *water, circle *player, unsigned int tess, char filled, water_distance_opts opts);

// much simpler, just draws the water
void draw_water(e_water *water, unsigned int tess, char filled);

void draw_wall(e_wall *wall, char filled);

#endif
