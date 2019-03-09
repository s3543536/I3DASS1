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

void cross_vectors(vector v1, vector v2, vector *ret) {
	/*
		|i	j	k|
		|a	b	c| = i(b*f - c*e) - j(a*f - d*c) + k(a*e - d*b)
		|d	e	f|
	*/
	ret->x = v1.y * v2.z - v1.z * v2.y;
	ret->y = -1 * (v1.x * v2.z - v1.z - v2.x);
	ret->z = v1.x * v2.y - v1.y * v2.x;
}

void draw_2d_function_normals(void (*f)(float x, float *y), float x_scale, float y_scale) {
	glBegin(GL_LINES);
	float y1;
	float y2;
	float dx = 0.001f;
	for(float x = -1; x < 1; x += 0.1f * x_scale) {
		f(x / x_scale, &y1);
		f((x + dx) / x_scale, &y2);
		vector tangent;
		tangent.x = dx;
		tangent.y = y2 - y1;
		tangent.z = 0;
		vector pos;
		pos.x = x;
		pos.y = y1;
		pos.z = 0;
		drawVector(tangent, pos, 100, (char)0);
		vector normal;
		vector unit_z;
		unit_z.x = 0;
		unit_z.y = 0;
		unit_z.z = 1;
		cross_vectors(tangent, unit_z, &normal);
		drawVector(normal, pos, 100, (char)0);
	}
	glEnd();
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


	//glBegin(GL_POINTS);
	//glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex2f(0.0f, 0.0f);
	//glEnd();

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
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("openGL hello world");

	init();

	glutDisplayFunc(display);
	//glutDisplayFunc(keyboard);
	glutMainLoop();

}
