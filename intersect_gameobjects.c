#include "intersect_gameobjects.h"



char player_water_is_intersect(e_player *p, e_water *w) {
	//make local copies we can modify
	sin_data waterdata = w->shape;
	oval player_oval = *(oval *)&p->bounds;

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

	// try to avoid a tail call here so the stack vars don't go out of scope
	if(oval_func_is_intersect(&player_oval, sin_x, dsin_x, &waterdata)) {
		return 1;
	} else {
		return 0;
	}
}
