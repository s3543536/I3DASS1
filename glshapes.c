#include "glshapes.h"


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

void draw_circle(circle *c, size_t nvertex, char filled) {
	if(filled) {
		glBegin(GL_POLYGON);
	} else {
		glBegin(GL_LINE_LOOP);
	}

	float angle = 0;
	for(size_t i = 0; i < nvertex; i++) {
		angle = i * 2 * PI / nvertex;
		glVertex3f(c->c.x + sin(angle) * c->r, c->c.y + cos(angle) * c->r, c->c.z);
	}
	glEnd();
}

