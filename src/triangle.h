#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include <stdint.h>

typedef struct  
{
    int a;
    int b;
    int c;

    uint32_t color;
} face_t; // stores vertex indexes

typedef struct 
{
    vec2_t points[3];
    uint32_t color;
} triangle_t; // actual projected points onto the 2D screen

void DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

#endif