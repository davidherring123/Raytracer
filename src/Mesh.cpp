#include "Mesh.h"

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MatrixStack.h"

using namespace std;
using namespace glm;

Mesh::Mesh()
{
}

Mesh::Mesh(vector<float> &posBuf, vector<float> &norBuf, mat4 _E) : E(_E), invE(inverse(E)), triangles({})
{

    float minX = posBuf.at(0);
    float maxX = posBuf.at(0);

    float minY = posBuf.at(1);
    float maxY = posBuf.at(1);

    float minZ = posBuf.at(2);
    float maxZ = posBuf.at(2);

    for (int i = 0; i < posBuf.size(); i += 9)
    {
        vector<vec3> triangle;
        for (int j = 0; j < 3; j++)
        {
            float x = posBuf.at(i + j * 3);
            minX = std::min(x, minX);
            maxX = std::max(x, maxX);

            float y = posBuf.at(i + j * 3 + 1);
            minY = std::min(y, minY);
            maxY = std::max(y, maxY);

            float z = posBuf.at(i + j * 3 + 2);
            minZ = std::min(z, minZ);
            maxZ = std::max(z, maxZ);

            triangle.push_back(vec3(x, y, z));
        }

        for (int j = 0; j < 3; j++)
        {
            float norX = norBuf.at(i + j * 3);
            float norY = norBuf.at(i + j * 3 + 1);
            float norZ = norBuf.at(i + j * 3 + 2);
            triangle.push_back(vec3(norX, norY, norZ));
        }

        triangles.push_back(triangle);
    }

    float cx = (minX + maxX) * 0.5f;
    float cy = (minY + maxY) * 0.5f;
    float cz = (minZ + maxZ) * 0.5f;
    center = vec3(E * vec4(cx, cy, cz, 1.0));

    radius = length(vec3(E * vec4((maxX - minX) * 0.5f, (maxY - minY) * 0.5f, (maxZ - minZ) * 0.5f, 1.0)));
}

Mesh::~Mesh()
{
}

Hit Mesh::intersect(Ray &ray, Hit &closest)
{
    vec3 pc = ray.orig - center;

    float a = dot(ray.dir, ray.dir);
    float b = 2 * dot(ray.dir, pc);
    float c = dot(pc, pc) - radius;
    float d = b * b - 4 * a * c;

    if (d < 0)
    {
        return closest;
    }

    vec3 orig = vec3(invE * vec4(ray.orig, 1.0));
    vec3 dir = normalize(vec3(invE * vec4(ray.dir, 0.0)));

    vec3 tvec, pvec, qvec;
    float t, u, v, det, invDet;

    for (vector<vec3> tri : triangles)
    {
        vec3 v0 = tri.at(0);
        vec3 v1 = tri.at(1);
        vec3 v2 = tri.at(2);

        vec3 n0 = tri.at(3);
        vec3 n1 = tri.at(4);
        vec3 n2 = tri.at(5);

        vec3 edge1 = v1 - v0;
        vec3 edge2 = v2 - v0;

        pvec = cross(dir, edge2);

        det = dot(edge1, pvec);
        invDet = 1 / det;

        if (abs(det) < 1e-4)
        {
            continue;
        }

        tvec = orig - v0;

        u = dot(tvec, pvec);
        if (u < 0 || u > det)
        {
            continue;
        }

        qvec = cross(tvec, edge1);

        v = dot(dir, qvec);
        if (v < 0 || u + v > det)
        {
            continue;
        }

        t = dot(edge2, qvec) * invDet;

        if (t < 0)
        {
            continue;
        }

        u *= invDet;
        v *= invDet;

        Hit h;

        h.x = vec3(E * vec4(orig + t * dir, 1.0));
        h.n = normalize(n0 * (1.0f - u - v) + n1 * u + n2 * v);
        h.t = length(h.x - ray.orig);
        h.m = getMaterial();

        if (dot(ray.dir, h.x - ray.orig) < 0)
        {
            h.t = -h.t;
        }

        closest = h;
    }

    return closest;
}