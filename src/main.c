#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "SDL.h"
#include "display.h"

bool g_bGameRunning = true;


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
    DrawGrid(0xFF00FF00, 10, 10);
    DrawRectangle(600, 800, 1200, 200, 0xFFFFC0CB);
    SDL_RenderPresent(g_Renderer);
}

void Update(void)
{

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