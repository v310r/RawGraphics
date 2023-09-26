#include "clipping.h"

#include <math.h>


#define NUM_PLANES 6
plane_t g_frustumPlanes[NUM_PLANES];

void InitFrustumPlanes(float fovX, float fovY, float zNear, float zFar)
{
    float XcosHalfFOV = cos(fovX / 2.0f);
    float XsinHalfFOV = sin(fovX / 2.0f);

    float YcosHalfFOV = cos(fovX / 2.0f);
    float YsinHalfFOV = sin(fovX / 2.0f);


    g_frustumPlanes[LEFT_FRUSTUM_PLANE].point = vec3New(0.0f, 0.0f, 0.0f);
    g_frustumPlanes[LEFT_FRUSTUM_PLANE].normal.x = XcosHalfFOV;
    g_frustumPlanes[LEFT_FRUSTUM_PLANE].normal.y = 0;
    g_frustumPlanes[LEFT_FRUSTUM_PLANE].normal.z = XsinHalfFOV;

    g_frustumPlanes[RIGHT_FRUSTUM_PLANE].point = vec3New(0.0f, 0.0f, 0.0f);
    g_frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.x = -XcosHalfFOV;
    g_frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
    g_frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.z = XsinHalfFOV;

    g_frustumPlanes[TOP_FRUSTUM_PLANE].point = vec3New(0.0f, 0.0f, 0.0f);
    g_frustumPlanes[TOP_FRUSTUM_PLANE].normal.x = 0;
    g_frustumPlanes[TOP_FRUSTUM_PLANE].normal.y = -YcosHalfFOV;
    g_frustumPlanes[TOP_FRUSTUM_PLANE].normal.z = YsinHalfFOV;

    g_frustumPlanes[BOTTOM_FRUSTUM_PLANE].point = vec3New(0.0f, 0.0f, 0.0f);
    g_frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
    g_frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.y = YcosHalfFOV;
    g_frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.z = YsinHalfFOV;

    g_frustumPlanes[NEAR_FRUSTUM_PLANE].point = vec3New(0.0f, 0.0f, zNear);
    g_frustumPlanes[NEAR_FRUSTUM_PLANE].normal.x = 0;
    g_frustumPlanes[NEAR_FRUSTUM_PLANE].normal.y = 0;
    g_frustumPlanes[NEAR_FRUSTUM_PLANE].normal.z = 1;

    g_frustumPlanes[FAR_FRUSTUM_PLANE].point = vec3New(0.0f, 0.0f, zFar);
    g_frustumPlanes[FAR_FRUSTUM_PLANE].normal.x = 0;
    g_frustumPlanes[FAR_FRUSTUM_PLANE].normal.y = 0;
    g_frustumPlanes[FAR_FRUSTUM_PLANE].normal.z = -1;
}

polygon_t CreatePolygonFromTriangle(vec3_t v0, vec3_t v1, vec3_t v2, tex2_t t0, tex2_t t1, tex2_t t2)
{
    polygon_t polygon =
    {
        .vertices = {v0, v1, v2},
        .texcoords = {t0, t1, t2},
        .numVertices = 3
    };

    return polygon;
}

static float floatLerp(float a, float b, float t)
{
    return a + t * (b - a);
}

static void clipPolygonAgainstPlane(polygon_t* polygon, int plane)
{
    vec3_t planePoint = g_frustumPlanes[plane].point;
    vec3_t planeNormal = g_frustumPlanes[plane].normal;

    vec3_t insideVertices[MAX_NUM_POLY_VERTICES];
    tex2_t insideTexcoords[MAX_NUM_POLY_VERTICES];
    int numInsideVertices = 0;

    vec3_t* currentVertex  = &polygon->vertices[0];
    tex2_t* currentTexcoord = &polygon->texcoords[0];

    vec3_t* previousVertex = &polygon->vertices[polygon->numVertices -1];
    tex2_t* previousTexcoord = &polygon->texcoords[polygon->numVertices - 1];

    float currentDot  = 0;
    float previousDot = vec3Dot(vec3Sub(*previousVertex, planePoint), planeNormal);

    // number of vertices can be up to ten
    while (currentVertex != &polygon->vertices[polygon->numVertices])
    {
        currentDot = vec3Dot(vec3Sub(*currentVertex, planePoint), planeNormal);

        if (currentDot * previousDot < 0.0f)
        {
            float t = previousDot / (previousDot - currentDot);

            // I = Qp + t(Qc-Qp)
            vec3_t intersectionPoint =
            {
                .x = floatLerp(previousVertex->x, currentVertex->x, t),
                .y = floatLerp(previousVertex->y, currentVertex->y, t),
                .z = floatLerp(previousVertex->z, currentVertex->z, t)
            };

            tex2_t interpolatedTexcoord =
            {
                .u = floatLerp(previousTexcoord->u, currentTexcoord->u, t),
                .v = floatLerp(previousTexcoord->v, currentTexcoord->v, t)
            };

            insideVertices[numInsideVertices] = intersectionPoint;
            insideTexcoords[numInsideVertices] = interpolatedTexcoord;
            ++numInsideVertices;
        }

        if (currentDot > 0.0f)
        {
            insideVertices[numInsideVertices] = vec3Clone(currentVertex);
            insideTexcoords[numInsideVertices] = tex2Clone(currentTexcoord);
            ++numInsideVertices;
        }

        previousDot = currentDot;
        previousVertex = currentVertex;
        previousTexcoord = currentTexcoord;
        ++currentVertex;
        ++currentTexcoord;
    }

    for (int i = 0; i < numInsideVertices; ++i)
    {
        polygon->vertices[i] = insideVertices[i];
        polygon->texcoords[i] = insideTexcoords[i];
    }

    polygon->numVertices = numInsideVertices;
}

void clipPolygon(polygon_t* polygon)
{
    clipPolygonAgainstPlane(polygon, LEFT_FRUSTUM_PLANE);
    clipPolygonAgainstPlane(polygon, RIGHT_FRUSTUM_PLANE);
    clipPolygonAgainstPlane(polygon, TOP_FRUSTUM_PLANE);
    clipPolygonAgainstPlane(polygon, BOTTOM_FRUSTUM_PLANE);
    clipPolygonAgainstPlane(polygon, NEAR_FRUSTUM_PLANE);
    clipPolygonAgainstPlane(polygon, FAR_FRUSTUM_PLANE);
}

void GetTrianglesFromPolygon(polygon_t* polygon, triangle_t* triangles, int* numTriangles)
{
    int index0 = 0;
    for (int i = 0; i < polygon->numVertices - 2; ++i)
    {
        int index1 = i + 1;
        int index2 = i + 2;

        triangles[i].points[0] = vec4FromVec3(polygon->vertices[index0]);
        triangles[i].points[1] = vec4FromVec3(polygon->vertices[index1]);
        triangles[i].points[2] = vec4FromVec3(polygon->vertices[index2]);

        triangles[i].texcoords[0] = polygon->texcoords[index0];
        triangles[i].texcoords[1] = polygon->texcoords[index1];
        triangles[i].texcoords[2] = polygon->texcoords[index2];
    }

    *numTriangles = polygon->numVertices - 2;
}
