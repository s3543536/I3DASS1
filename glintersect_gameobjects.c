#include "glintersect_gameobjects.h"


void draw_trajectory(trajectory *t) {
	if(!t->is_points_on_heap) {
		return;
	}

	vector *last = NULL;
	glBegin(GL_LINE_STRIP);
	for(size_t i = 0; i < t->n_points; i++) {
		vector *current = &t->points[i].pos;
		if(last != NULL) {
			glVertex3f(current->x, current->y, current->z);
		}
		last = current;
	}
	glEnd();
}
