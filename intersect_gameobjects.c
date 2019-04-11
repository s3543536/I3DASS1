#include "intersect_gameobjects.h"


const char (*gameobj_intersect_func[5])(e_player *p, e_gameobject *obj) = {
	NULL,//player
	NULL,//car
	NULL,//log
	player_water_is_intersect,//water
	NULL,//wall
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
			t->max_points = t->n_points;
		} else {// set the size

			temp = realloc(t->points, sizeof(*t->points) * points);
			if(points < t->n_points) {
				t->n_points = points;// removing points
			}
			// set new max number of points
			t->max_points = points;
		}

		if(temp == NULL) {
			perror("realloc trajectory points failed\n");
			// reset it
			free(t->points);
			t->is_points_on_heap = 0;
			t->n_points = 0;
			t->max_points = 0;
			return 0;
		} else {
			// success!
			t->points = temp;
			return 1;
		}
	}
}

void update_trajectory(trajectory *t, e_gameobject **objects, size_t n_objects, float time_step) {
	/*
	 * x = x0 + v0*t + 0.5*a*t^2                            newtons constant acceleration equation for position
	 * 0 = 0.5*a*t^2 + v0*t - dx
	 * 0 = ax^2      + bx   + c                             layout for quadratic formula
	 * x = (-b +-sqrt(b^2 - 4*a*c))/2*a                     quadratic formula
	 * t = ((-1*v0) +-sqrt((v0)^2 - 4*0.5*a*dx))/2*0.5*a    equation to find max time from top of level to bottom
	 */
	float dx = -2;//fall from top of level to bottom of level
	// assuming gravity is facing downwards
	float pos_neg = sqrt(max_jump*max_jump - 4*0.5*-1*gravity*dx);
	float max_time = (-1 * max_jump + pos_neg)/2*0.5*-1*gravity;
	if(max_time < 0) {
		max_time = (-1 * max_jump - pos_neg)/2*0.5*-1*gravity;
	}
	if(max_time < 0) {
		perror("time to fall to bottom is negative, check the equation");
		return;
	} else if(!isfinite(max_time)) {
		perror("max time calculation");
	}

	if(!alloc_trajectory(t)) {
		printf("no alloc\n");
		return;
	}
	printf("alloced\n");

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
			} else {
				// no intersect, add this point and move on
				if(t->n_points = t->max_points) {
					if(!realloc_trajectory(t, t->max_points * 2)) {
						perror("can't add another point!\n");
						return;
					}
				}

				t->points[current_point] = player.proj;
				t->n_points = ++current_point;
			}
		}
	}

	printf("total trajectory time: %5.2f has_intersected: %d\n", total_time, has_intersected);
}

char player_water_is_intersect(e_player *p, e_gameobject *obj) {
	e_water *w = (e_water *)obj;
	if(!circle_box_is_intersect(&p->bounds, &w->bounds)) {
		//player is outside the bounds, no chance of intersect
		return 0;
	}



	oval player_oval = *(oval *)&p->bounds;
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
