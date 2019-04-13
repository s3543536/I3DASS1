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
	e_player player_start_state;
	size_t n_cars;
	e_car *cars;
	e_water *water;
	e_wall *terrain;
	size_t n_objects;
	e_gameobject **objects;//index of level objects
};

struct leveldata leveldata;

enum smooth_keys {
	kw = 1 << 0,
	ks = 1 << 1,
	ka = 1 << 2,
	kd = 1 << 3,
} keys;
//x-or with the thing you want to flip

void handle_keys();


/*
 * glTranslate(x, y, z);
 * glRotate(angle, x, y, z);//(xyz is a vector to rotate around)
 * glScale(x, y, z);
 * glLoadIdentity();
 */
