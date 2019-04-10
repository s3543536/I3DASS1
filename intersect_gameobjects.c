#include "intersect_gameobjects.h"



char player_water_is_intersect(e_player *p, e_water *w) {
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
