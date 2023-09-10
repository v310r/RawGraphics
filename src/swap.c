#include "swap.h"


void swapInt(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swapFloat(float* a, float* b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}