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

/** update projectile position
 *		analytical approach, uses newtons constant acceleration equation (for position)
 *		to update the position based on its start velocity and location
 *
 *		@param projectile *p    the projectile to update
 *		@param float time       the world time (should be before start time)
 */
void updateProjectileStateAnalytical(projectile *p, float time);

/** update projectile position
 *		numerical approach, uses newtons constant acceleration equation (for change in time)
 *		to update position based on current velocity and change in time
 *
 *		@param projectile *p    the projectile to update
 *		@param float dt         change in time since last update
 */
void updateProjectileStateNumerical(projectile *p, float dt);


#endif
