#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include "texture.h"
#include "upng.h"

#include <stdint.h>


typedef struct  
{
    int a;
    int b;
    int c;
    tex2_t a_uv;
    tex2_t b_uv;
    tex2_t c_uv;

    uint32_t color;
} face_t; // stores vertex indexes

typedef struct 
{
    vec4_t points[3];
    tex2_t texcoords[3];
    uint32_t color;
    upng_t* texture;
} triangle_t; // actual projected points onto the 2D screen

void DrawTexel(
    int x, int y, upng_t* texture,
    vec4_t pointA, vec4_t pointB, vec4_t pointC
    , tex2_t a_uv, tex2_t b_uv, tex2_t c_uv);

void DrawTexturedTriangle(
    int x0, int y0, float z0, float w0, float u0, float v0, 
    int x1, int y1, float z1, float w1, float u1, float v1,
    int x2, int y2, float z2, float w2, float u2, float v2,
    upng_t* texture);

void DrawTrianglePixel(
    int x, int y,
    uint32_t color,
    vec4_t pointA, vec4_t pointB, vec4_t pointC);

void DrawFilledTriangle(
    int x0, int y0, float z0, float w0,
    int x1, int y1, float z1, float w1,
    int x2, int y2, float z2, float w2, 
    uint32_t color);

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

#endif