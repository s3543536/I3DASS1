#ifndef INTERSECT_GAMEOBJECTS
#define INTERSECT_GAMEOBJECTS

#include <assert.h>
#include <stdio.h>

#include "global.h"
#include "physics.h"
#include "vector.h"
#include "shapes.h"
#include "intersect.h"
#include "gameobjects.h"

// array of function ptrs to the intersect function for each type
// use the gameobject_type enum as array values
// 0 t_eplayer
// 1 t_ecar
// 2 t_elog
// 3 t_ewater
// 4 t_ewall
const char (*gameobj_intersect_func[5])(e_player *p, e_gameobject *obj);
const char is_gameobj_dynamic[5];
const void (*gameobj_attach_func[5])(e_player *p, e_gameobject *obj);

/*
 * these functions are used for gameobj_intersect_func.
 * make sure they fit that definition
 */
char player_water_is_intersect(e_player *p, e_gameobject *obj);
char player_wall_is_intersect(e_player *p, e_gameobject *obj);
char player_car_is_intersect(e_player *p, e_gameobject *obj);
char player_log_is_intersect(e_player *p, e_gameobject *obj);

/*
 * these functions are used for gameobj_attach_func
 * make sure they fit that definition
 */
void log_attach(e_player *p, e_gameobject *obj);
void water_attach(e_player *p, e_gameobject *obj);

struct trajectory {
	char is_dynamic;// has the end point landed on something dynamic?
	e_player *player;
	float flight_time;
	char is_points_on_heap;
	size_t n_points;
	size_t max_points;
	projectile *points;
};

// mallocs
void update_trajectory(trajectory *t, e_gameobject **objects, size_t n_objects, float time_step);
void free_trajectory(trajectory *t);




#endif
