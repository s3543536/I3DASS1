#include "intersect_gameobjects.h"


const char (*gameobj_intersect_func[5])(e_player *p, e_gameobject *obj) = {
	NULL,//player
	player_car_is_intersect,//car
	player_log_is_intersect,//log
	player_water_is_intersect,//water
	player_wall_is_intersect,//wall
};
const char is_gameobj_dynamic[5] = {
	0,
	0,
	1,
	1,
	0
};

char alloc_trajectory(trajectory *t) {
	if(!t->is_points_on_heap) {
		t->points = malloc(sizeof(*t->points) * 10);
		if(t->points == NULL) {
			perror("can't malloc trajectory points\n");
			//reset it
			t->n_points = 0;
			t->max_points = 0;
			return 0;
		}
		// just malloced, no points
		t->is_points_on_heap = 1;
		t->n_points = 0;
		t->max_points = 10;
		return 1;
	}
	return 1;
}

char realloc_trajectory(trajectory *t, size_t points) {
	if(!t->is_points_on_heap) {
		// not allocated, allocate it
		t->points = malloc(sizeof(*t->points) * points);
		if(t->points == NULL) {
			perror("can't malloc trajectory points\n");
			return 0;
		}
		t->is_points_on_heap = 1;
		t->n_points = 0;
		t->max_points = points;
		return 1;
	} else {

		void *temp = NULL;
		if(points == 0) {
			// just trim the end
			temp = realloc(t->points, sizeof(*t->points) * t->n_points);
			if(temp != NULL) {
				t->points = temp;
				t->max_points = t->n_points;
				return 1;
			} else {
				perror("trajectory resize to trim the end failed\n");
				return 0;
			}
		} else {// set the size

			temp = realloc(t->points, sizeof(*t->points) * points);
			if(points < t->n_points) {
				t->n_points = points;// removing points
			}
			// set new max number of points
			if(temp != NULL) {
				t->points = temp;
				t->max_points = points;
				return 1;
			} else {
				perror("trajectory resize failed\n");
				return 0;
			}
		}
	}
}

void update_trajectory(trajectory *t, e_gameobject **objects, size_t n_objects, float time_step) {
	float max_time = 10;
#if 0// this doesn't seem to work... it just gives me 0.8 max time
	/*
	 * x = x0 + v0*t + 0.5*a*t^2                            newtons constant acceleration equation for position
	 * 0 = 0.5*a*t^2 + v0*t - dx
	 * 0 = ax^2      + bx   + c                             layout for quadratic formula
	 * x = (-b +-sqrt(b^2 - 4*a*c))/2*a                     quadratic formula
	 */
	float c = -2;//bottom of level minus top
	float a = -0.5 * gravity;//gravity is already neegative
	float b = max_jump;
	float pos_neg = sqrt(fabs(b*b - 4*a*c));
	max_time = (-1 * b + pos_neg)/2*a;
	printf("4ac: %f\n",4*a*c);
	if(max_time < 0) {
		max_time = (-1 * b - pos_neg)/2*a;
	}
	if(max_time < 0) {
		perror("time to fall to bottom is negative, check the equation");
		return;
	} else if(!isfinite(max_time)) {
		perror("max time calculation");
	}
	printf("max time: %f\n", max_time);
	printf("posneg: %f\n", pos_neg);
#endif



	if(!alloc_trajectory(t)) {
		return;
	}



	// copy so we can move it in time and place
	e_player player = *t->player;

	float total_time = 0;

	size_t current_point = 0;
	char has_intersected = 0;
	while(!has_intersected && total_time < max_time) {// 1 loop per point
		total_time += time_step;

		// update the position
		if(g.i_mode == analytical) {
			updateProjectileStateAnalytical(&player.proj, total_time);
		} else {
			updateProjectileStateNumerical(&player.proj, time_step);
		}
		player.bounds.c = player.proj.pos;

		// 1 loop per object
		for(size_t i = 0; i < n_objects; i++) {
			// get the intersection function for this gameobject type
			char (*intersect_func)(e_player *p, e_gameobject *obj) = gameobj_intersect_func[objects[i]->type];
			if(intersect_func == NULL) {
				// can't intersect with this
				continue;
			}
			if(intersect_func(&player, objects[i])) {
				// intersected with this object
				has_intersected = 1;
				t->is_dynamic = is_gameobj_dynamic[objects[i]->type];
				t->flight_time = total_time;
				break;
			}
		}

		// no intersect, add this point and move on
		if(t->n_points == t->max_points) {
			if(!realloc_trajectory(t, t->max_points * 2)) {
				perror("can't add another point!\n");
				return;
			}
		}

		t->points[current_point] = player.proj;
		t->n_points = ++current_point;
	}

	//printf("total trajectory time: %5.2f has_intersected: %d n_points: %d\n", total_time, has_intersected, t->n_points);
}

char player_water_is_intersect(e_player *p, e_gameobject *obj) {
	e_water *w = (e_water *)obj;
	if(!circle_box_is_intersect(&p->bounds, &w->bounds)) {
		//player is outside the bounds, no chance of intersect
		return 0;
	}



	//oval player_oval = *(oval *)&p->bounds;
	oval player_oval = {.r=p->bounds.r, .c=p->bounds.c};
	/* ^^^ turning a circle into an oval here
	 * this is because the scaling (see below) is separate on y and x axes
	 * not because the frogs collision is an oval
	 */

	/* the water is scaled by the location and position of its bounding box
	 * we can't really scale the oval_func_is_intersect
	 * instead i will apply the reverse transformations to the player
	 */

	//apply translation in reverse
	player_oval.c.x -= w->bounds.c.x;
	player_oval.c.y -= w->bounds.c.y;
	player_oval.c.z -= w->bounds.c.z;

	//apply scale in reverse
	player_oval.c.x *= 1/(0.5*w->bounds.w);
	player_oval.c.y *= 1/(0.5*w->bounds.h);
	player_oval.a = 1/(0.5*w->bounds.w);
	player_oval.b = 1/(0.5*w->bounds.h);


	if(sin_x(&w->shape, player_oval.c.x) > player_oval.c.y) {
		//player center is below the sin wave, its under water
		return 1;
	}




	// the rest is to check if the circle intersects with the surface of water:
	if(oval_func_is_intersect(&player_oval, sin_x, dsin_x, &w->shape)) {
		// trying to avoid a tail call here so the stack vars don't go out of scope
		return 1;
	} else {
		return 0;
	}
}



char player_wall_is_intersect(e_player *p, e_gameobject *obj) {
	e_wall *wall = (e_wall *)obj;

	if(!wall->is_collision) {
		return 0;
	}

	for(int i = 0; i < wall->n_boxes; i++) {
		if(circle_box_is_intersect(&p->bounds, &wall->box_collision[i])) {
			return 1;
		}
	}
	return 0;
};

char player_car_is_intersect(e_player *p, e_gameobject *obj) {
	e_car *car = (e_car *)obj;

	box boxcar = {.c=car->pos,.h=car->height, .w=car->width};
	//bounding box pos is the centre, car pos is the center on the bottom
	boxcar.c.y += boxcar.h/2;
	if(circle_box_is_intersect(&p->bounds, &boxcar)) {
		return 1;
	}
	return 0;
}

char player_log_is_intersect(e_player *p, e_gameobject *obj) {
	e_log *log_obj = (e_log*)obj;

	return circle_is_intersect(&p->bounds, &log_obj->shape);
}
