#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define NUM_CUBE_VERTICES 8
#define NUM_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face


extern vec3_t g_CubeVertices[NUM_CUBE_VERTICES];

extern face_t g_CubeFaces[NUM_CUBE_FACES];

typedef struct 
{
    vec3_t* Vertices;
    face_t* Faces;

    vec3_t Scale;
    vec3_t Rotation;
    vec3_t Translation;
} mesh_t;

extern mesh_t g_Mesh;

void LoadCubeMeshData(void);

void LoadOBJMeshData(const char* filename);

#endif