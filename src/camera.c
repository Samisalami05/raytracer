#include "camera.h"
#include "vec3.h"
#include <stdbool.h>

Vec3 camera_forward(Camera cam) {
	return vec3_normalized(vec3_sub(cam.target, cam.pos));
}

Vec3 camera_up(Camera cam) {
	return vec3(0, 1, 0);
}

Vec3 camera_right(Camera cam) {
	Vec3 forward = camera_forward(cam);
	Vec3 up = camera_up(cam);

	return vec3_normalized(vec3_cross(forward, up));
}

void camera_move(Camera* cam, Vec3 move) {
	cam->pos = vec3_add(cam->pos, move);
	cam->target = vec3_add(cam->target, move);
}

void camera_move_forward(Camera* cam, float amount) {
	Vec3 forward = camera_forward(*cam);
	Vec3 move = vec3_scale(forward, amount);
	
	camera_move(cam, move);
}

void camera_move_right(Camera* cam, float amount) {
	Vec3 right = camera_right(*cam);
	Vec3 move = vec3_scale(right, amount);
	
	camera_move(cam, move);
}

void camera_move_up(Camera* cam, float amount) {
	Vec3 up = camera_up(*cam);
	Vec3 move = vec3_scale(up, amount);
	
	camera_move(cam, move);
}

// From raylib.h
void camera_yaw(Camera *cam, float angle)
{
    // Rotation axis
    Vec3 up = camera_up(*cam);

    // View vector
    Vec3 targetPosition = vec3_sub(cam->target, cam->pos);

    // Rotate view vector around up axis
    targetPosition = vec3_rotate_euler(targetPosition, up, angle);

    // Move target relative to position
    cam->target = vec3_add(cam->pos, targetPosition);
}

// From raylib.h
void camera_pitch(Camera *cam, float angle)
{
    // Up direction
    Vec3 up = camera_up(*cam);

    // View vector
    Vec3 targetPosition = vec3_sub(cam->target, cam->pos);

	bool lockView = true;
    if (lockView)
    {
        // In these camera modes, clamp the Pitch angle
        // to allow only viewing straight up or down

        // Clamp view up
        float maxAngleUp = vec3_angle(up, targetPosition);
        maxAngleUp -= 0.001f; // avoid numerical errors
        if (angle > maxAngleUp) angle = maxAngleUp;

        // Clamp view down
        float maxAngleDown = vec3_angle(vec3_inverse(up), targetPosition);
        maxAngleDown *= -1.0f; // downwards angle is negative
        maxAngleDown += 0.001f; // avoid numerical errors
        if (angle < maxAngleDown) angle = maxAngleDown;
    }

    // Rotation axis
    Vec3 right = camera_right(*cam);

    // Rotate view vector around right axis
    targetPosition = vec3_rotate_euler(targetPosition, right, angle);

    // Move target relative to position
    cam->target = vec3_add(cam->pos, targetPosition);
}
