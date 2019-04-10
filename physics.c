#include "physics.h"

void updateProjectileStateAnalytical(projectile *p, float time) {
	// newtons constant acceleration equations
	// x = x0 + v0*t + a * t^2 / 2
	float dt = time - p->start_time;

	// update vertical position
	p->pos.y = p->pos0.y + p->vel0.y * dt + (gravity * dt*dt / 2);
	// update vertical velocity
	p->vel.y = p->vel0.y + gravity * dt;

	// update horizontal position
	p->pos.x = p->pos0.x + p->vel0.x * dt;
}

void updateProjectileStateNumerical(projectile *p, float dt) {
	// add the change over the frame time
	p->pos.y += p->vel.y * dt;
	p->pos.x += p->vel.x * dt;

	p->vel.y += gravity * dt;
}

