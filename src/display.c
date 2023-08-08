#include "display.h"
#include "SDL.h"


int g_WindowWidth;
int g_WindowHeight;

SDL_Window* g_Window = NULL;
SDL_Renderer* g_Renderer = NULL;

uint32_t* g_ColorBuffer = NULL;
SDL_Texture* g_ColorBufferTexture = NULL;


void DrawRectangle(int _x, int _y, int width, int height, uint32_t color)
{
    const int realWidth = width + _x;
    const int realHeight = height + _y;
    for (int y = _y; y < realHeight; ++y)
    {
        for (int x = _x; x < realWidth; ++x)
        {
            g_ColorBuffer[(y * g_WindowWidth) + x] = color;
        }
    }
}

void DrawGrid(uint32_t color, int strideX, int strideY)
{
    // vertical lines
    for (int x = strideX; x < g_WindowWidth; x += strideX)
    {
        for (int y = 0; y < g_WindowHeight; ++y)
        {
            g_ColorBuffer[(g_WindowWidth * y) + x] = color;
        }
    }

    //horizontal lines
    for (int y = strideY; y < g_WindowHeight; y += strideY)
    {
        for (int x = 0; x < g_WindowWidth; ++x)
        {
            g_ColorBuffer[(g_WindowWidth * y) + x] = color;
        }
    }
}

void ClearColorBuffer(uint32_t color)
{
    for (int y = 0; y < g_WindowHeight; ++y)
    {
        for (int x = 0; x < g_WindowWidth; ++x)
        {
            g_ColorBuffer[(g_WindowWidth * y) + x] = color;
        }

    }
}

int InitGraphics(void)
{
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    g_WindowWidth = displayMode.w;
    g_WindowHeight = displayMode.h;

    g_Window = SDL_CreateWindow("RawGraphics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_WindowWidth, g_WindowHeight, SDL_WINDOW_BORDERLESS);
    if (!g_Window)
    {
        SDL_Log("Error SDL_CreateWindow() failed");
        return 2;
    }

    g_Renderer = SDL_CreateRenderer(g_Window, -1, 0);
    if (!g_Renderer)
    {
        SDL_Log("Error SDL_CreateRenderer() failed");
        return 3;
    }

    return 0;
}

void DestroyGraphics(void)
{
    free(g_ColorBuffer);

    SDL_DestroyRenderer(g_Renderer);
    SDL_DestroyWindow(g_Window);
    SDL_Quit();
}
