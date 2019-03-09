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

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//white
	glColor3f(1, 1, 1);

	draw_2d_function(&sin_x, 1 / 3.14159f, 1);

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
