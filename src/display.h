#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define FPS 180.0f
#define FRAME_TARGET_TIME (1000.0f / FPS)

extern int g_WindowWidth;
extern int g_WindowHeight;

extern struct SDL_Window* g_Window;
extern struct SDL_Renderer* g_Renderer;

extern uint32_t* g_ColorBuffer;
extern struct SDL_Texture* g_ColorBufferTexture;


void DrawGrid(uint32_t color, int strideX, int strideY);

void DrawRectangle(int _x, int _y, int width, int height, uint32_t color);

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);

void DrawPixel(int x, int y, uint32_t color);

void ClearColorBuffer(uint32_t color);

int InitGraphics(void);

void DestroyGraphics(void);

#endif