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

typedef struct {
	char is_dynamic;
	gameobject_type type;
	vector pos;
	float height;
	float width;
} e_car;

typedef struct {
	char is_dynamic;
	gameobject_type type;
	float radius;
} e_log;

typedef struct {
	char is_dynamic;
	gameobject_type type;
	box bounds;
	float depth;
	sin_data shape;
	size_t nlogs;
	e_log logs[0];
} e_water;

typedef struct {
	char is_dynamic;
	gameobject_type type;
	char is_collision;
	size_t n_boxes;
	box *box_collision;
	size_t n_vertices;
	vector vertices[0];
} e_wall;



#endif
