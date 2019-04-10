#include "glvector.h"


void drawVector(vector v, vector pos, float s, char normalize) {
	if(v.x == 0 && v.y == 0 && v.z == 0) return;

	if(normalize) {
		float length = LENGTHVEC(v);
		v.x /= length;
		v.y /= length;
		v.z /= length;
	}

    glBegin(GL_LINES);
	glVertex3f(pos.x, pos.y, pos.z);
	glVertex3f(pos.x + v.x * s, pos.y + v.y * s, pos.z + v.z * s);
	glEnd();
}

