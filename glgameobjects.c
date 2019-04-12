#include "glgameobjects.h"

void draw_car(e_car *car, char filled) {
	glPushMatrix();
	glTranslatef(car->pos.x, car->pos.y, car->pos.z);

	if(filled) {
		glBegin(GL_QUADS);
	} else {
		glBegin(GL_LINE_STRIP);
	}
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

void draw_wall(e_wall *wall, char filled) {
	if(filled) {
		for(size_t i = 0; i < wall->n_boxes; i++) {
			draw_box(&wall->box_collision[i], 1);
		}
	} else {
		glBegin(GL_LINE_STRIP);
		for(size_t i = 0; i < wall->n_vertices; i++) {
				float x = wall->vertices[i].x;
				float y = wall->vertices[i].y;
				float z = wall->vertices[i].z;
				glVertex3f(x, y, z);
		}
		glEnd();
	}
}

void draw_water_distance(e_water *water, circle *player, char filled, water_distance_opts opts) {

	if(opts == wd_water) {
		return draw_water(water, filled);
	}
	//printf("water height: %f\n", water->bounds.h);

	// create a local copy that we can modify
	sin_data waterdata = water->shape;
	circle player_circle = *player;

	//apply translation
	player_circle.c.x -= water->bounds.c.x;
	player_circle.c.y -= water->bounds.c.y;
	player_circle.c.z -= water->bounds.c.z;

	//apply scale
	player_circle.c.x *= 1/(0.5*water->bounds.w);
	player_circle.c.y *= 1/(0.5*water->bounds.h);
	player_circle.r *= 1/(0.5*water->bounds.w);
	// can't apply both scales to the radius, so
	// we compensate for height scale here
	//waterdata.a *= 1/(water->bounds.h/water->bounds.w);

	f_dist_data fdd = {.i=player_circle.c.x, .j=player_circle.c.y, .f=sin_x, .df=dsin_x, .f_data=&waterdata};


	float roots[] = {-0.1, -0.2, -0.3, -0.4, -0.5, -0.6, -0.7, -0.8, -0.9, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
	size_t n_roots = sizeof(roots) / sizeof(*roots);
	newtons(f_dist, f_dist_derivative, &fdd, 8, roots, n_roots);

	float min_val = FLT_MAX;
	float min_x = 0;
	for(size_t i = 0; i < n_roots; i++) {
		roots[i] = fmin(roots[i], 1);
		roots[i] = fmax(roots[i], -1);
		float current_val = f_dist(&fdd, roots[i]);
		//float val_at_current = fdd.f(fdd.f_data, roots[i]);

		if(current_val < 0 || !isfinite(current_val) || !isfinite(roots[i])) continue;
		min_val = fmin(min_val, current_val);
		min_x = roots[i];
	}




	glPushMatrix();

	glTranslatef(water->bounds.c.x, water->bounds.c.y, water->bounds.c.z);
	glScalef(0.5*water->bounds.w, 0.5*water->bounds.h, 1);

	if((opts & wd_water) != 0) {
		// draw water
		draw_2d_function(fdd.f, fdd.f_data, 1, 1, filled);
	}

	if((opts & wd_closest) != 0) {
		// draw closest point
		circle nearest = {.r=0.1f, .c=(vector){.x=min_x, .y=fdd.f(fdd.f_data, min_x), .z=0}};
		draw_circle(&nearest, 10, filled);
	}

	if((opts & wd_distance) != 0) {
		// draw distance
		draw_2d_function(f_dist, &fdd, 1, 1, filled);
	}

	if((opts & wd_deriv) != 0) {
		// draw distance derivative
		draw_2d_function(f_dist_derivative, &fdd, 1, 1, filled);
	}
	glPopMatrix();
}



void draw_water(e_water *water, char filled) {

	sin_data waterdata = water->shape;
	//waterdata.a *= 1/(water->bounds.h/water->bounds.w);
	//waterdata.a *= water->bounds.w*0.5;

	glPushMatrix();
	glTranslatef(water->bounds.c.x, water->bounds.c.y, water->bounds.c.z);
	glScalef(0.5*water->bounds.w, 0.5*water->bounds.h, 1);

	draw_2d_function(sin_x, &waterdata, 1, 1, filled);

	glPopMatrix();
}
