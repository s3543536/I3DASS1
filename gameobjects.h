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
	gameobject_type type;
} e_gameobject;

// forward declare a trajectory
// so player and trajectory can point to each-other
typedef struct trajectory trajectory;

//every game object can be cast to a e_gameobject
typedef struct {
	gameobject_type type;
	char is_active;
	e_gameobject *attached_to;
	projectile proj;
	char jump;
	vector jump_vec;
	char is_t_on_heap;
	trajectory *t;
	circle bounds;
} e_player;
#define E_PLAYER_PROTOTYPE (e_player){ \
	.type = t_eplayer, \
	.is_active = 1, \
	.attached_to = NULL, \
	.proj=PROJECTILE_PROTOTYPE, \
	.jump = 0, \
	.jump_vec = UNIT_VECTOR, \
	.is_t_on_heap = 0, \
	.bounds=(circle){ \
		.r = 0.05, \
		.c = {.x=0,.y=0.5,.z=0}, \
	}, \
}; \

typedef struct {
	gameobject_type type;
	vector pos;
	float height;
	float width;
} e_car;
#define E_CAR_PROTOTYPE (e_car){ \
	.type = t_ecar, \
	.pos = ZERO_VECTOR, \
	.height = 0, \
	.width = 0, \
}; \

typedef struct {
	gameobject_type type;
	circle shape;
} e_log;
#define E_LOG_PROTOTYPE (e_log){ \
	.type = t_elog, \
	.shape = (circle){.r=0.5,.c=ZERO_VECTOR}, \
}; \

typedef struct {
	gameobject_type type;
	box bounds;
	float depth;
	sin_data shape;
	size_t nlogs;
	e_log logs[0];
} e_water;
#define E_WATER_PROTOTYPE (e_water){ \
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
	gameobject_type type;
	char is_collision;
	size_t n_boxes;
	box *box_collision;
	char is_vertices_on_heap;
	size_t n_vertices;
	vector vertices[0];
} e_wall;
#define E_WALL_PROTOTYPE (e_wall){ \
	.type = t_ewall, \
	.is_collision = 1, \
	.n_boxes = 0, \
	.box_collision = NULL, \
	.n_vertices = 0, \
	.is_vertices_on_heap = 0, \
}; \



#endif
