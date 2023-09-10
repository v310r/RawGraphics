#include "triangle.h"
#include "display.h"
#include "swap.h"


static void FillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float InvertedSlope1 = 0.0f;
    float InvertedSlope2 = 0.0f;
    
    if (y0 - y1 != 0.0f)
    {
        InvertedSlope1 = (float)(x0 - x1) / (y0 - y1);
    }

    if (y0 - y2 != 0.0f)
    {
        InvertedSlope2 = (float)(x0 - x2) / (y0 - y2);
    }

    float xStart = x0;
    float xEnd = x0;

    for (int y = y0; y <= y2; ++y)
    {
        DrawLine(xStart, y, xEnd, y, color);
        xStart += InvertedSlope1;
        xEnd += InvertedSlope2;
    }
}

static void FillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float InvertedSlope1 = 0.0f;
    float InvertedSlope2 = 0.0f;

    if (y2 - y0 != 0.0f)
    {
        InvertedSlope1 = (float)(x2 - x0) / (y2 - y0);
    }

    if (y2 - y1 != 0.0f)
    {
        InvertedSlope2 = (float)(x2 - x1) / (y2 - y1);
    }

    float xStart = x2;
    float xEnd = x2;

    for (int y = y2; y >= y0; --y)
    {
        DrawLine(xStart, y, xEnd, y, color);
        xStart -= InvertedSlope1;
        xEnd -= InvertedSlope2;
    }
}

void DrawTexturedTriangle(
    int x0, int y0, float u0, float v0,
    int x1, int y1, float u1, float v1,
    int x2, int y2, float u2, float v2,
    uint32_t* texture)
{
    // y0 < y1 < y2 (sorting)
    if (y0 > y1)
    {
        swapInt(&y0, &y1);
        swapInt(&x0, &x1);
        swapFloat(&u0, &u1);
        swapFloat(&v0, &v1);
    }

    if (y1 > y2)
    {
        swapInt(&y1, &y2);
        swapInt(&x1, &x2);
        swapFloat(&u1, &u2);
        swapFloat(&v1, &v2);
    }

    if (y0 > y1)
    {
        swapInt(&y0, &y1);
        swapInt(&x0, &x1);
        swapFloat(&u0, &u1);
        swapFloat(&v0, &v1);
    }

    // flat-bottom filling (basically top filling)
    // 
    // how much x changes when y is constantly increasing 1 by one (scanline)
    float inverseSlope1 = 0.0f;
    float inverseSlope2 = 0.0f;

    if (y1 - y0 != 0.0f)
    {
        inverseSlope1 = (float)(x1 - x0) / abs(y1 - y0);
    }

    if (y2 - y0 != 0.0f)
    {
        inverseSlope2 = (float)(x2 - x0) / abs(y2 - y0);
    }

    if (y1 - y0 == 0.0f)
    {
        return;
    }

    for (int y = y0; y <= y1; ++y)
    {
        // just another formula
        int xStart = x1 + (y - y1) * inverseSlope1;
        int xEnd = x0 + (y - y0) * inverseSlope2;

        if (xEnd < xStart)
        {
            swapInt(&xStart, &xEnd);
        }

        for (int x = xStart; x < xEnd; ++x)
        {
            DrawPixel(x, y, 0xFFFF00FF);
        }
    }

    // flat-top filling (basically bottom filling)
    // 
    // how much x changes when y is constantly increasing 1 by one (scanline)
    inverseSlope1 = 0.0f;
    inverseSlope2 = 0.0f;

    if (y2 - y1 != 0.0f)
    {
        inverseSlope1 = (float)(x2 - x1) / abs(y2 - y1);
    }

    if (y2 - y0 != 0.0f)
    {
        inverseSlope2 = (float)(x2 - x0) / abs(y2 - y0);
    }

    if (y1 - y0 == 0.0f)
    {
        return;
    }

    for (int y = y1; y <= y2; ++y)
    {
        // just another formula
        int xStart = x1 + (y - y1) * inverseSlope1;
        int xEnd = x0 + (y - y0) * inverseSlope2;

        if (xEnd < xStart)
        {
            swapInt(&xStart, &xEnd);
        }

        for (int x = xStart; x < xEnd; ++x)
        {
            DrawPixel(x, y, 0xFFFF00FF);
        }
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

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    DrawLine(x0, y0, x1, y1, color);
    DrawLine(x1, y1, x2, y2, color);
    DrawLine(x2, y2, x0, y0, color);
}
