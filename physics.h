#ifndef PHYSICS
#define PHYSICS

#include "vector.h"
#include "global.h"

typedef struct {
	char reset_start;
	char is_dynamic;//is the projectile in the air?
	float start_time;
	vector pos0;
	vector pos;
	vector vel0;
	vector vel;
} projectile;
#define PROJECTILE_PROTOTYPE (projectile){ \
	.reset_start = 0, \
	.is_dynamic = 0, \
	.start_time = 0, \
	.pos0 = ZERO_VECTOR, \
	.pos = ZERO_VECTOR, \
	.vel0 = UNIT_VECTOR, \
	.vel = UNIT_VECTOR, \
} \


/** update projectile position
 *		numerical approach, uses newtons constant acceleration equation (for change in time)
 *		to update position based on current velocity and change in time
 *
 *		@param projectile *p    the projectile to update
 *		@param float dt         change in time since last update
 */
void updateProjectileStateNumerical(projectile *p, float dt);


#endif
