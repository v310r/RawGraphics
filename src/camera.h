#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

typedef struct
{
    vec3_t position;
    vec3_t direction;
    vec3_t forwardVelocity;
    float yaw;
    float pitch;
} camera_t;

void InitCamera(vec3_t position, vec3_t direction);

vec3_t GetCameraPosition(void);
vec3_t GetCameraDirection(void);
vec3_t GetCameraForwardVelocity(void);
float GetCameraYaw(void);
float GetCameraPitch(void);

void UpdateCameraPosition(vec3_t position);
void UpdateCameraDirection(vec3_t direction);
void UpdateCameraForwardVelocity(vec3_t forwardVelocity);

void rotateCameraYaw(float angle);
void rotateCameraPitch(float angle);

vec3_t GetCameraLookAtTarget(void);

#endif // CAMERA_H