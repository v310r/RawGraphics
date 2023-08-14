#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

typedef struct  
{
    int a;
    int b;
    int c;
} face_t; // stores vertex indexes

typedef struct 
{
    vec2_t points[3];
} triangle_t; // actual projected points onto the 2D screen

#endif