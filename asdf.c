#include <stdlib.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void plot(float x, float *y) {
	*y = x * x*x;
}

void drawAxes(int draw_negative) {
	//red
	glColor3f(1.0f, 0.0f, 0.0f);
	//x-axis
	glBegin(GL_LINES);
	if(draw_negative)
		glVertex3f(-1.0f, 0.0f, 0.0f);
	else
		glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	//green
	glColor3f(0.0f, 1.0f, 0.0f);
	//y-axis
	if(draw_negative)
		glVertex3f(0.0f, -1.0f, 0.0f);
	else
		glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	//blue
	glColor3f(0.0f, 0.0f, 1.0f);
	//z-axis
	if(draw_negative)
		glVertex3f(0.0f, 0.0f, -1.0f);
	else
		glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//white
	glColor3f(1, 1, 1);

	glBegin(GL_LINE_STRIP);
	float y = 0;
	for(float i = -1; i < 1.1f; i+= 0.1) {
		plot(i, &y);
		glVertex3f(i, y, 0.0f);
	}
	glEnd();

	drawAxes(0);


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
