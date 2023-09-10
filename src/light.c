#include "light.h"

light_t g_DirectionalLight = { .Direction = {.x = 1.0f, .y = 0.0f, .z = 0.0f} };



uint32_t ApplyLightIntensity(uint32_t originalColor, float percentageFactor)
{
    if (percentageFactor < 0.0f)
    {
        percentageFactor = 0.0f;
    }
    else if (percentageFactor > 1.0f)
    {
        percentageFactor = 1.0f;
    }

    uint32_t a = (originalColor & 0xFF000000);
    uint32_t r = (originalColor & 0x00FF0000) * percentageFactor;
    uint32_t g = (originalColor & 0x0000FF00) * percentageFactor;
    uint32_t b = (originalColor & 0x000000FF) * percentageFactor;

    uint32_t newColor = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

    return newColor;
}