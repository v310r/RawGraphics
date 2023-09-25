#include "camera.h"

camera_t g_Camera = 
{ 
    .position =  { .x = 0.0f, .y = 0.0f, .z = 0.0f },
    .direction = { .x = 0.0f, .y = 0.0f, .z = 1.0f },
    .forwardVelocity = {0.0f, 0.0f, 0.0f},
    .yaw = 0.0f
};