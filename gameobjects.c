#include "gameobjects.h"


void update_water_logs(e_water *w) {

	for(size_t i = 0; i < w->nlogs; i++) {
		circle log_circle = w->logs[i].shape;

		//apply translation in reverse
		log_circle.c.x -= w->bounds.c.x;
		log_circle.c.y -= w->bounds.c.y;
		log_circle.c.z -= w->bounds.c.z;

		//apply scale in reverse
		log_circle.c.x *= 1/(0.5*w->bounds.w);
		log_circle.c.y *= 1/(0.5*w->bounds.h);

		//apply function
		log_circle.c.y = sin_x(&w->shape, log_circle.c.x);

		//re-apply scale
		log_circle.c.y *= (0.5*w->bounds.h);
		//re-apply translation
		log_circle.c.y += w->bounds.c.y;

		w->logs[i].shape.c.y = log_circle.c.y;
	}
}

