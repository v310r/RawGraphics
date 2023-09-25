#include "matrix.h"
#include <math.h>


mat4_t mat4Identity(void)
{
    mat4_t m =
    {
      {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
      }
    };

    return m;
}

mat4_t mat4MakeScale(float sx, float sy, float sz)
{
    mat4_t m = mat4Identity();

    m.m[0][0] = sx;
    m.m[1][1] = sy;
    m.m[2][2] = sz;

    return m;
}

mat4_t mat4MakeRotationX(float angle)
{
    // | 1  0  0 0 |
    // | 0  c -s 0 |
    // | 0  s  c 0 |
    // | 0  0  0 1 |

    float c = cos(angle);
    float s = sin(angle);

    mat4_t m = mat4Identity();

    m.m[1][1] = c;
    m.m[1][2] = -s;
    m.m[2][1] = s;
    m.m[2][2] = c;

    return m;
}

mat4_t mat4MakeRotationY(float angle)
{
    // | c  0 s 0 |
    // | 0  1 0 0 |
    // | -s 0 c 0 |
    // | 0  0 0 1 |

    float c = cos(angle);
    float s = sin(angle);

    mat4_t m = mat4Identity();

    m.m[0][0] = c;
    m.m[0][2] = s;
    m.m[2][0] = -s;
    m.m[2][2] = c;

    return m;
}

mat4_t mat4MakeRotationZ(float angle)
{
    // | c  -s 0 0 |
    // | s  c  0 0 |
    // | 0  0  1 0 |
    // | 0  0  0 1 |

    float c = cos(angle);
    float s = sin(angle);

    mat4_t m = mat4Identity();

    m.m[0][0] = c;
    m.m[0][1] = -s;
    m.m[1][0] = s;
    m.m[1][1] = c;

    return m;
}

mat4_t mat4MakeTranslation(float tx, float ty, float tz)
{
    mat4_t m = mat4Identity();

    m.m[0][3] = tx;
    m.m[1][3] = ty;
    m.m[2][3] = tz;

    return m;
}


vec4_t mat4Mulvec4(mat4_t m, vec4_t v)
{
    vec4_t result;

    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;

    return result;
}

mat4_t mat4Mulmat4(mat4_t a, mat4_t b)
{
    mat4_t m;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            m.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
        }
    }

    return m;
}

mat4_t mat4MakePerspective(float fov, float aspect, float znear, float zfar)
{
    // |  (h/w)*1/tan(fov/2)              0              0                  0  |
    // |                   0   1/tan(fov/2)              0                  0  |
    // |                   0              0     zf/(zf-zn)   (-zf*zn)/(zf-zn)  |
    // |                   0              0              1                  0  |

    mat4_t m = { {{0.0f}} };
    m.m[0][0] = aspect * (1 / tan(fov / 2));
    m.m[1][1] = 1 / tan(fov / 2);
    m.m[2][2] = zfar / (zfar - znear);
    m.m[2][3] = (-zfar * znear) / (zfar - znear);
    m.m[3][2] = 1.0f;

    return m;
}                          

vec4_t mat4MulVec4PerspectiveProjection(mat4_t projectionMatrix, vec4_t v)
{
    vec4_t result = mat4Mulvec4(projectionMatrix, v);
    if (result.w != 0.0f)
    {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }

    return result;
}

mat4_t mat4LookAt(vec3_t eye, vec3_t target, vec3_t up)
{
    vec3_t z = vec3Sub(target, eye);
    z = vec3Normalize(z);
    vec3_t x = vec3Cross(up, z);
    x = vec3Normalize(x);
    vec3_t y = vec3Cross(z, x);

    mat4_t viewMatrix =
    {
        {
            { x.x, x.y, x.z,    -vec3Dot(x, eye) },
            { y.x, y.y, y.z,    -vec3Dot(y, eye) },
            { z.x, z.y, z.z,    -vec3Dot(z, eye) },
            { 0.0f, 0.0f, 0.0f,                1 }
        }
    };

    return viewMatrix;
}
