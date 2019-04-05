
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "vector.h"
#include "glvector.h"


#ifndef PI
#define PI 3.14159f
#endif

/** sin function
 * @param x value of x-axis on the sin function
 * @param y ptr to float where y value will be stored
 */
void sin_x(void *data, float x, float *y);
/** sin function
 * @param x value of x-axis on the x^3 function
 * @param y ptr to float where y value will be stored
 */
void x_cubed(void *data, float x, float *y);

typedef enum { analytical, numerical } integrationMode;

typedef struct {
	float r;//radius
	vector c;//centre
} circle;

void draw_circle(circle *c, unsigned int nvertex, char filled);

typedef struct {
	vector c;//centre
	float h;//height
	float w;//width
} box;

struct global_t {
	float time;
	float dt;
	float start_time;
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

typedef struct {
	vector pos;
	float height;
	float width;
} e_car;

void draw_car(e_car *car);

typedef struct {
	float radius;
} e_log;

typedef struct {
	vector bottom_left;
	vector top_right;
	float depth;
	unsigned int nlogs;
	e_log logs[0];
} e_water;

typedef struct {
	unsigned int nvertices;
	vector vertices[0];
} e_wall;

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
