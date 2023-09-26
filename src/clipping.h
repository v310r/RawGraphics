#ifndef CLIPPING_H
#define CLIPPING_H

#include "vector.h"
#include "triangle.h"


#define MAX_NUM_POLY_VERTICES 10
#define MAX_NUM_POLY_TRIANGLES 10

enum 
{
    LEFT_FRUSTUM_PLANE,
    RIGHT_FRUSTUM_PLANE,
    TOP_FRUSTUM_PLANE,
    BOTTOM_FRUSTUM_PLANE,
    NEAR_FRUSTUM_PLANE,
    FAR_FRUSTUM_PLANE
};

typedef struct  
{
    vec3_t point;
    vec3_t normal;
} plane_t;

typedef struct
{
    vec3_t vertices[MAX_NUM_POLY_VERTICES];
    tex2_t texcoords[MAX_NUM_POLY_VERTICES];
    int numVertices;
} polygon_t;

void InitFrustumPlanes(float fovX, float fovY, float zNear, float zFar);

polygon_t CreatePolygonFromTriangle(vec3_t v0, vec3_t v1, vec3_t v2, tex2_t t0, tex2_t t1, tex2_t t2);

void clipPolygon(polygon_t* polygon);

void GetTrianglesFromPolygon(polygon_t* polygon, triangle_t* triangles, int* numTriangles);

#endif // CLIPPING_H