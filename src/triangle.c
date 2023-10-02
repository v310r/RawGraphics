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

///////////////////////////////////////////////////////////////////////////////
// Return the barycentric weights alpha, beta, and gamma for point p
///////////////////////////////////////////////////////////////////////////////
//
//         (B)
//         /|\
//        / | \
//       /  |  \
//      /  (P)  \
//     /  /   \  \
//    / /       \ \
//   //           \\
//  (A)------------(C)
//
///////////////////////////////////////////////////////////////////////////////
vec3_t GetBarycentricWeights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
    vec2_t ac = vec2Sub(c, a);
    vec2_t ab = vec2Sub(b, a);
    vec2_t ap = vec2Sub(p, a);
    vec2_t pc = vec2Sub(c, p);
    vec2_t pb = vec2Sub(b, p);

    float areaParallelogramABC = (ac.x * ab.y - ac.y * ab.x);

    float alpha = (pc.x * pb.y - pc.y * pb.x) / areaParallelogramABC;

    float beta = (ac.x * ap.y - ac.y * ap.x) / areaParallelogramABC;

    float gamma = 1.0f - alpha - beta;

    vec3_t weights = { alpha, beta, gamma };
    return weights;
}

void DrawTexel(int x, int y, upng_t* texture, vec4_t pointA, vec4_t pointB, vec4_t pointC, tex2_t a_uv, tex2_t b_uv, tex2_t c_uv)
{
    vec2_t pointP = { x, y };
    vec2_t a = vec2FromVec4(pointA);
    vec2_t b = vec2FromVec4(pointB);
    vec2_t c = vec2FromVec4(pointC);

    vec3_t weights = GetBarycentricWeights(a, b, c, pointP);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    float interpolated_u;
    float interpolated_v;
    float interpolated_reciprocal_w;

    interpolated_u = (a_uv.u / pointA.w) * alpha + (b_uv.u / pointB.w) * beta + (c_uv.u / pointC.w) * gamma;
    interpolated_v = (a_uv.v / pointA.w) * alpha + (b_uv.v / pointB.w) * beta + (c_uv.v / pointC.w) * gamma;

    interpolated_reciprocal_w = (1.0f / pointA.w) * alpha + (1.0f / pointB.w) * beta + (1.0f / pointC.w) * gamma;


    interpolated_u /= interpolated_reciprocal_w;
    interpolated_v /= interpolated_reciprocal_w;

    int textureWidth = upng_get_width(texture);
    int textureHeight = upng_get_height(texture);

    int textureX = interpolated_u * textureWidth;
    int textureY = interpolated_v * textureHeight;

    if (textureX >= textureWidth)
    {
        textureX = textureWidth - 1;
    }

    if (textureX < 0)
    {
        textureX = 0;
    }

    if (textureY >= textureHeight)
    {
        textureY = textureHeight - 1;
    }

    if (textureY < 0)
    {
        textureY = 0;
    }

    // Adjust 1/w so the pixels that are closer to the camera have smaller values
    interpolated_reciprocal_w = 1.0f - interpolated_reciprocal_w;

    if (y < g_WindowHeight && x < g_WindowWidth && y >= 0 && x >= 0)
    {
        if (interpolated_reciprocal_w < g_zBuffer[(g_WindowWidth * y) + x])
        {
            uint32_t* textureBuffer = (uint32_t*)upng_get_buffer(texture);

            DrawPixel(x, y, textureBuffer[(textureWidth * textureY) + textureX]);

            g_zBuffer[(g_WindowWidth * y) + x] = interpolated_reciprocal_w;
        }
    }
}

void DrawTexturedTriangle(
    int x0, int y0, float z0, float w0, float u0, float v0,
    int x1, int y1, float z1, float w1, float u1, float v1,
    int x2, int y2, float z2, float w2, float u2, float v2,
    upng_t* texture)
{
    // y0 < y1 < y2 (sorting)
    if (y0 > y1)
    {
        swapInt(&y0, &y1);
        swapInt(&x0, &x1);
        swapFloat(&z0, &z1);
        swapFloat(&w0, &w1);
        swapFloat(&u0, &u1);
        swapFloat(&v0, &v1);
    }

    if (y1 > y2)
    {
        swapInt(&y1, &y2);
        swapInt(&x1, &x2);
        swapFloat(&z1, &z2);
        swapFloat(&w1, &w2);
        swapFloat(&u1, &u2);
        swapFloat(&v1, &v2);
    }

    if (y0 > y1)
    {
        swapInt(&y0, &y1);
        swapInt(&x0, &x1);
        swapFloat(&z0, &z1);
        swapFloat(&w0, &w1);
        swapFloat(&u0, &u1);
        swapFloat(&v0, &v1);
    }
    
    // flip the V component to account for inverted UV-coordinates(V grows downwards)
    v0 = 1.0f - v0;
    v1 = 1.0f - v1;
    v2 = 1.0f - v2;

    vec4_t pointA = { x0, y0, z0, w0 };
    vec4_t pointB = { x1, y1, z1, w1 };
    vec4_t pointC = { x2, y2, z2, w2 };

    tex2_t a_uv = { u0, v0 };
    tex2_t b_uv = { u1, v1 };
    tex2_t c_uv = { u2, v2 };

    // flat-bottom filling (basically top filling)
    // 
    // how much x changes when y is constantly increasing 1 by one (scanline)
    float inverseSlope1 = 0.0f;
    float inverseSlope2 = 0.0f;

    if (y1 - y0 != 0)
    {
        inverseSlope1 = (float)(x1 - x0) / abs(y1 - y0);
    }

    if (y2 - y0 != 0)
    {
        inverseSlope2 = (float)(x2 - x0) / abs(y2 - y0);
    }

    if (y1 - y0 != 0)
    {
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
                DrawTexel(x, y, texture, pointA, pointB, pointC, a_uv, b_uv, c_uv);
            }
        }
    }


    // flat-top filling (basically bottom filling)
    // 
    // how much x changes when y is constantly increasing 1 by one (scanline)
    inverseSlope1 = 0.0f;
    inverseSlope2 = 0.0f;

    if (y2 - y1 != 0)
    {
        inverseSlope1 = (float)(x2 - x1) / abs(y2 - y1);
    }

    if (y2 - y0 != 0)
    {
        inverseSlope2 = (float)(x2 - x0) / abs(y2 - y0);
    }

    if (y2 - y1 != 0)
    {
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
                DrawTexel(x, y, texture, pointA, pointB, pointC, a_uv, b_uv, c_uv);
            }
        }
    }
}

void DrawTrianglePixel(
    int x, int y,
    uint32_t color,
    vec4_t pointA, vec4_t pointB, vec4_t pointC)
{
    vec2_t pointP = { x, y };
    vec2_t a = vec2FromVec4(pointA);
    vec2_t b = vec2FromVec4(pointB);
    vec2_t c = vec2FromVec4(pointC);

    vec3_t weights = GetBarycentricWeights(a, b, c, pointP);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    float interpolated_u;
    float interpolated_v;
    float interpolated_reciprocal_w;


    interpolated_reciprocal_w = (1.0f / pointA.w) * alpha + (1.0f / pointB.w) * beta + (1.0f / pointC.w) * gamma;

    // Adjust 1/w so the pixels that are closer to the camera have smaller values
    interpolated_reciprocal_w = 1.0f - interpolated_reciprocal_w;

    if (y < g_WindowHeight && x < g_WindowWidth && y >= 0 && x >= 0)
    {
        if (interpolated_reciprocal_w < g_zBuffer[(g_WindowWidth * y) + x])
        {
            DrawPixel(x, y, color);

            g_zBuffer[(g_WindowWidth * y) + x] = interpolated_reciprocal_w;
        }
    }

}

void DrawFilledTriangle(
    int x0, int y0, float z0, float w0,
    int x1, int y1, float z1, float w1,
    int x2, int y2, float z2, float w2,
    uint32_t color)
{
    // y0 < y1 < y2 (sorting)
    if (y0 > y1)
    {
        swapInt(&y0, &y1);
        swapInt(&x0, &x1);
        swapFloat(&z0, &z1);
        swapFloat(&w0, &w1);
    }

    if (y1 > y2)
    {
        swapInt(&y1, &y2);
        swapInt(&x1, &x2);
        swapFloat(&z1, &z2);
        swapFloat(&w1, &w2);
    }

    if (y0 > y1)
    {
        swapInt(&y0, &y1);
        swapInt(&x0, &x1);
        swapFloat(&z0, &z1);
        swapFloat(&w0, &w1);
    }

    vec4_t pointA = { x0, y0, z0, w0 };
    vec4_t pointB = { x1, y1, z1, w1 };
    vec4_t pointC = { x2, y2, z2, w2 };

    // flat-bottom filling (basically top filling)
    // 
    // how much x changes when y is constantly increasing 1 by one (scanline)
    float inverseSlope1 = 0.0f;
    float inverseSlope2 = 0.0f;

    if (y1 - y0 != 0)
    {
        inverseSlope1 = (float)(x1 - x0) / abs(y1 - y0);
    }

    if (y2 - y0 != 0)
    {
        inverseSlope2 = (float)(x2 - x0) / abs(y2 - y0);
    }

    if (y1 - y0 != 0)
    {
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
                DrawTrianglePixel (x, y, color, pointA, pointB, pointC);
            }
        }
    }

    // flat-top filling (basically bottom filling)
    // 
    // how much x changes when y is constantly increasing 1 by one (scanline)
    inverseSlope1 = 0.0f;
    inverseSlope2 = 0.0f;

    if (y2 - y1 != 0)
    {
        inverseSlope1 = (float)(x2 - x1) / abs(y2 - y1);
    }

    if (y2 - y0 != 0)
    {
        inverseSlope2 = (float)(x2 - x0) / abs(y2 - y0);
    }

    if (y2 - y1 != 0)
    {
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
                DrawTrianglePixel(x, y, color, pointA, pointB, pointC);
            }
        }
    }

    //int My = y1;
    //int Mx = (((float)(x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

    //FillFlatBottomTriangle(x0, y0, x1, y1, Mx, My, color);

    //FillFlatTopTriangle(x1, y1, Mx, My, x2, y2, color);

}

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    DrawLine(x0, y0, x1, y1, color);
    DrawLine(x1, y1, x2, y2, color);
    DrawLine(x2, y2, x0, y0, color);
}
