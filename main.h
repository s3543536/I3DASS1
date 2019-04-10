#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "vector.h"
#include "glvector.h"
#include "shapes.h"
#include "physics.h"
#include "glshapes.h"
#include "mathfunc.h"
#include "glfunc.h"
#include "gameobjects.h"
#include "glgameobjects.h"
#include "intersect.h"
#include "intersect_gameobjects.h"


typedef enum { analytical, numerical } integrationMode;

struct global_t {
	float time;
	float dt;
	float start_time;
	char draw_box_collision;
	integrationMode i_mode;
	char OSD;//on screen display
};

struct global_t g;

struct leveldata {
	e_player player;
	char is_cars_on_heap;
	size_t n_cars;
	e_car *cars;
	char is_water_on_heap;
	e_water *water;
	char is_terrain_on_heap;
	e_wall *terrain;
};

struct leveldata leveldata;


/*
 * glTranslate(x, y, z);
 * glRotate(angle, x, y, z);//(xyz is a vector to rotate around)
 * glScale(x, y, z);
 * glLoadIdentity();
 */
