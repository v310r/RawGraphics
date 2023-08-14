#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "SDL.h"
#include "display.h"
#include "vector.h"
#include "triangle.h"
#include "mesh.h"
#include "array.h"


bool g_bGameRunning = true;

float g_PreviousFrameTime = 0; // ms

vec3_t g_CameraPosition = { .x = 0.0f, .y = 0.0f, .z = 0.0f };

triangle_t* g_TrianglesToRender = NULL;

float g_FOV_factor = 640;


void Setup(void)
{
    g_ColorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * g_WindowWidth * g_WindowHeight);
    if (g_ColorBuffer == NULL)
    {
        SDL_Log("color buffer is NULL");
    }

    g_ColorBufferTexture = SDL_CreateTexture(g_Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, g_WindowWidth, g_WindowHeight);
    if (g_ColorBufferTexture == NULL)
    {
        SDL_Log("Error SDL_CreateTexture() failed");
    }

    //LoadCubeMeshData();
    LoadOBJMeshData("assets/f22.obj");
}

void ProcessInput(void)
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
        case SDL_QUIT:
            g_bGameRunning = false;
            break;
        case SDL_KEYDOWN:
            if (ev.key.keysym.sym == SDLK_ESCAPE)
            {
                g_bGameRunning = false;
            }

            break;
        }
    }
}

void Render(void)
{
    SDL_UpdateTexture(g_ColorBufferTexture, NULL, g_ColorBuffer, (int)(sizeof(uint32_t) * g_WindowWidth));
    SDL_RenderCopy(g_Renderer, g_ColorBufferTexture, NULL, NULL);

    ClearColorBuffer(0xFF000000);

    for (int i = 0; i < array_length(g_TrianglesToRender); ++i)
    {
        triangle_t triangle = g_TrianglesToRender[i];

        DrawRectangle(triangle.points[0].x, triangle.points[0].y, 6, 6, 0xFFFFFF00);
        DrawRectangle(triangle.points[1].x, triangle.points[1].y, 6, 6, 0xFFFFFF00);
        DrawRectangle(triangle.points[2].x, triangle.points[2].y, 6, 6, 0xFFFFFF00);

        DrawTriangle(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y,
            0xFF00FF00);
    }

    SDL_RenderPresent(g_Renderer);
}

vec2_t ProjectOrthographic(vec3_t point)
{
    vec2_t projectedPoint = { .x = point.x * g_FOV_factor, .y = point.y * g_FOV_factor};
    return projectedPoint;
}

vec2_t ProjectPerspective(vec3_t point)
{
    vec2_t projectedPoint = 
    { 
        .x = (point.x * g_FOV_factor) / point.z,  
        .y = (point.y * g_FOV_factor) / point.z 
    };
    return projectedPoint;
}

void Update(void)
{
    float currentFrameTime = SDL_GetTicks();
    float delayTime = FRAME_TARGET_TIME - (currentFrameTime - g_PreviousFrameTime);
    if (delayTime > 0.0f)
    {
        SDL_Delay(delayTime);
    }

    g_PreviousFrameTime = currentFrameTime;

    array_free(g_TrianglesToRender);
    g_TrianglesToRender = NULL;

    g_Mesh.Rotation.x += 0.01f;
    g_Mesh.Rotation.y += 0.01f;
    g_Mesh.Rotation.z += 0.01f;

    int numberOfFaces = array_length(g_Mesh.Faces);
    for (int i = 0; i < numberOfFaces; ++i)
    {
        face_t meshFace = g_Mesh.Faces[i];

        vec3_t faceVertices[3];
        faceVertices[0] = g_Mesh.Vertices[meshFace.a - 1];
        faceVertices[1] = g_Mesh.Vertices[meshFace.b - 1];
        faceVertices[2] = g_Mesh.Vertices[meshFace.c - 1];


        triangle_t projectedTriangle;
        vec3_t transformedVertices[3];
        // for each vertex in current face_t we will apply transformations
        for (int j = 0; j < 3; ++j)
        {
            vec3_t transformedVertex = faceVertices[j];

            transformedVertex = vec3RotateX(transformedVertex, g_Mesh.Rotation.y);
            transformedVertex = vec3RotateY(transformedVertex, g_Mesh.Rotation.y);
            transformedVertex = vec3RotateZ(transformedVertex, g_Mesh.Rotation.y);

            transformedVertex.z += 5;

            transformedVertices[j] = transformedVertex;
        }

        // backface culling check
        vec3_t vectorA = transformedVertices[0];
        vec3_t vectorB = transformedVertices[1];
        vec3_t vectorC = transformedVertices[2];

        vec3_t vectorAB = vec3Normalize(vec3Sub(vectorB, vectorA));
        vec3_t vectorAC = vec3Normalize(vec3Sub(vectorC, vectorA));

        vec3_t normal = vec3Normalize(vec3Cross(vectorAB, vectorAC)); 

        vec3_t cameraRay = vec3Sub(g_CameraPosition, vectorA);

        float dotNormalCamera = vec3Dot(cameraRay, normal);

        if (dotNormalCamera < 0)
        {
            // we will not render faces that we do not see
            continue;
        }

        for (int j = 0; j < 3; ++j)
        {
            vec2_t projectedPoint = ProjectPerspective(transformedVertices[j]);
            //vec2_t projectedPoint = ProjectOrthographic(transformedVertex);


            projectedPoint.x += (g_WindowWidth / 2.0f);
            projectedPoint.y += (g_WindowHeight / 2.0f);


            projectedTriangle.points[j] = projectedPoint;
        }

        array_push(g_TrianglesToRender, projectedTriangle);
    }
}

void FreeResources(void)
{
    free(g_ColorBuffer);
    array_free(g_Mesh.Faces);
    array_free(g_Mesh.Vertices);
}

#undef main
int main(int argc, char* args[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Error SDL_Init() failed");
        return 1;
    }

    if (InitSDLGraphics() != 0)
    {
        SDL_Log("Error InitSDLGraphics() failed");
        return 2;
    }

    //SDL_SetWindowFullscreen(g_Window, SDL_WINDOW_FULLSCREEN);

    Setup();


    while (g_bGameRunning)
    {
        ProcessInput();
        Update();
        Render();
    }


    FreeResources();
    DestroySDLGraphics();
    SDL_Quit();
    
    return 0;
}