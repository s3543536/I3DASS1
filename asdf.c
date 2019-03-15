#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void sin_x(float x, float *y) {
	*y = sinf(x);
}
void x_cubed(float x, float *y) {
	*y = x * x*x;
}

typedef struct vector {
	float x, y, z;
} vector;

void cross_vectors(vector v1, vector v2, vector *ret) {
	/*
		|i	j	k|
		|a	b	c| = i(b*f - c*e) - j(a*f - d*c) + k(a*e - d*b)
		|d	e	f|
	*/
	ret->x = v1.y * v2.z - v1.z * v2.y;
	ret->y = -1 * (v1.x * v2.z - v1.z * v2.x);
	ret->z = v1.x * v2.y - v1.y * v2.x;
}

#define LENGTHVEC(V) sqrt((V).x * (V).x + (V).y * (V).y + (V).z * (V).z)

void drawVector(vector v, vector pos, float s, char normalize) {
	if(v.x == 0 && v.y == 0 && v.z == 0) return;

	if(normalize) {
		float length = LENGTHVEC(v);
		v.x /= length;
		v.y /= length;
		v.z /= length;
	}

    glBegin(GL_LINES);
	glVertex3f(pos.x, pos.y, pos.z);
	glVertex3f(pos.x + v.x * s, pos.y + v.y * s, pos.z + v.z * s);
	glEnd();
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

void draw_2d_function(void (*f)(float x, float *y), float x_scale, float y_scale) {
	glBegin(GL_LINE_STRIP);
	float y = 0;
	for(float x = -1; x < 1; x += 0.1f * x_scale) {
		f(x / x_scale, &y);
		glVertex3f(x, y * y_scale, 0.0f);
	}
	f(1 / x_scale, &y);
	glVertex3f(1, y * y_scale, 0.0f);
	glEnd();
}

void draw_2d_function_normals(void (*f)(float x, float *y), float x_scale, float y_scale) {
	float y1;
	float y2;
	float dx = 0.001f;
	for(float x = -1; x < 1; x += 0.1f * x_scale) {
		//work out 2 (close) values
		f(x / x_scale, &y1);
		f((x + dx) / x_scale, &y2);

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

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	//white
	glColor3f(1, 1, 1);

	draw_2d_function(&sin_x, 1 / 3.14159f, 1);

	glColor3f(0, 0, 1);
	draw_2d_function_normals(&sin_x, 1 / 3.14159f, 1);
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

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

}
