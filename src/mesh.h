#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "upng.h"


typedef struct 
{
    vec3_t* Vertices;
    face_t* Faces;

    upng_t* Texture;

    vec3_t Scale;
    vec3_t Rotation;
    vec3_t Translation;
} mesh_t;

void LoadMesh(char* objFileName, char* pngFileName, vec3_t scale, vec3_t translation, vec3_t rotation);
mesh_t* GetMesh(int index);
int GetNumMeshes();
void LoadOBJMeshData(mesh_t* mesh, const char* filename);
void LoadMeshPngData(mesh_t* mesh, char* filename);

void FreeMeshes();

#endif