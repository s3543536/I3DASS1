#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "vector.h"
#include "glvector.h"
#include "mathfunc.h"
#include "glfunc.h"
#include "shapes.h"
#include "glshapes.h"
#include "intersect.h"
#include "gameobjects.h"
#include "glgameobjects.h"


typedef enum { analytical, numerical } integrationMode;

struct global_t {
	float time;
	float dt;
	float start_time;
	float draw_box_collision;
	integrationMode i_mode;
	char OSD;//on screen display
};

struct global_t g;

typedef struct {
	char reset_start;
	float start_time;
	vector pos0;
	vector pos;
	vector vel0;
	vector vel;
} projectile;

struct leveldata {
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
