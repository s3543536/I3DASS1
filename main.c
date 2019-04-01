#include "main.h"

#ifndef PI
#define PI 3.14159f
#endif

float scalex = 0.37f;
float scaley = 0.37f;
float posx = 0.375f;
float posy = 0.375f;



void draw_circle(float r, float x, float y, unsigned int nvertex, char filled) {
	if(filled) {
		glBegin(GL_POLYGON);
	} else {
		glBegin(GL_LINE_LOOP);
	}

	float angle = 0;
	for(int i = 0; i < nvertex; i++) {
		angle = i * 2 * PI / nvertex;
		glVertex3f(x + sin(angle) * r, y + cos(angle) * r, 0);
	}
	glEnd();
}

typedef struct {
	float a;
	float b;
	float c;
	float d;
} sin_data;
/** sin function
 *
 * @param sin_data *data    constains the co-efficients of the function
 *                            y = asin(bx + c) + d;
 * @param x                 x value of function
 * @param flaot *y          y value of function will be put here
 */
void sin_x(void *data, float x, float *y) {
	sin_data _data = *(sin_data*)data;
	*y = _data.a * sinf(_data.b * x + _data.c) + _data.d;
}

typedef struct {
	float a;
	float c;
} x_cubed_data;
/** x^3 function
 *
 * @param x_cubed_data *data    constains the co-efficients of the function
 *                                y = ax^3 + c
 * @param x                     x value of function
 * @param flaot *y              y value of function will be put here
 */
void x_cubed(void *data, float x, float *y) {
	x_cubed_data _data = *(x_cubed_data*)data;
	*y = _data.a*x*x*x + _data.c;
}

void drawAxes(float length, int draw_negative) {
	//red
	glColor3f(1, 0, 0);
	//x-axis
	glBegin(GL_LINES);
	if(draw_negative)
		glVertex3f(-1 * length, 0, 0);
	else
		glVertex3f(0, 0, 0);
	glVertex3f(length, 0, 0);

	//green
	glColor3f(0, 1, 0);
	//y-axis
	if(draw_negative)
		glVertex3f(0, -1 * length, 0);
	else
		glVertex3f(0, 0, 0);
	glVertex3f(0, length, 0);

	//blue
	glColor3f(0, 0, 1);
	//z-axis
	if(draw_negative)
		glVertex3f(0, 0, -1 * length);
	else
		glVertex3f(0, 0, 0);
	glVertex3f(0, 0, length);
	glEnd();
}

void draw_2d_function(void (*f)(void *data, float x, float *y), void *pass_thr, float x_scale, float y_scale) {
	glBegin(GL_LINE_STRIP);
	float y = 0;
	for(float x = -1; x < 1; x += 0.1f * x_scale) {
		f(pass_thr, x / x_scale, &y);
		glVertex3f(x, y * y_scale, 0.0f);
	}
	f(pass_thr, 1 / x_scale, &y);
	glVertex3f(1, y * y_scale, 0.0f);
	glEnd();
}

void draw_2d_function_normals(void (*f)(void *data, float x, float *y), void *pass_thr, float x_scale, float y_scale) {
	float y1;
	float y2;
	float dx = 0.001f;
	for(float x = -1; x < 1; x += 0.1f * x_scale) {
		//work out 2 (close) values
		f(pass_thr, x / x_scale, &y1);
		f(pass_thr, (x + dx) / x_scale, &y2);

		//work out the position and tangent
		vector tangent = {.x = dx, .y = y2 - y1, .z = 0};
		vector pos = {.x = x, .y = y1, .z = 0};

		// work out the normal
		vector normal = {.x = -1 * tangent.y, .y = tangent.x, .z = 0};
		// in 2d, we don't need the full blown cross product:
		// y = mx + b (m is gradient)
		// m = dy/dx
		// m1 * m2 = -1
		// m2 = -1/m1
		// m2 = -1 / (dy / dx) = -dx / dy

		//draw the tangent and normal
		drawVector(tangent, pos, 0.1f, (char)1);
		drawVector(normal, pos, 0.1f, (char)1);
	}
}

#ifndef CAR_HEIGHT
#define CAR_HEIGHT 0.6f
#endif
#ifndef CAR_WIDTH
#define CAR_WIDTH 1.0f
#endif

void draw_car(float height, vector offset, vector scale) {
	glBegin(GL_LINE_STRIP);
	// bottom left
	glVertex3f((CAR_WIDTH/2 - CAR_WIDTH) * scale.x + offset.x, 0 * scale.y + offset.y, 0 + offset.z);
	//top left
	glVertex3f((CAR_WIDTH/2 - CAR_WIDTH) * scale.x + offset.x, CAR_HEIGHT * scale.y + offset.y, 0 + offset.z);
	//top right
	glVertex3f((CAR_WIDTH/2) * scale.x + offset.x, CAR_HEIGHT * scale.y + offset.y, 0 + offset.z);
	//bottom right
	glVertex3f((CAR_WIDTH/2) * scale.x + offset.x, 0 * scale.y + offset.y, 0 + offset.z);
	glEnd();
}

const int milli = 1000;
char is_init = (char)1;

void init_level() {
	// malloc cars
	unsigned int ncars = 4;
	leveldata.cars = malloc(sizeof(*leveldata.cars) * ncars);
	leveldata.is_cars_on_heap = leveldata.cars != NULL;
	if(leveldata.cars == NULL) {
		perror("can't malloc leveldata.cars\n");
	} else {
		init_vector(&leveldata.cars[0].pos, -0.7, 0.03, 0);
		init_vector(&leveldata.cars[1].pos, -0.58, 0.03, 0);
		init_vector(&leveldata.cars[2].pos, -0.44, 0.03, 0);
		init_vector(&leveldata.cars[3].pos, -0.2, 0.03, 0);

		leveldata.cars[0].height = 0.6f;
		leveldata.cars[1].height = 0.6f;
		leveldata.cars[2].height = 0.6f;
		leveldata.cars[3].height = 0.6f;
	}

	// malloc water
	unsigned int nlogs = 4;
	leveldata.water = malloc(sizeof(*leveldata.water) + sizeof(e_log) * nlogs);
	leveldata.is_water_on_heap = leveldata.cars != NULL;
	if(leveldata.water == NULL) {
		perror("can't malloc leveldata.water\n");
	} else {
		init_vector(&leveldata.water->bottom_left, 0.5, 0, 0);
		init_vector(&leveldata.water->top_right, 0.6, 0.1, 0);
		leveldata.water->depth = 0.8f;
		leveldata.water->nlogs = nlogs;

		leveldata.water->logs[0].radius = 0.3f;
		leveldata.water->logs[1].radius = 0.42f;
		leveldata.water->logs[2].radius = 0.25f;
		leveldata.water->logs[3].radius = 0.5f;
	}

	// malloc terrain
	unsigned int nterrain_points = 10;
	leveldata.terrain = malloc(sizeof(*leveldata.terrain) + sizeof(vector) * nterrain_points);
	leveldata.is_terrain_on_heap = leveldata.terrain != NULL;
	if(leveldata.terrain == NULL) {
		perror("can't malloc leveldata.terrain\n");
	} else {
		leveldata.terrain->nvertices = nterrain_points;
		init_vector(&leveldata.terrain->vertices[0], -1, 0, 0);
		init_vector(&leveldata.terrain->vertices[1], -0.75, 0, 0);
		init_vector(&leveldata.terrain->vertices[2], -0.75, 0.025, 0);
		init_vector(&leveldata.terrain->vertices[3], -0.25, 0.025, 0);
		init_vector(&leveldata.terrain->vertices[4], -0.25, 0, 0);
		init_vector(&leveldata.terrain->vertices[5], 0, 0, 0);
		init_vector(&leveldata.terrain->vertices[6], -0, -0.06, 0);
		init_vector(&leveldata.terrain->vertices[7], 0.75, -0.06, 0);
		init_vector(&leveldata.terrain->vertices[8], 0.75, 0, 0);
		init_vector(&leveldata.terrain->vertices[9], 1, 0, 0);
	}
}

const float gravity = -0.2f;

void updateProjectileStateAnalytical(projectile *p) {
	// newtons constant acceleration equations
	// x = x0 + v0*t + a * t^2 / 2
	float dt = g.time - p->start_time;

	// update vertical position
	p->pos.y = p->pos0.y + p->vel0.y * dt + (gravity * dt*dt / 2);
	// update vertical velocity
	p->vel.y = p->vel0.y + gravity * dt;

	// update horizontal position
	p->pos.x = p->pos0.x + p->vel0.x * dt;
}

void updateProjectileStateNumerical(projectile *p) {
	// add the change over the frame time
	p->pos.y += p->vel.y * g.dt;
	p->pos.x += p->vel.x * g.dt;

	p->vel.y += gravity * g.dt;
}

projectile p;
char activep = (char)0;

void update(void) {
	static float lastT;

	// update the time
	g.time = glutGet(GLUT_ELAPSED_TIME) / (float)milli - g.start_time;

	if(is_init) {
		is_init = (char)0;
		
		lastT = g.time;

		init_level();

		init_vector(&p.pos0, 0,0,0);
		init_vector(&p.pos, 0,0,0);
		init_vector(&p.vel0, 0,0,0);
		init_vector(&p.vel, 0,0,0);
		p.start_time = g.time;

		g.i_mode = numerical;
	}

	g.dt = g.time - lastT;
	lastT = g.time;

	if(g.i_mode == analytical) {
		updateProjectileStateAnalytical(&p);
	} else if(g.i_mode == numerical) {
		updateProjectileStateNumerical(&p);
	} else {
		perror("invalid integration mode");
		updateProjectileStateNumerical(&p);
	}

	// redraw the screen
	glutPostRedisplay();
}

#define VSYNC 1

void display() {

	//printf("time: %5.1f\n", g.time);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//white
	glColor3f(1, 1, 0);

	glBegin(GL_POINTS);
	glVertex3f(p.pos.x, p.pos.y, p.pos.z);
	glEnd();

	glColor3f(1, 1, 1);

	vector car_offset = {.x = 0.3, .y = -0.5, .z = 0};
	vector car_scale = {.x = 0.2, .y = 0.5, .z = 0.2};
	draw_car(1, car_offset, car_scale);

	draw_circle(0.5f, 0, 0, 3, (char)0);
	draw_circle(0.3f, 0.5f, 0.3f, (unsigned int)g.time%10, (char)1);

	//x_cubed_data;
	sin_data fdata = {.a = 1, .b = 1, .c = g.time, .d = 0};

	glPushMatrix();

	glTranslatef(0.375, 0, 0);
	glScalef(0.37, 0.5f, 0.5f);
	draw_2d_function(&sin_x, &fdata, 1 / 3.14159f, 1);

	glColor3f(0, 0, 1);
	draw_2d_function_normals(&sin_x, &fdata, 1 / 3.14159f, 1);
	drawAxes(1, 0);

	glPopMatrix();

	// use a stack for all transformations
	/* glPushMatrix(); */
	/* glPopMatrix(); */

	if(!is_init) {
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < leveldata.terrain->nvertices; i++) {
			float x = leveldata.terrain->vertices[i].x;
			float y = leveldata.terrain->vertices[i].y;
			float z = leveldata.terrain->vertices[i].z;
			glVertex3f(x, y, z);
		}
		glEnd();
	}

#if VSYNC
	//vsync
	glutSwapBuffers();
#else
	glFlush();
#endif


	// print out errors
	int err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("display: %s\n", gluErrorString(err));
	}

}

void keyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 27:
		case 'q':
			exit(EXIT_SUCCESS);
			break;
		case 'h':
			// decrement posx and print
			posx -= 0.01f;
			printf("posx: %5.3f\n", posx);
			break;
		case 'j':
			// decrement scalex and print
			posy -= 0.01f;
			printf("scalex: %5.3f\n", scalex);
			break;
		case 'k':
			// increment scalex and print
			posy += 0.01f;
			printf("scalex: %5.3f\n", scalex);
			break;
		case 'l':
			// increment posx and print
			posx += 0.01f;
			printf("posx: %5.3f\n", posx);
			break;
		case 'w':
			p.vel.y = 0.2;
			break;
		case 'a':
			p.vel.x -= 0.05;
			break;
		case 'd':
			p.vel.x += 0.05;
			break;
		case 's':
			activep = (char)1;
			break;
		default://every other key
			break;
	}
}

void init() {
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, (GLUT_SCREEN_HEIGHT * 2 / GLUT_SCREEN_WIDTH) - 1, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
#if VSYNC
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
#else
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
#endif
	glutCreateWindow("openGL hello world");

	g.start_time = glutGet(GLUT_ELAPSED_TIME) / (float)milli;
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);
	glutMainLoop();

}
