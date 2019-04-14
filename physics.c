#include "physics.h"


void updateProjectileStateNumerical(projectile *p, float dt) {
	// add the change over the frame time
	p->pos.y += p->vel.y * dt;
	p->pos.x += p->vel.x * dt;

	p->vel.y += -1*gravity * dt;
}

