#include "main.h"

float scalex = 0.37f;
float scaley = 0.37f;
float posx = 0.375f;
float posy = 0.375f;

const int milli = 1000;
char is_init = (char)1;

void init_level() {

	e_player *p = &leveldata.player;

	*p = E_PLAYER_PROTOTYPE;
	p->is_active = 0;
	vector_scale(&p->jump_vec, max_jump);

	// set player radius
	circle player_bounds = {.r=0.015};
	// set position based on radius
	p->start_pos = (vector){.x=-0.87,.y=player_bounds.r,.z=0};

	//set players physics position
	p->proj.pos0 = p->start_pos;
	p->proj.pos  = p->start_pos;
	p->proj.vel0 = ZERO_VECTOR;
	p->proj.vel  = ZERO_VECTOR;

	//set player physics initial state
	p->proj.reset_start = 1;
	p->proj.is_dynamic = 1;

	// set players collision position
	p->bounds = player_bounds;
	p->bounds.c = (vector){
		.x=p->proj.pos.x,
		.y=p->proj.pos.y,
		.z=p->proj.pos.z,
	};

	p->t = malloc(sizeof(*p->t));
	assert(p->t != NULL);

	p->is_t_on_heap = 1;
	*p->t = (trajectory){
		.is_dynamic = 1,
		.player = p,
		.flight_time = 0,
		.is_points_on_heap = 0,
		.n_points = 0,
		.max_points = 0,
		.points = NULL,
	};

	leveldata.player_start_state = leveldata.player;





	// malloc cars
	unsigned int ncars = 4;
	leveldata.cars = malloc(sizeof(*leveldata.cars) * ncars);
	assert(leveldata.cars != NULL);
	leveldata.n_cars = 4;

	leveldata.cars[0] = E_CAR_PROTOTYPE;
	leveldata.cars[1] = E_CAR_PROTOTYPE;
	leveldata.cars[2] = E_CAR_PROTOTYPE;
	leveldata.cars[3] = E_CAR_PROTOTYPE;

	init_vector(&leveldata.cars[0].pos, -0.7, 0.025, 0);
	init_vector(&leveldata.cars[1].pos, -0.58, 0.025, 0);
	init_vector(&leveldata.cars[2].pos, -0.44, 0.025, 0);
	init_vector(&leveldata.cars[3].pos, -0.32, 0.025, 0);

	leveldata.cars[0].height = 0.1f;
	leveldata.cars[1].height = 0.18f;
	leveldata.cars[2].height = 0.12f;
	leveldata.cars[3].height = 0.08f;

	leveldata.cars[0].width = 0.05f;
	leveldata.cars[1].width = 0.05f;
	leveldata.cars[2].width = 0.05f;
	leveldata.cars[3].width = 0.05f;





	// malloc water
	size_t nlogs = 4;
	leveldata.water = malloc(sizeof(*leveldata.water) + sizeof(e_log) * nlogs);
	assert(leveldata.water != NULL);
	*leveldata.water = E_WATER_PROTOTYPE;

	leveldata.water->bounds.c = (vector){.x=0.375, .y=-0.1, .z=0};
	leveldata.water->bounds.w = 0.75;
	leveldata.water->bounds.h = 0.2;
	leveldata.water->nlogs = nlogs;

	leveldata.water->shape = (sin_data){.a=0.5, .b=5, .c=g.time, .d=0.5};

	leveldata.water->logs[0] = E_LOG_PROTOTYPE;
	leveldata.water->logs[1] = E_LOG_PROTOTYPE;
	leveldata.water->logs[2] = E_LOG_PROTOTYPE;
	leveldata.water->logs[3] = E_LOG_PROTOTYPE;

	leveldata.water->logs[0].shape.c.x = 0.7;
	leveldata.water->logs[1].shape.c.x = 0.58;
	leveldata.water->logs[2].shape.c.x = 0.44;
	leveldata.water->logs[3].shape.c.x = 0.32;

	leveldata.water->logs[0].shape.r=0.03;
	leveldata.water->logs[1].shape.r=0.042;
	leveldata.water->logs[2].shape.r=0.025;
	leveldata.water->logs[3].shape.r=0.05;





	// malloc terrain
	unsigned int nterrain_points = 10;
	leveldata.terrain = malloc(sizeof(*leveldata.terrain) + sizeof(*leveldata.terrain->vertices) * nterrain_points);
	assert(leveldata.terrain != NULL);
	*leveldata.terrain = E_WALL_PROTOTYPE;

	leveldata.terrain->n_vertices = nterrain_points;
	init_vector(&leveldata.terrain->vertices[0], -1, 0, 0);
	init_vector(&leveldata.terrain->vertices[1], -0.75, 0, 0);
	init_vector(&leveldata.terrain->vertices[2], -0.75, 0.025, 0);
	init_vector(&leveldata.terrain->vertices[3], -0.25, 0.025, 0);
	init_vector(&leveldata.terrain->vertices[4], -0.25, 0, 0);
	init_vector(&leveldata.terrain->vertices[5], 0, 0, 0);
	init_vector(&leveldata.terrain->vertices[6], -0, -0.2, 0);
	init_vector(&leveldata.terrain->vertices[7], 0.75, -0.2, 0);
	init_vector(&leveldata.terrain->vertices[8], 0.75, 0, 0);
	init_vector(&leveldata.terrain->vertices[9], 1, 0, 0);

	// malloc terrain collision
	leveldata.terrain->n_boxes = 4;
	leveldata.terrain->box_collision = malloc(sizeof(*leveldata.terrain->box_collision) * leveldata.terrain->n_boxes);
	assert(leveldata.terrain->box_collision != NULL);
	leveldata.terrain->is_collision = 1;
	leveldata.terrain->n_boxes = 4;

	vector centre0 = {.x=-0.5, .y=-0.5, .z=0};//main left quadrant
	vector centre1 = {.x=-0.5, .y=0.0125, .z=0};//road
	vector centre2 = {.x=0.5, .y=-0.6, .z=0};//main right quadrant
	vector centre3 = {.x=0.875, .y=-0.1, .z=0};//right-most bank

	leveldata.terrain->box_collision[0] = (box){.c=centre0, .h=1, .w=1};//main left quadrant
	leveldata.terrain->box_collision[1] = (box){.c=centre1, .h=0.025, .w=0.5};//road
	leveldata.terrain->box_collision[2] = (box){.c=centre2, .h=0.8, .w=1};
	leveldata.terrain->box_collision[3] = (box){.c=centre3, .h=0.2, .w=0.25};




	// setup generic gameobject array with all objects
	size_t water_and_terrain = 2;
	size_t cars_offset = water_and_terrain;
	size_t logs_offset = cars_offset + leveldata.n_cars;

	leveldata.n_objects = logs_offset + leveldata.water->nlogs;
	leveldata.objects = malloc(sizeof(*leveldata.objects) * leveldata.n_objects);
	assert(leveldata.objects != NULL);
	leveldata.objects[0] = (e_gameobject *)leveldata.water;
	leveldata.objects[1] = (e_gameobject *)leveldata.terrain;
	for(size_t i = 0; i < leveldata.n_cars; i++) {
		leveldata.objects[i+cars_offset] = (e_gameobject *)&leveldata.cars[i];
	}
	for(size_t i = 0; i < leveldata.water->nlogs; i++) {
		leveldata.objects[i+logs_offset] = (e_gameobject *)&leveldata.water->logs[i];
	}


}

void update(void) {
	static float last_t;

	// update the time
	g.time = glutGet(GLUT_ELAPSED_TIME) / (float)milli - g.start_time;

	if(is_init) {
		is_init = (char)0;

		init_level();

		g.draw_box_collision = (char)0;

		//3 degrees (in radians)

		last_t = g.time;

	}

	g.real_dt = g.time - last_t;
	if(g.pause) {
		g.dt = 0;
		g.start_time += g.real_dt;
	} else {
		g.dt = g.real_dt;
	}
	last_t = g.time;

	// update water
	leveldata.water->shape.c += g.dt;
	update_water_logs(leveldata.water);


	handle_keys();

	e_player *p = &leveldata.player;
	//player stuff
	if(p->is_dead && p->respawn) {
		*p = leveldata.player_start_state;
		p->t->is_dynamic = 1;
	}

	if(!p->is_active) {
		p->proj.vel = p->jump_vec;
		p->proj.vel0 = p->jump_vec;

		// handle attaching the player
		if(p->attached_to != NULL) {
			void (*attach_func)(e_player *p, e_gameobject *obj) = 
				gameobj_attach_func[p->attached_to->type];

			//move the player according to the thing its attached to
			attach_func(p, p->attached_to);
			p->proj.pos0 = p->proj.pos;
			p->bounds.c = p->proj.pos;
			p->t->is_dynamic = 1;//dirty the trajectory redraw
		}
	}

	// handle jumping
	if(p->jump) {
		p->jump = 0;
		p->is_active = 1;
	}

	//player position
	if(p->is_active) {
		p->attached_to = NULL;
		// check if trajectory is dynamic
		if(g.i_mode == analytical) {
			if(p->proj.reset_start || p->jump) {
				p->proj.start_time = g.time;
				p->proj.pos0 = p->proj.pos;
				p->proj.vel0 = p->proj.vel;
				p->proj.reset_start = 0;
			}
			updateProjectileStateAnalytical(&p->proj, g.time);
		} else if(g.i_mode == numerical) {
			p->proj.reset_start = 0;
			updateProjectileStateNumerical(&p->proj, g.dt);
		} else {
			perror("invalid integration mode");
			updateProjectileStateNumerical(&p->proj, g.dt);
		}
		p->bounds.c = p->proj.pos;
		p->jump = 0;
	}

	// player collision
	if(p->t->is_dynamic || p->is_active) {

		update_trajectory(p->t, leveldata.objects, leveldata.n_objects, 0.01);

		if(p->is_active) {
			char has_intersected = 0;
			for(size_t i = 0; i < leveldata.n_objects; i++) {
				// get the intersection function for this gameobject type
				char (*intersect_func)(e_player *p, e_gameobject *obj) = gameobj_intersect_func[leveldata.objects[i]->type];

				if(intersect_func == NULL) {
					// can't intersect with this
					continue;
				}

				if(intersect_func(p, leveldata.objects[i])) {

					
#ifndef PRINT_COLLIDES
#define PRINT_COLLIDES 0
#endif
#if PRINT_COLLIDES
					switch(leveldata.objects[i]->type) {
						case t_ewater:
							printf("collide with water\n");
							break;
						case t_eplayer:
							printf("collide with player\n");
							break;
						case t_elog:
							printf("collide with log\n");
							break;
						case t_ecar:
							printf("collide with car\n");
							break;
						case t_ewall:
							printf("collide with wall\n");
							break;
					}
#endif

					// intersected with a car
					if(leveldata.objects[i]->type == t_ecar
							|| leveldata.objects[i]->type == t_ewater) {
						p->is_dead = 1;
					}

					has_intersected = 1;

					// rewind time to just before the collision
					updateProjectileStateNumerical(&p->proj, -1*g.dt);
					p->bounds.c = p->proj.pos;

					if(gameobj_attach_func[leveldata.objects[i]->type] != NULL) {
						// we can attach to this object... don't just be static
						p->attached_to = leveldata.objects[i];
					}

					p->proj.vel = p->jump_vec;
					p->proj.vel0 = p->jump_vec;
					p->t->is_dynamic = 1;// trigger a trajectory update
					p->is_active = 0;

					break;
				}
			}
		}
	}






	// redraw the screen
	glutPostRedisplay();
}

#define VSYNC 1

void display() {

	//printf("time: %5.1f\n", g.time);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	if(!is_init) {


		// draw terrain
		glColor3f(1, 1, 1);
		draw_wall(leveldata.terrain, 0);

		//player
		if(leveldata.player.is_dead) {
			glColor3f(1,0,0);//dead
		} else if(g.flymode) {
			glColor3f(0,1,1);//super power cyan
		} else {
			glColor3f(0,1,0);//green
		}
		float angle = atan2f(leveldata.player.proj.vel.y, leveldata.player.proj.vel.x);
		oval player_oval = {
			.c=leveldata.player.bounds.c,
			.r=leveldata.player.bounds.r,
			.a=1,
			.b=0.5,
		};
		draw_oval(&player_oval, angle * 180/PI, g.tess, g.drawfill);


		//trajectroy and vector
		if(!leveldata.player.is_dead) {
			glColor3f(1, 1, 1);
			draw_trajectory(leveldata.player.t);
			drawVector(leveldata.player.proj.vel, leveldata.player.proj.pos, 1, 0);
		}


		// cars
		glColor3f(1,1,0);
		for(int i = 0; i < leveldata.n_cars; i++) {
			draw_car(&leveldata.cars[i], g.drawfill);
		}

		// draw water
		glColor3f(0,1,1);
		draw_water_distance(leveldata.water, &leveldata.player.bounds, g.tess, g.drawfill, wd_water);

		//draw logs
		glColor3f(0.2,0.2,0.2);
		for(size_t i = 0; i < leveldata.water->nlogs; i++) {
			draw_circle(&leveldata.water->logs[i].shape, g.tess, g.drawfill);
		}

		//collisions
		if(g.draw_box_collision) {
			//water
			draw_box(&leveldata.water->bounds, 0);

			//terrain
			glPushMatrix();
			for(size_t i = 0; i < leveldata.terrain->n_boxes; i++) {
				float red = (i % 3) / 3.0;
				float green = ((i+1) % 6) / 6.0;
				float blue = ((i+2) % 9) / 9.0;
				glColor3f(red, green, blue);
				draw_box(&leveldata.terrain->box_collision[i], (char)1);
			}
			glPopMatrix();
			glColor3f(1,1,1);
		}

	}


#if VSYNC
	//vsync
	glutSwapBuffers();
#else
	glFlush();
#endif


	// print out errors
	int err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("display: %s\n", gluErrorString(err));
	}

}

void free_leveldata() {
	free_trajectory(leveldata.player.t);
	free(leveldata.player.t);

	//free cars
	free(leveldata.cars);
	leveldata.n_cars = 0;

	//free water
	free(leveldata.water);

	//free terrain
	if(leveldata.terrain->is_collision) {
		free(leveldata.terrain->box_collision);
	}
	free(leveldata.terrain);
}

void handle_keys() {
	if(keys & kw) {
		if(!leveldata.player.is_active) {
			vector *change_vec = &leveldata.player.jump_vec;
			leveldata.player.t->is_dynamic = 1;//trigger dynamic update
			float change_vec_len = LENGTHVEC(*change_vec);
			vector_normalize(change_vec);
			vector_scale(change_vec, change_vec_len + g.velocity_change * g.real_dt);
		} else if(g.flymode) {
			vector *change_vec = &leveldata.player.proj.vel;
			change_vec->y += g.velocity_change * g.real_dt;
		}
	}
	if(keys & ks) {
		if(!leveldata.player.is_active) {
			vector *change_vec = &leveldata.player.jump_vec;
			leveldata.player.t->is_dynamic = 1;//trigger dynamic update
			float change_vec_len = LENGTHVEC(*change_vec);
			vector_normalize(change_vec);
			vector_scale(change_vec, change_vec_len - g.velocity_change * g.real_dt);
		} else if(g.flymode) {
			vector *change_vec = &leveldata.player.proj.vel;
			change_vec->y -= g.velocity_change * g.real_dt;
		}
	}
	if(keys & ka) {
		if(!leveldata.player.is_active) {
			vector *change_vec = &leveldata.player.jump_vec;
			leveldata.player.t->is_dynamic = 1;//trigger dynamic update
			vector_rotate_xy(change_vec, g.rotate_angle * g.real_dt);
		} else if(g.flymode) {
			vector *change_vec = &leveldata.player.proj.vel;
			change_vec->x -= g.velocity_change * g.real_dt;
		}
	}
	if(keys & kd) {
		if(!leveldata.player.is_active) {
			vector *change_vec = &leveldata.player.jump_vec;
			leveldata.player.t->is_dynamic = 1;//trigger dynamic update
			vector_rotate_xy(change_vec, -1 * g.rotate_angle * g.real_dt);
		} else if(g.flymode) {
			vector *change_vec = &leveldata.player.proj.vel;
			change_vec->x += g.velocity_change * g.real_dt;
		}
	}
}


void keyboardUp(unsigned char key, int x, int y) {
	switch(key) {
		case 'w':
			keys &= ~kw;
			break;
		case 's':
			keys &= ~ks;
			break;
		case 'a':
			keys &= ~ka;
			break;
		case 'd':
			keys &= ~kd;
			break;
	}
}

void keyboard(unsigned char key, int x, int y) {
	if(leveldata.player.is_dead) {
		switch(key) {
			case 'r':
				leveldata.player.respawn = 1;
				break;
			case 27:
			case 'q':
				free_leveldata();
				exit(EXIT_SUCCESS);
				break;
			case 'g':
				g.pause = !g.pause;
				break;
		}
	} else {
		switch(key) {
			case 'g':
				g.pause = !g.pause;
				break;
			case 27:
			case 'q':
				free_leveldata();
				exit(EXIT_SUCCESS);
				break;
			case '+':
			case '=':
				g.tess *= 2;
				break;
			case '-':
				g.tess /= 2;
				if(g.tess < 4) {
					g.tess = 4;
				}
				break;
			case 'm':
				// toggle draw box collision
				g.draw_box_collision = !g.draw_box_collision;
				break;
			case 'f':
				g.drawfill = !g.drawfill;
				break;
			case 'i':
				if(g.i_mode == analytical) {
					g.i_mode = numerical;
				} else if(g.i_mode == numerical) {
					g.i_mode = analytical;
				} else {
					perror("invalid integration mode");
				}
				break;
			case 'n':
				g.flymode = !g.flymode;
				break;
			case ' ':
				if(g.flymode) {
					leveldata.player.is_active = !leveldata.player.is_active;
				} else if(!leveldata.player.is_active) {
					leveldata.player.jump = 1;
				}
				leveldata.player.t->is_dynamic = 1;//trigger dynamic update
				break;
			case 'w':
				keys |= kw;
				break;
			case 'a':
				keys |= ka;
				break;
			case 'd':
				keys |= kd;
				break;
			case 's':
				keys |= ks;
				break;
			default://every other key
				break;
		}
	}
}

void init() {
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, (GLUT_SCREEN_HEIGHT * 2 / GLUT_SCREEN_WIDTH) - 1, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
#if VSYNC
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
#else
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
#endif
	glutCreateWindow("frogger cross section");

	g.start_time = glutGet(GLUT_ELAPSED_TIME) / (float)milli;
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutIdleFunc(update);
	glutMainLoop();

}
