#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include "texture.h"
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
    float averageDepth;
} triangle_t; // actual projected points onto the 2D screen

void DrawTexturedTriangle(
    int x0, int y0, float u0, float v0, 
    int x1, int y1, float u1, float v1, 
    int x2, int y2, float u2, float v2, 
    uint32_t* texture);

void DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

#endif