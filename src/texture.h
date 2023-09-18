#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include "upng.h"


typedef struct  
{
    float u;
    float v;
} tex2_t;

extern int g_TextureWidth;
extern int g_TextureHeight;

extern const uint8_t g_REDBRICK_TEXTURE[];

extern uint32_t* g_MeshTexture;

extern upng_t* g_PNG_texture;

void LoadPngTextureData(char* filename);

#endif