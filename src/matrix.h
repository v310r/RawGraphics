#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"


typedef struct  
{
    float m[4][4];
} mat4_t;

mat4_t mat4Identity(void);

mat4_t mat4MakeScale(float sx, float sy, float sz);

mat4_t mat4MakeRotationX(float angle);
mat4_t mat4MakeRotationY(float angle);
mat4_t mat4MakeRotationZ(float angle);


mat4_t mat4MakeTranslation(float tx, float ty, float tz);

vec4_t mat4Mulvec4(mat4_t m, vec4_t v);

mat4_t mat4Mulmat4(mat4_t a, mat4_t b);

mat4_t mat4MakePerspective(float fov, float aspect, float znear, float zfar);
vec4_t mat4MulVec4PerspectiveProjection(mat4_t projectionMatrix, vec4_t v);

#endif