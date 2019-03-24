#include "main.h"

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

const int milli = 1000;
float time_;
float start_time;

void update(void) {

	time_ = glutGet(GLUT_ELAPSED_TIME) / (float)milli - start_time;
}

void display() {

	//printf("time: %5.1f\n", time_);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//white
	glColor3f(1, 1, 1);

	//x_cubed_data;
	sin_data fdata = {.a = 1, .b = 1, .c = time_, .d = 0};

	draw_2d_function(&sin_x, &fdata, 1 / 3.14159f, 1);

	glColor3f(0, 0, 1);
	draw_2d_function_normals(&sin_x, &fdata, 1 / 3.14159f, 1);
	drawAxes(1, 0);

	//vsync
	glutSwapBuffers();

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
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("openGL hello world");

	start_time = glutGet(GLUT_ELAPSED_TIME) / (float)milli;
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);
	glutMainLoop();

}
