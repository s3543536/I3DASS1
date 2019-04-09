#include "main.h"

#define DRAW_CLOSEST_SIN_DIST 1
#define DRAW_SIN_DIST_FUNC 1
#define DRAW_SIN_DIST_DERIVATIVE_FUNC 0

float scalex = 0.37f;
float scaley = 0.37f;
float posx = 0.375f;
float posy = 0.375f;

const int milli = 1000;
char is_init = (char)1;

void init_level() {
	// malloc cars
	unsigned int ncars = 4;
	leveldata.cars = malloc(sizeof(*leveldata.cars) * ncars);
	leveldata.is_cars_on_heap = leveldata.cars != NULL;
	if(leveldata.cars == NULL) {
		perror("can't malloc leveldata.cars\n");
		leveldata.n_cars = 0;
	} else {
		init_vector(&leveldata.cars[0].pos, -0.7, 0.03, 0);
		init_vector(&leveldata.cars[1].pos, -0.58, 0.03, 0);
		init_vector(&leveldata.cars[2].pos, -0.44, 0.03, 0);
		init_vector(&leveldata.cars[3].pos, -0.2, 0.03, 0);

		leveldata.cars[0].height = 0.6f;
		leveldata.cars[1].height = 0.6f;
		leveldata.cars[2].height = 0.6f;
		leveldata.cars[3].height = 0.6f;

		leveldata.cars[0].width = 0.05f;
		leveldata.cars[1].width = 0.05f;
		leveldata.cars[2].width = 0.05f;
		leveldata.cars[3].width = 0.05f;
		
		leveldata.n_cars = 4;
	}

	// malloc water
	size_t nlogs = 4;
	leveldata.water = malloc(sizeof(*leveldata.water) + sizeof(e_log) * nlogs);
	leveldata.is_water_on_heap = leveldata.cars != NULL;
	if(leveldata.water == NULL) {
		perror("can't malloc leveldata.water\n");
	} else {
		leveldata.water->bounds.c = (vector){.x=0.375, .y=0, .z=0};
		leveldata.water->bounds.w = 0.37;
		leveldata.water->bounds.h = 0.5;
		//init_vector(&leveldata.water->bottom_left, 0.5, 0, 0);
		//init_vector(&leveldata.water->top_right, 0.6, 0.1, 0);
		leveldata.water->depth = 0.8f;
		leveldata.water->nlogs = nlogs;

		leveldata.water->shape = (sin_data){.a=0.2, .b=(10), .c=g.time, .d=0};

		//((sin_data*)fdd.f_data)->b = 1;

		leveldata.water->logs[0].radius = 0.3f;
		leveldata.water->logs[1].radius = 0.42f;
		leveldata.water->logs[2].radius = 0.25f;
		leveldata.water->logs[3].radius = 0.5f;
	}

	// malloc terrain
	unsigned int nterrain_points = 10;
	leveldata.terrain = malloc(sizeof(*leveldata.terrain) + sizeof(*leveldata.terrain->vertices) * nterrain_points);
	leveldata.is_terrain_on_heap = leveldata.terrain != NULL;
	if(leveldata.terrain == NULL) {
		perror("can't malloc leveldata.terrain\n");
	} else {
		leveldata.terrain->n_vertices = nterrain_points;
		init_vector(&leveldata.terrain->vertices[0], -1, 0, 0);
		init_vector(&leveldata.terrain->vertices[1], -0.75, 0, 0);
		init_vector(&leveldata.terrain->vertices[2], -0.75, 0.025, 0);
		init_vector(&leveldata.terrain->vertices[3], -0.25, 0.025, 0);
		init_vector(&leveldata.terrain->vertices[4], -0.25, 0, 0);
		init_vector(&leveldata.terrain->vertices[5], 0, 0, 0);
		init_vector(&leveldata.terrain->vertices[6], -0, -0.06, 0);
		init_vector(&leveldata.terrain->vertices[7], 0.75, -0.06, 0);
		init_vector(&leveldata.terrain->vertices[8], 0.75, 0, 0);
		init_vector(&leveldata.terrain->vertices[9], 1, 0, 0);
	}
	// malloc terrain collision
	leveldata.terrain->n_boxes = 4;
	leveldata.terrain->box_collision = malloc(sizeof(*leveldata.terrain->box_collision) * leveldata.terrain->n_boxes);
	leveldata.terrain->is_collision = leveldata.terrain->box_collision != NULL;
	if(!leveldata.terrain->is_collision) {
		perror("can't malloc leveldata.terrain->box_collision");
	} else {
		leveldata.terrain->n_boxes = 4;
		vector centre0 = {.x=-0.5, .y=-0.5, .z=0};//main left quadrant
		vector centre1 = {.x=-0.5, .y=0.0125, .z=0};//road
		vector centre2 = {.x=0.5, .y=-0.53, .z=0};//main right quadrant
		vector centre3 = {.x=0.875, .y=-0.03, .z=0};//right-most bank
		leveldata.terrain->box_collision[0] = (box){.c=centre0, .h=1, .w=1};//main left quadrant
		leveldata.terrain->box_collision[1] = (box){.c=centre1, .h=0.025, .w=0.5};//road
		leveldata.terrain->box_collision[2] = (box){.c=centre2, .h=0.94, .w=1};
		leveldata.terrain->box_collision[3] = (box){.c=centre3, .h=0.06, .w=0.25};
	}
}

const float gravity = -0.2f;

void updateProjectileStateAnalytical(projectile *p) {
	// newtons constant acceleration equations
	// x = x0 + v0*t + a * t^2 / 2
	float dt = g.time - p->start_time;

	// update vertical position
	p->pos.y = p->pos0.y + p->vel0.y * dt + (gravity * dt*dt / 2);
	// update vertical velocity
	p->vel.y = p->vel0.y + gravity * dt;

	// update horizontal position
	p->pos.x = p->pos0.x + p->vel0.x * dt;
}

void updateProjectileStateNumerical(projectile *p) {
	// add the change over the frame time
	p->pos.y += p->vel.y * g.dt;
	p->pos.x += p->vel.x * g.dt;

	p->vel.y += gravity * g.dt;
}

projectile p;
char activep = (char)0;

void update(void) {
	static float lastT;

	// update the time
	g.time = glutGet(GLUT_ELAPSED_TIME) / (float)milli - g.start_time;

	if(is_init) {
		is_init = (char)0;
		
		lastT = g.time;

		printf("init level\n");
		init_level();

		g.draw_box_collision = (char)0;


		init_vector(&p.pos0, 0,0.5,0);
		init_vector(&p.pos, 0,0.5,0);
		init_vector(&p.vel0, 0,0,0);
		init_vector(&p.vel, 0,0,0);
		p.start_time = g.time;

		g.i_mode = numerical;
	}

	g.dt = g.time - lastT;
	lastT = g.time;

	if(g.i_mode == analytical) {
		if(p.reset_start) {
			p.start_time = g.time;
			p.pos0 = p.pos;
			p.vel0 = p.vel;
			p.reset_start = 0;
		}
		updateProjectileStateAnalytical(&p);
	} else if(g.i_mode == numerical) {
		updateProjectileStateNumerical(&p);
	} else {
		perror("invalid integration mode");
		updateProjectileStateNumerical(&p);
	}

	// update water
	leveldata.water->shape.c = g.time;


	//player collision circle
	circle pj = {.r=0.05, .c={.x=p.pos.x, .y=p.pos.y, .z=p.pos.z}};

	// collide with cars
	for(int i = 0; i < leveldata.n_cars; i++) {
		box car = {.c=leveldata.cars[i].pos,.h=leveldata.cars[i].height, .w=leveldata.cars[i].width};
		//bounding box pos is the centre, car pos is the center on the bottom
		car.c.y += car.h/2;
		if(circle_box_is_intersect(&pj, &car)) {
			printf("%4.2f circle is intersecting with car %d\n", g.time, i);
		}
	}

	// collide with walls
	for(int i = 0; i < leveldata.terrain->n_boxes; i++) {
		if(circle_box_is_intersect(&pj, &leveldata.terrain->box_collision[i])) {
			printf("%4.2f circle is intersecting with wall %d\n", g.time, i);
		}
	}


	//glTranslatef(0.375, 0, 0);
	//glScalef(0.37, 0.5f, 0.5f);
	// collide with water
	sin_data waterdata = leveldata.water->shape;
	circle player_circle = pj;
	player_circle.c.x -= 0.375;
	player_circle.c.x *= 1/0.37;
	player_circle.c.y *= 1/0.5;
	player_circle.r *= 1/0.37;
	waterdata.a *= 1/(0.5/0.37);
	if(circle_func_is_intersect(&player_circle, sin_x, dsin_x, &waterdata)) {
		printf("%4.2f circle is intersecting with wave\n", g.time);
	}

	// redraw the screen
	glutPostRedisplay();
}

#define VSYNC 1

void display() {

	//printf("time: %5.1f\n", g.time);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//white
	glColor3f(1, 1, 0);

	glBegin(GL_POINTS);
	glVertex3f(p.pos.x, p.pos.y, p.pos.z);
	glEnd();

	glColor3f(1, 1, 1);

	// draw circle around player
	circle pj = {.r=0.05, .c={.x=p.pos.x, .y=p.pos.y, .z=p.pos.z}};
	draw_circle(&pj, 10, (char)0);

	if(!is_init) {
		for(int i = 0; i < leveldata.n_cars; i++) {
			draw_car(&leveldata.cars[i]);
		}

		if(g.draw_box_collision) {
			glPushMatrix();
			for(size_t i = 0; i < leveldata.terrain->n_boxes; i++) {
				float red = (i % 3) / 3.0;
				float green = ((i+1) % 3) / 3.0;
				float blue = ((i+2) % 3) / 3.0;
				glColor3f(red, green, blue);
				draw_box(&leveldata.terrain->box_collision[i], (char)1);
			}
			glPopMatrix();
			glColor3f(1,1,1);
		}
	}

#if 0
	vector triangle_pos = {.x=0, .y=0.5f, .z=0};
	circle triangle = {.r=0.5f, .c=triangle_pos};
	vector acircle_pos = {.x=0.5f, .y=0.3f, .z=0};
	circle acircle = {.r=0.3f, .c=acircle_pos};
	draw_circle(&triangle, 3, (char)0);
	draw_circle(&acircle, (unsigned int)g.time%10, (char)1);
#endif

	

	if(!is_init) {



		// draw water and distance to it
		draw_water_distance(leveldata.water, &pj, wd_water );
		//draw_water(leveldata.water);





		//glPushMatrix();

		//glTranslatef(0.375, 0, 0);
		//glScalef(0.37, 0.5f, 0.5f);
		//draw_2d_function(sin_x, &leveldata.water->shape, 1 / 3.14159f, 1);

		//glColor3f(0, 0, 1);
		//draw_2d_function_normals(sin_x, &leveldata.water->shape, 1 / 3.14159f, 1);
		//drawAxes(1, 0);

		//glPopMatrix();
	}

	glColor3f(1,1,1);
	// draw road
	if(!is_init) {
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < leveldata.terrain->n_vertices; i++) {
			float x = leveldata.terrain->vertices[i].x;
			float y = leveldata.terrain->vertices[i].y;
			float z = leveldata.terrain->vertices[i].z;
			glVertex3f(x, y, z);
		}
		glEnd();
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
	//free cars
	if(leveldata.is_cars_on_heap) {
		free(leveldata.cars);
	}

	//free water
	if(leveldata.is_water_on_heap) {
		free(leveldata.water);
	}

	//free terrain
	if(leveldata.is_terrain_on_heap) {
		if(leveldata.terrain->is_collision) {
			free(leveldata.terrain->box_collision);
		}
		free(leveldata.terrain);
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 27:
		case 'q':
			free_leveldata();
			exit(EXIT_SUCCESS);
			break;
		case 'm':
			// toggle draw box collision
			g.draw_box_collision = !g.draw_box_collision;
			break;
		case 'h':
			// decrement posx and print
			posx -= 0.01f;
			printf("posx: %5.3f\n", posx);
			break;
		case 'j':
			// decrement scalex and print
			posy -= 0.01f;
			printf("scalex: %5.3f\n", scalex);
			break;
		case 'k':
			// increment scalex and print
			posy += 0.01f;
			printf("scalex: %5.3f\n", scalex);
			break;
		case 'l':
			// increment posx and print
			posx += 0.01f;
			printf("posx: %5.3f\n", posx);
			break;
		case 'i':
			if(g.i_mode == analytical) {
				g.i_mode = numerical;
			} else if(g.i_mode == numerical) {
				g.i_mode = analytical;
			} else {
				perror("invalid integration mode");
			}
		case 'w':
			p.vel.y = 0.2;
			p.reset_start = (char)1;
			break;
		case 'a':
			p.vel.x -= 0.05;
			p.reset_start = (char)1;
			break;
		case 'd':
			p.vel.x += 0.05;
			p.reset_start = (char)1;
			break;
		case 's':
			activep = (char)1;
			p.reset_start = (char)1;
			break;
		default://every other key
			break;
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
	glutCreateWindow("openGL hello world");

	g.start_time = glutGet(GLUT_ELAPSED_TIME) / (float)milli;
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);
	glutMainLoop();

}
