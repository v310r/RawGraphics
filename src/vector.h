#ifndef VECTOR_H
#define VECTOR_H

typedef struct  
{
    float x;
    float y;
} vec2_t;

typedef struct
{
    float x;
    float y;
    float z;
} vec3_t;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vec4_t;
////////////////////////////////////
// Vector 2D functions
////////////////////////////////////

vec2_t vec2New(float x, float y);

float vec2Length(vec2_t v);

vec2_t vec2Add(vec2_t a, vec2_t b);
vec2_t vec2Sub(vec2_t a, vec2_t b);
vec2_t vec2Mul(vec2_t v, float factor);
vec2_t vec2Div(vec2_t v, float factor);

float vec2Dot(vec2_t a, vec2_t b);

vec2_t vec2Normalize(vec2_t v);

////////////////////////////////////
// Vector 3D functions
////////////////////////////////////

vec3_t vec3New(float x, float y, float z);

vec3_t vec3Clone(vec3_t* v);

float vec3Length(vec3_t v);

vec3_t vec3Add(vec3_t a, vec3_t b);
vec3_t vec3Sub(vec3_t a, vec3_t b);
vec3_t vec3Mul(vec3_t v, float factor);
vec3_t vec3Div(vec3_t v, float factor);

vec3_t vec3Cross(vec3_t a, vec3_t b);
float vec3Dot(vec3_t a, vec3_t b);

vec3_t vec3Normalize(vec3_t v);

vec3_t vec3RotateX(vec3_t v, float angle);
vec3_t vec3RotateY(vec3_t v, float angle);
vec3_t vec3RotateZ(vec3_t v, float angle);

////////////////////////////////////
// Vector 4D functions
////////////////////////////////////

vec4_t vec4FromVec3(vec3_t v);

vec3_t vec3FromVec4(vec4_t v);

vec2_t vec2FromVec4(vec4_t v);

#endif