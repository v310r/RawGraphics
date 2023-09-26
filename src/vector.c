#include "vector.h"
#include <math.h>


vec2_t vec2New(float x, float y)
{
    vec2_t result = { x, y };
    return result;
}

float vec2Length(vec2_t v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2Add(vec2_t a, vec2_t b)
{
    vec2_t result = { a.x + b.x, a.y + b.y };
    return result;
}

vec2_t vec2Sub(vec2_t a, vec2_t b)
{
    vec2_t result = { a.x - b.x, a.y - b.y };
    return result;
}

vec2_t vec2Mul(vec2_t v, float factor)
{
    vec2_t result = { v.x * factor, v.y * factor };
    return result;
}

vec2_t vec2Div(vec2_t v, float factor)
{
    vec2_t result = { v.x / factor, v.y / factor };
    return result;
}

float vec2Dot(vec2_t a, vec2_t b)
{
    return a.x * b.x + a.y * b.y;
}

vec2_t vec2Normalize(vec2_t v)
{
    float length = vec2Length(v);
    vec2_t result = { v.x / length , v.y / length };
    return result;
}

/////////////////////////////////////////////////////////////////////////
// 3D ///////////////////////////////////////////////////////////////////

vec3_t vec3New(float x, float y, float z)
{
    vec3_t result = { x, y, z };
    return result;
}

vec3_t vec3Clone(vec3_t* v)
{
    vec3_t result = { v->x, v->y, v->z };
    return result;
}

float vec3Length(vec3_t v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3Add(vec3_t a, vec3_t b)
{
    vec3_t result = { a.x + b.x, a.y + b.y, a.z + b.z };
    return result;
}

vec3_t vec3Sub(vec3_t a, vec3_t b)
{
    vec3_t result = { a.x - b.x, a.y - b.y, a.z - b.z };
    return result;
}

vec3_t vec3Mul(vec3_t v, float factor)
{
    vec3_t result = { v.x * factor, v.y * factor, v.z * factor };
    return result;
}

vec3_t vec3Div(vec3_t v, float factor)
{
    vec3_t result = { v.x / factor, v.y / factor, v.z / factor };
    return result;
}

vec3_t vec3Cross(vec3_t a, vec3_t b)
{
    vec3_t result =
    {   .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x
    };

    return result;
}

float vec3Dot(vec3_t a, vec3_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3_t vec3Normalize(vec3_t v)
{
    float length = vec3Length(v);
    vec3_t result = { v.x / length , v.y / length, v.z / length };
    return result;
}

vec3_t vec3RotateX(vec3_t v, float angle)
{
    vec3_t rotatedVector = 
    {
        .x = v.x,
        .y = v.y * cos(angle) - v.z * sin(angle),
        .z = v.y * sin(angle) + v.z * cos(angle)
    };

    return rotatedVector;
}
vec3_t vec3RotateY(vec3_t v, float angle)
{
    vec3_t rotatedVector = 
    {
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z * cos(angle)
    };

    return rotatedVector;
}

vec3_t vec3RotateZ(vec3_t v, float angle)
{
    vec3_t rotatedVector = 
    {
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle),
        .z = v.z
    };

    return rotatedVector;
}

vec4_t vec4FromVec3(vec3_t v)
{
    vec4_t result = { v.x, v.y, v.z, 1.0f };
    return result;
}

vec3_t vec3FromVec4(vec4_t v)
{
    vec3_t result = { v.x, v.y, v.z };
    return result;
}

vec2_t vec2FromVec4(vec4_t v)
{
    vec2_t result = { v.x, v.y };
    return result;
}

