#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

extern int g_WindowWidth;
extern int g_WindowHeight;

extern struct SDL_Window* g_Window;
extern struct SDL_Renderer* g_Renderer;

extern uint32_t* g_ColorBuffer;
extern struct SDL_Texture* g_ColorBufferTexture;


void DrawRectangle(int _x, int _y, int width, int height, uint32_t color);

void DrawGrid(uint32_t color, int strideX, int strideY);

void ClearColorBuffer(uint32_t color);

int InitGraphics(void);

void DestroyGraphics(void);

#endif