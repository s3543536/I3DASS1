#include "glfunc.h"


void draw_2d_function(float (*f)(void *data, float x), void *pass_thr, float x_scale, float y_scale, unsigned int tess, char filled) {
	if(filled) {
		glBegin(GL_QUAD_STRIP);
		float y = 0;
		for(float x = -1; x < 1; x += (2.0/tess) * x_scale) {
			glVertex3f(x, -1, 0.0f);

			y = f(pass_thr, x / x_scale);
			glVertex3f(x, y * y_scale, 0.0f);
		}
		y = f(pass_thr, 1 / x_scale);
		glVertex3f(1, -1, 0.0f);
		glVertex3f(1, y * y_scale, 0.0f);
		glEnd();
	} else {
		glBegin(GL_LINE_STRIP);
		float y = 0;
		for(float x = -1; x < 1; x += (2.0/tess) * x_scale) {
			y = f(pass_thr, x / x_scale);
			glVertex3f(x, y * y_scale, 0.0f);
		}
		y = f(pass_thr, 1 / x_scale);
		glVertex3f(1, y * y_scale, 0.0f);
		glEnd();
	}
}

void draw_2d_function_normals(float (*f)(void *data, float x), void *pass_thr, float x_scale, float y_scale) {
	float y1;
	float y2;
	float dx = 0.001f;
	for(float x = -1; x < 1; x += 0.1f * x_scale) {
		//work out 2 (close) values
		y1 = f(pass_thr, x / x_scale);
		y2 = f(pass_thr, (x + dx) / x_scale);

		//work out the position and tangent
		vector tangent = {.x = dx, .y = y2 - y1, .z = 0};
		vector pos = {.x = x, .y = y1, .z = 0};

		// work out the normal
		vector normal = {.x = -1 * tangent.y, .y = tangent.x, .z = 0};
		// in 2d, we don't need the full blown cross product:
		// y = mx + b (m is gradient)
		// m = dy/dx
		// m1 * m2 = -1
		// m2 = -1/m1
		// m2 = -1 / (dy / dx) = -dx / dy

		//draw the tangent and normal
		drawVector(tangent, pos, 0.1f, (char)1);
		drawVector(normal, pos, 0.1f, (char)1);
	}
}
