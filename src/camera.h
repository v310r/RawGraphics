#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

typedef struct
{
    vec3_t position;
    vec3_t direction;
    vec3_t forwardVelocity;
    float yaw;
} camera_t;

extern camera_t g_Camera;

#endif // CAMERA_H