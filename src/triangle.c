#include "triangle.h"
#include "display.h"


static void swapInt(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void FillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float InvertedSlope1 = (x0 - x1) / (float)(y0 - y1);
    float InvertedSlope2 = (x0 - x2) / (float)(y0 - y2);

    float x_start = x0;
    float x_end = x0;

    for (int y = y0; y <= y2; ++y)
    {
        DrawLine(x_start, y, x_end, y, color);
        x_start += InvertedSlope1;
        x_end += InvertedSlope2;
    }
}

static void FillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float InvertedSlope1 = (x2 - x0) / (float)(y2 - y0);
    float InvertedSlope2 = (x2 - x1) / (float)(y2 - y1);

    float x_start = x2;
    float x_end = x2;

    for (int y = y2; y >= y0; --y)
    {
        DrawLine(x_start, y, x_end, y, color);
        x_start -= InvertedSlope1;
        x_end -= InvertedSlope2;
    }
}

void DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // y0 < y1 < y2 (sorting)

    if (y0 > y1)
    {
        swapInt(&y0, &y1);
        swapInt(&x0, &x1);
    }

    if (y1 > y2)
    {
        swapInt(&y1, &y2);
        swapInt(&x1, &x2);
    }

    if (y0 > y1)
    {
        swapInt(&y0, &y1);
        swapInt(&x0, &x1);
    }

    // finding Mx,My

    if (y1 == y2)
    {
        FillFlatBottomTriangle(x0, y0, x1, y1, x2, y2, color);
        return;
    }
    else if (y0 == y1)
    {
        FillFlatTopTriangle(x0, y0, x1, y1, x2, y2, color);
        return;
    }

    int My = y1;
    int Mx = (((float)(x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

    FillFlatBottomTriangle(x0, y0, x1, y1, Mx, My, color);

    FillFlatTopTriangle(x1, y1, Mx, My, x2, y2, color);

}

