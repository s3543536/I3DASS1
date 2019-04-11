#ifndef GAMEOBJECTS
#define GAMEOBJECTS

#include "vector.h"
#include "shapes.h"
#include "mathfunc.h"
#include "physics.h"
#include "gameobjects.h"


typedef enum {
	t_eplayer,
	t_ecar,
	t_elog,
	t_ewater,
	t_ewall,
} gameobject_type;

typedef struct {
	char is_dynamic;
	gameobject_type type;
} e_gameobject;


//every game object can be cast to a e_gameobject
typedef struct {
	char is_dynamic;
	gameobject_type type;
	char is_active;
	projectile proj;
	circle bounds;
} e_player;
#define E_PLAYER_PROTOTYPE (e_player){ \
	.is_dynamic = 0, \
	.type = t_eplayer, \
	.is_active = 1, \
	.proj=(projectile){ \
		.reset_start = 1, \
		.is_dynamic = 1, \
		.start_time = 0, \
		.pos0= {.x=0,.y=0.5,.z=0}, \
		.pos=  {.x=0,.y=0.5,.z=0}, \
		.vel0=(vector){.x=0,.y=0  ,.z=0}, \
		.vel= (vector){.x=0,.y=0  ,.z=0}, \
	}, \
	.bounds=(circle){ \
		.r = 0.05, \
		.c = {.x=0,.y=0.5,.z=0}, \
	}, \
}; \

typedef struct {
	char is_dynamic;
	gameobject_type type;
	vector pos;
	float height;
	float width;
} e_car;
#define E_CAR_PROTOTYPE (e_car){ \
	.is_dynamic = 0, \
	.type = t_ecar, \
	.pos = ZERO_VECTOR, \
	.height = 0, \
	.width = 0, \
}; \

typedef struct {
	char is_dynamic;
	gameobject_type type;
	circle shape;
} e_log;
#define E_LOG_PROTOTYPE (e_log){ \
	.is_dynamic = 1, \
	.type = t_elog, \
	.shape = (circle){.r=0.5,.c=ZERO_VECTOR}, \
}; \

typedef struct {
	char is_dynamic;
	gameobject_type type;
	box bounds;
	float depth;
	sin_data shape;
	size_t nlogs;
	e_log logs[0];
} e_water;
#define E_WATER_PROTOTYPE (e_water){ \
	.is_dynamic = 1, \
	.type = t_ewater, \
	.bounds = (box){ \
		.c = ZERO_VECTOR, \
		.w = 1, \
		.h = 1, \
	}, \
	.depth = 0, \
	.shape = (sin_data){.a=1,.b=1,.c=0,.d=0,}, \
	.nlogs = 0, \
} \

typedef struct {
	char is_dynamic;
	gameobject_type type;
	char is_collision;
	size_t n_boxes;
	box *box_collision;
	char is_vertices_on_heap;
	size_t n_vertices;
	vector vertices[0];
} e_wall;
#define E_WALL_PROTOTYPE (e_wall){ \
	.is_dynamic = 0, \
	.type = t_ewall, \
	.is_collision = 1, \
	.n_boxes = 0, \
	.box_collision = NULL, \
	.n_vertices = 0, \
	.is_vertices_on_heap = 0, \
}; \



#endif
