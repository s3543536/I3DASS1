#include "glgameobjects.h"

void draw_car(e_car *car) {
	glPushMatrix();
	glTranslatef(car->pos.x, car->pos.y, car->pos.z);

	glBegin(GL_LINE_STRIP);
	// bottom left
	glVertex3f((car->width/2 - car->width), 0, 0);
	//top left
	glVertex3f((car->width/2 - car->width), car->height, 0);
	//top right
	glVertex3f((car->width/2), car->height, 0);
	//bottom right
	glVertex3f((car->width/2), 0, 0);
	glEnd();
	glPopMatrix();
}
