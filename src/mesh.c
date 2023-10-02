#include "mesh.h"
#include "array.h"
#include <stdio.h>
#include <string.h>


#define MAX_NUM_MESHES 10
static mesh_t s_Meshes[MAX_NUM_MESHES];
static int s_MeshCount = 0;

void LoadMesh(char* objFileName, char* pngFileName, vec3_t scale, vec3_t translation, vec3_t rotation)
{
    LoadOBJMeshData(&s_Meshes[s_MeshCount], objFileName);
    LoadMeshPngData(&s_Meshes[s_MeshCount], pngFileName);
    s_Meshes[s_MeshCount].Scale = scale;
    s_Meshes[s_MeshCount].Translation = translation;
    s_Meshes[s_MeshCount].Rotation = rotation;

    ++s_MeshCount;
}

mesh_t* GetMesh(int index)
{
    return &s_Meshes[index];
}

int GetNumMeshes()
{
    return s_MeshCount;
}

void LoadOBJMeshData(mesh_t* mesh, const char* filename)
{
    FILE* file;
    file = fopen(filename, "r");

    char line[1024];

    tex2_t* texcoords = NULL;

    while (fgets(line, 1024, file))
    {
        if (strncmp(line, "v ", 2) == 0)
        {
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(mesh->Vertices, vertex);
        }
        else if (strncmp(line, "vt ", 3) == 0)
        {
            tex2_t texcoord;
            sscanf(line, "vt %f %f", &texcoord.u, &texcoord.v);
            array_push(texcoords, texcoord);
        }
        else if (strncmp(line, "f ", 2) == 0)
        {
            int vertex_indices[3];
            int texture_indices[3];
            int normalIndices[3];
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                &vertex_indices[0], &texture_indices[0], &normalIndices[0],
                &vertex_indices[1], &texture_indices[1], &normalIndices[1],
                &vertex_indices[2], &texture_indices[2], &normalIndices[2]);


            face_t face =
            {
                .a = vertex_indices[0] - 1,
                .b = vertex_indices[1] - 1,
                .c = vertex_indices[2] - 1,
                .a_uv = texcoords[texture_indices[0] - 1],
                .b_uv = texcoords[texture_indices[1] - 1],
                .c_uv = texcoords[texture_indices[2] - 1],
                .color = 0xFFFFFFFF
            };

            array_push(mesh->Faces, face);
        }

    }

    array_free(texcoords);
}

void LoadMeshPngData(mesh_t* mesh, char* filename)
{
    upng_t* pngImage = upng_new_from_file(filename);
    if (pngImage)
    {
        upng_decode(pngImage);
        if (upng_get_error(pngImage) == UPNG_EOK)
        {
            mesh->Texture = pngImage;
        }
    }
}

void FreeMeshes()
{
    for (int i = 0; i < s_MeshCount; ++i)
    {
        upng_free(s_Meshes[i].Texture);
        array_free(s_Meshes[i].Faces);
        array_free(s_Meshes[i].Vertices);
    }
}
