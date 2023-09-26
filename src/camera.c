#include "camera.h"

#include "matrix.h"


static camera_t g_Camera;

void InitCamera(vec3_t position, vec3_t direction)
{
    g_Camera.position = position;
    g_Camera.direction = direction;
    g_Camera.forwardVelocity = vec3New(0.0f, 0.0f, 0.0f);
    g_Camera.yaw = 0.0f;
    g_Camera.pitch = 0.0;
}

vec3_t GetCameraPosition(void)
{
    return g_Camera.position;
}

vec3_t GetCameraDirection(void)
{
    return g_Camera.direction;
}

vec3_t GetCameraForwardVelocity(void)
{
    return g_Camera.forwardVelocity;
}

float GetCameraYaw(void)
{
    return g_Camera.yaw;
}

float GetCameraPitch(void)
{
    return g_Camera.pitch;
}

void UpdateCameraPosition(vec3_t position)
{
    g_Camera.position = position;
}
void UpdateCameraDirection(vec3_t direction)
{
    g_Camera.direction = direction;
}

void UpdateCameraForwardVelocity(vec3_t forwardVelocity)
{
    g_Camera.forwardVelocity = forwardVelocity;
}

void rotateCameraYaw(float angle)
{
    g_Camera.yaw += angle;
}

void rotateCameraPitch(float angle)
{
    g_Camera.pitch += angle;
}

vec3_t GetCameraLookAtTarget(void)
{
    vec3_t target = { 0.0f, 0.0f, 1.0f };

    mat4_t cameraYawRotation = mat4MakeRotationY(g_Camera.yaw);
    mat4_t cameraPitchRotation = mat4MakeRotationX(g_Camera.pitch);

    mat4_t cameraRotation = mat4Identity();
    cameraRotation = mat4Mulmat4(cameraPitchRotation, cameraRotation);
    cameraRotation = mat4Mulmat4(cameraYawRotation, cameraRotation);

    vec4_t cameraDirection = mat4Mulvec4(cameraRotation, vec4FromVec3(target));
    g_Camera.direction = vec3FromVec4(cameraDirection);

    target = vec3Add(g_Camera.position, g_Camera.direction);

    return target;
}