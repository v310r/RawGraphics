#ifndef TEXTURE_H
#define TEXTURE_H

#include "upng.h"


typedef struct  
{
    float u;
    float v;
} tex2_t;

tex2_t tex2Clone(tex2_t* t);

#endif