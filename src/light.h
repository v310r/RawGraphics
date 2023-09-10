#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"
#include <stdint.h>

typedef struct  
{
    vec3_t Direction;
} light_t;


extern light_t g_DirectionalLight;

uint32_t ApplyLightIntensity(uint32_t originalColor, float percentageFactor);

#endif