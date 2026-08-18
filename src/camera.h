#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

typedef struct {
	Vec3 pos;
	Vec3 target;
	float fov;
} Camera;

Vec3 camera_forward(Camera cam);

void camera_move_forward(Camera* cam, float amount);
void camera_move_right(Camera* cam, float amount);
void camera_move_up(Camera* cam, float amount);
void camera_yaw(Camera *cam, float angle);
void camera_pitch(Camera *cam, float angle);

#endif
