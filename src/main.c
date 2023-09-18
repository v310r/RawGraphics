#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "SDL.h"
#include "display.h"
#include "vector.h"
#include "triangle.h"
#include "mesh.h"
#include "array.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "upng.h"


bool g_bGameRunning = true;

float g_PreviousFrameTime = 0; // ms

vec3_t g_CameraPosition = { .x = 0.0f, .y = 0.0f, .z = 0.0f };

triangle_t* g_TrianglesToRender = NULL;

mat4_t g_PerspectiveProjectionMatrix;

void Setup(void)
{
    g_RenderMethod = RENDER_FILL_TRIANGLE;
    g_CullMethod = CULL_BACKFACE;

    g_ColorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * g_WindowWidth * g_WindowHeight);
    if (g_ColorBuffer == NULL)
    {
        SDL_Log("color buffer is NULL");
    }

    g_ColorBufferTexture = SDL_CreateTexture(g_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, g_WindowWidth, g_WindowHeight);
    if (g_ColorBufferTexture == NULL)
    {
        SDL_Log("Error SDL_CreateTexture() failed");
    }

    const float fov = M_PI / 3.0f; // 60 degrees = 60 * PI / 180
    const float aspectRation = (float)g_WindowHeight / (float)g_WindowWidth;
    const float znear = 0.1f;
    const float zfar = 100.0f;
    g_PerspectiveProjectionMatrix = mat4MakePerspective(fov, aspectRation, znear, zfar);

    //LoadCubeMeshData();
    LoadOBJMeshData("assets/f22.obj");

    LoadPngTextureData("assets/f22.png");
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

            if (ev.key.keysym.sym == SDLK_1)
            {
                g_RenderMethod = RENDER_WIRE_VERTEX;
            }

            if (ev.key.keysym.sym == SDLK_2)
            {
                g_RenderMethod = RENDER_WIRE;
            }

            if (ev.key.keysym.sym == SDLK_3)
            {
                g_RenderMethod = RENDER_FILL_TRIANGLE;
            }

            if (ev.key.keysym.sym == SDLK_4)
            {
                g_RenderMethod = RENDER_FILL_TRIANGLE_WIRE;
            }

            if (ev.key.keysym.sym == SDLK_5)
            {
                g_RenderMethod = RENDER_TEXTURED;
            }

            if (ev.key.keysym.sym == SDLK_6)
            {
                g_RenderMethod = RENDER_TEXTURED_WIRE;
            }

            if (ev.key.keysym.sym == SDLK_c)
            {
                g_CullMethod = CULL_BACKFACE;
            }

            if (ev.key.keysym.sym == SDLK_d)
            {
                g_CullMethod = CULL_NONE;
            }

            break;
        }
    }
}

void Render(void)
{
    SDL_UpdateTexture(g_ColorBufferTexture, NULL, g_ColorBuffer, (int)(sizeof(uint32_t) * g_WindowWidth));
    SDL_RenderCopy(g_Renderer, g_ColorBufferTexture, NULL, NULL);

    ClearColorBuffer(0xFF808080);

    for (int i = 0; i < array_length(g_TrianglesToRender); ++i)
    {
        triangle_t triangle = g_TrianglesToRender[i];

        if (g_RenderMethod == RENDER_FILL_TRIANGLE || g_RenderMethod == RENDER_FILL_TRIANGLE_WIRE)
        {
            DrawFilledTriangle(
                triangle.points[0].x, triangle.points[0].y, // vertex A
                triangle.points[1].x, triangle.points[1].y, // vertex B
                triangle.points[2].x, triangle.points[2].y, // vertex C
                triangle.color);
        }

        if (g_RenderMethod == RENDER_TEXTURED || g_RenderMethod == RENDER_TEXTURED_WIRE)
        {
            DrawTexturedTriangle(
                triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v,
                triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v,
                triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v,
                g_MeshTexture);
        }

        if (g_RenderMethod == RENDER_WIRE || g_RenderMethod == RENDER_WIRE_VERTEX || g_RenderMethod == RENDER_FILL_TRIANGLE_WIRE || g_RenderMethod == RENDER_TEXTURED_WIRE)
        {
            DrawTriangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                0xFF0000FF);
        }

        if (g_RenderMethod == RENDER_WIRE_VERTEX)
        {
            DrawRectangle(triangle.points[0].x, triangle.points[0].y, 6, 6, 0xFFFF0000);
            DrawRectangle(triangle.points[1].x, triangle.points[1].y, 6, 6, 0xFFFF0000);
            DrawRectangle(triangle.points[2].x, triangle.points[2].y, 6, 6, 0xFFFF0000);
        }
    }


    SDL_RenderPresent(g_Renderer);
}

vec2_t ProjectOrthographic(vec3_t point)
{
    // deprecated, but left as an example
    static float g_FOV_factor = 640;
    vec2_t projectedPoint = { .x = point.x * g_FOV_factor, .y = point.y * g_FOV_factor};
    return projectedPoint;
}

vec2_t ProjectPerspective(vec3_t point)
{
    // deprecated, but left as an example
    static float g_FOV_factor = 640;
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

    g_Mesh.Rotation.x += 0.05f;
    //g_Mesh.Rotation.y += 0.1f;
    //g_Mesh.Rotation.z += 0.1f;

    // temporary camera
    g_Mesh.Translation.z = 5.0f;

    mat4_t scaleMatrix = mat4MakeScale(g_Mesh.Scale.x, g_Mesh.Scale.y, g_Mesh.Scale.z);
    mat4_t translationMatrix = mat4MakeTranslation(g_Mesh.Translation.x, g_Mesh.Translation.y, g_Mesh.Translation.z);
    mat4_t rotationMatrix_X = mat4MakeRotationX(g_Mesh.Rotation.x);
    mat4_t rotationMatrix_Y = mat4MakeRotationY(g_Mesh.Rotation.y);
    mat4_t rotationMatrix_Z = mat4MakeRotationZ(g_Mesh.Rotation.z);


    int numberOfFaces = array_length(g_Mesh.Faces);
    for (int i = 0; i < numberOfFaces; ++i)
    {
        face_t meshFace = g_Mesh.Faces[i];

        vec3_t faceVertices[3];
        faceVertices[0] = g_Mesh.Vertices[meshFace.a];
        faceVertices[1] = g_Mesh.Vertices[meshFace.b];
        faceVertices[2] = g_Mesh.Vertices[meshFace.c];


        vec4_t transformedVertices[3];
        // for each vertex in current face_t we will apply transformations
        for (int j = 0; j < 3; ++j)
        {
            vec4_t transformedVertex = vec4FromVec3(faceVertices[j]);

            mat4_t worldMatrix = mat4Identity();
            worldMatrix = mat4Mulmat4(scaleMatrix, worldMatrix);
            worldMatrix = mat4Mulmat4(rotationMatrix_X, worldMatrix);
            worldMatrix = mat4Mulmat4(rotationMatrix_Y, worldMatrix);
            worldMatrix = mat4Mulmat4(rotationMatrix_Z, worldMatrix);
            worldMatrix = mat4Mulmat4(translationMatrix, worldMatrix);

            transformedVertex = mat4Mulvec4(worldMatrix, transformedVertex);


            //transformedVertex = vec3RotateX(transformedVertex, g_Mesh.Rotation.y);
            //transformedVertex = vec3RotateY(transformedVertex, g_Mesh.Rotation.y);
            //transformedVertex = vec3RotateZ(transformedVertex, g_Mesh.Rotation.y);

            transformedVertices[j] = transformedVertex;
        }

        vec3_t vectorA = vec3FromVec4(transformedVertices[0]);
        vec3_t vectorB = vec3FromVec4(transformedVertices[1]);
        vec3_t vectorC = vec3FromVec4(transformedVertices[2]);

        vec3_t vectorAB = vec3Normalize(vec3Sub(vectorB, vectorA));
        vec3_t vectorAC = vec3Normalize(vec3Sub(vectorC, vectorA));

        vec3_t normal = vec3Normalize(vec3Cross(vectorAB, vectorAC));

        vec3_t cameraRay = vec3Sub(g_CameraPosition, vectorA);

        float dotNormalCamera = vec3Dot(cameraRay, normal);

        if (g_CullMethod == CULL_BACKFACE && dotNormalCamera < 0)
        {
            // we will not render faces that we do not see
            continue;
        }

        //light calculation
        float dotNormalLight = vec3Dot(g_DirectionalLight.Direction, normal);
        float intensityPercentage = (dotNormalLight * 0.5f) + 0.5;
        meshFace.color = ApplyLightIntensity(meshFace.color, intensityPercentage);


        // perspective projection application and perspective divide
        vec4_t projectedPoints[3];
        for (int j = 0; j < 3; ++j)
        {
            projectedPoints[j] = mat4MulVec4PerspectiveProjection(g_PerspectiveProjectionMatrix, transformedVertices[j]);
            //vec2_t projectedPoint = ProjectOrthographic(transformedVertex);


            //scale into the view
            projectedPoints[j].x *= (g_WindowWidth / 2.0f);
            projectedPoints[j].y *= (g_WindowHeight / 2.0f);

            // Invert the x values to account for mirroring
            //projectedPoints[j].x = -projectedPoints[j].x;

            // Invert the y values to account for flipped screen y coordinate
            projectedPoints[j].y = -projectedPoints[j].y;


            // translate projected points to the middle of the screen
            projectedPoints[j].x += (g_WindowWidth / 2.0f);
            projectedPoints[j].y += (g_WindowHeight / 2.0f);


        }
        
        // Depth calculation

        float averageDepth = (transformedVertices[0].z + transformedVertices[1].z + transformedVertices[2].z) / 3.0f;

        triangle_t projectedTriangle =
        {
            .points =
            {
                projectedPoints[0], projectedPoints[1], projectedPoints[2]
            },

            .texcoords =
            {
                {meshFace.a_uv.u, meshFace.a_uv.v},
                {meshFace.b_uv.u, meshFace.b_uv.v},
                {meshFace.c_uv.u, meshFace.c_uv.v},
            },

            .color = meshFace.color,
            .averageDepth = averageDepth
        };

        array_push(g_TrianglesToRender, projectedTriangle);
    }

    // sorting triangles by depth
    uint32_t numOfTriangles = array_length(g_TrianglesToRender);
    for (int i = 0; i < numOfTriangles; ++i)
    {
        for (int j = i + 1; j < numOfTriangles; ++j)
        {
            if (g_TrianglesToRender[i].averageDepth < g_TrianglesToRender[j].averageDepth)
            {
                triangle_t temp = g_TrianglesToRender[i];
                g_TrianglesToRender[i] = g_TrianglesToRender[j];
                g_TrianglesToRender[j] = temp;
            }
        }
    }
}

void FreeResources(void)
{
    free(g_ColorBuffer);
    upng_free(g_PNG_texture);
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