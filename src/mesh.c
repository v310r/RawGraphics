#include "mesh.h"
#include "array.h"
#include <stdio.h>
#include <string.h>

mesh_t g_Mesh =
{
    .Vertices = NULL,
    .Faces = NULL,
    .Rotation = {.x = 0.0f, .y = 0.0f, .z = 0.0f}
};

vec3_t g_CubeVertices[NUM_CUBE_VERTICES] =
{
    {.x = -1.0f, .y = -1.0f, .z = -1.0f},   // 1
    {.x = -1.0f, .y = 1.0f, .z = -1.0f},    // 2
    {.x = 1.0f, .y = 1.0f, .z = -1.0f},     // 3
    {.x = 1.0f, .y = -1.0f, .z = -1.0f},    // 4
    {.x = 1.0f, .y = 1.0f, .z = 1.0f},      // 5
    {.x = 1.0f, .y = -1.0f, .z = 1.0f},     // 6
    {.x = -1.0f, .y = 1.0f, .z = 1.0f},     // 7
    {.x = -1.0f, .y = -1.0f, .z = 1.0f},    // 8
};

face_t g_CubeFaces[NUM_CUBE_FACES] =
{
    //front
    {.a = 1, .b = 2, .c = 3},
    {.a = 1, .b = 3, .c = 4},

    //right
    {.a = 4, .b = 3, .c = 5},
    {.a = 4, .b = 5, .c = 6},

    //back
    {.a = 6, .b = 5, .c = 7},
    {.a = 6, .b = 7, .c = 8},

    //left
    {.a = 8, .b = 7, .c = 2},
    {.a = 8, .b = 2, .c = 1},

    //top
    {.a = 2, .b = 7, .c = 5},
    {.a = 2, .b = 5, .c = 3},

    //bottom
    {.a = 6, .b = 8, .c = 1},
    {.a = 6, .b = 1, .c = 4},
};

void LoadCubeMeshData(void)
{
    for (int i = 0; i < NUM_CUBE_VERTICES; ++i)
    {
        vec3_t cubeVertex = g_CubeVertices[i];
        array_push(g_Mesh.Vertices, cubeVertex);
    }

    for (int i = 0; i < NUM_CUBE_FACES; ++i)
    {
        face_t cubeFace = g_CubeFaces[i];
        array_push(g_Mesh.Faces, cubeFace);
    }
}

void LoadOBJMeshData(const char* filename)
{
    FILE* file;
    file = fopen(filename, "r");

    char line[1024];

    while (fgets(line, 1024, file))
    {
        if (strncmp(line, "v ", 2) == 0)
        {
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(g_Mesh.Vertices, vertex);
        }
        else if (strncmp(line, "f ", 2) == 0)
        {
            face_t face;
            int texture_indices[3];
            int normalIndices[3];
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                &face.a, &texture_indices[0], &normalIndices[0], 
                &face.b, &texture_indices[1], &normalIndices[1], 
                &face.c, &texture_indices[2], &normalIndices[2]);

            array_push(g_Mesh.Faces, face);
        }

    }
}