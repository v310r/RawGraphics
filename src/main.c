#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "SDL.h"
#include "display.h"
#include "vector.h"


#define N_POINTS (9 * 9 * 9)


bool g_bGameRunning = true;

vec3 g_CubePoints[N_POINTS];
vec2 g_ProjectedPoints[N_POINTS];

vec3 g_CameraPosition = { .x = 0.0f, .y = 0.0f, .z = -5.0f };

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

    int pointCount = 0;
    for (float x = -1; x <= 1; x += 0.25)
    {
        for (float y = -1; y <= 1; y += 0.25)
        {
            for (float z = -1; z <= 1; z += 0.25)
            {
                vec3 newPoint = { x, y, z };
                g_CubePoints[pointCount++] = newPoint;
            }
        }
    }
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

    for (int i = 0; i < N_POINTS; ++i)
    {
        vec2 projectedPoint = g_ProjectedPoints[i];
        DrawRectangle(projectedPoint.x + g_WindowWidth / 2, projectedPoint.y + g_WindowHeight / 2, 4, 4, 0xFFFFFF00);
    }


    //vec2 projectedPoint = g_ProjectedPoints[0];
    //DrawRectangle(projectedPoint.x, projectedPoint.y, 4, 4, 0xFFFFFF00);

    SDL_RenderPresent(g_Renderer);
}

vec2 ProjectOrthographic(vec3 point)
{
    vec2 projectedPoint = { .x = point.x * g_FOV_factor, .y = point.y * g_FOV_factor};
    return projectedPoint;
}

vec2 ProjectPerspective(vec3 point)
{
    vec2 projectedPoint = 
    { 
        .x = (point.x * g_FOV_factor) / point.z, 
        .y = (point.y * g_FOV_factor) / point.z 
    };
    return projectedPoint;
}

void Update(void)
{
    for (int i = 0; i < N_POINTS; ++i)
    {
        vec3 point = g_CubePoints[i];
        point.z -= g_CameraPosition.z;

        vec2 projectedPoint = ProjectPerspective(point);
        //vec2 projectedPoint = ProjectOrthographic(point);

        g_ProjectedPoints[i] = projectedPoint;
    }
}

#undef main
int main(int argc, char* args[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Error SDL_Init() failed");
        return 1;
    }

    if (InitGraphics() != 0)
    {
        SDL_Log("Error SDL_Init() failed");
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

    DestroyGraphics();
    
    return 0;
}