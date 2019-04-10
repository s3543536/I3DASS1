#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "global.h"
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
#include "glintersect_gameobjects.h"


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
