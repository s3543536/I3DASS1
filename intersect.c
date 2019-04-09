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


char circle_func_is_intersect(circle *c, float (*f)(void *data, float x), float (*df)(void *data, float x), void *data) {
	// setup the distance function 
	f_dist_data fdd = {.i=c->c.x, .j=c->c.y, .f=f, .df=df, .f_data=data};


	// find the roots of the distance function (to get the minimum)
	float roots[] = {-0.1, -0.2, -0.3, -0.4, -0.5, -0.6, -0.7, -0.8, -0.9, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
	size_t n_roots = sizeof(roots) / sizeof(*roots);
	// TODO: find a good n_itter             V
	newtons(f_dist, f_dist_derivative, &fdd, 8, roots, n_roots);

	float min_val = FLT_MAX;
	float min_x = 0;
	for(size_t i = 0; i < n_roots; i++) {
		float current_val = f_dist(&fdd, roots[i]);
		//float val_at_current = fdd.f(fdd.f_data, roots[i]);

		if(current_val < 0 || !isfinite(current_val) || !isfinite(roots[i])) continue;
		min_val = fmin(min_val, current_val);
		min_x = roots[i];
	}

	//distance is less than c.r
	return min_val < c->r;
}

