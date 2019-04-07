#include "intersect.h"




char circle_point_is_intersect(circle *c, vector *point) {
	return c->r > distance_vector(&c->c, point);
}

char circle_is_intersect(circle *c1, circle *c2) {
	return c1->r + c2->r > distance_vector(&c1->c, &c2->c);
}



char circle_box_is_intersect(circle *c, box *b) {
	vector nearest_point;
	nearest_point.z = 0;
	if(c->c.x < b->c.x) {
		nearest_point.x = fmax(c->c.x, b->c.x - b->w/2);
	} else {
		nearest_point.x = fmin(c->c.x, b->c.x + b->w/2);
	}
	if(c->c.y < b->c.y) {
		nearest_point.y = fmax(c->c.y, b->c.y - b->h/2);
	} else {
		nearest_point.y = fmin(c->c.y, b->c.y + b->h/2);
	}

	return circle_point_is_intersect(c, &nearest_point);
}

