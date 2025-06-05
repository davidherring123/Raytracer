#include "Plane.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MatrixStack.h"

using namespace std;
using namespace glm;

Plane::Plane() : translation(vec3(0, 0, 0))
{
}

Plane::Plane(vec3 _translation, vec3 _normal) : translation(_translation), normal(_normal)
{
}

Plane::~Plane()
{
}

Hit Plane::intersect(Ray &ray, Hit &closest)
{
    float t = glm::dot(normal, translation - ray.orig) / dot(normal, ray.dir);

    if (t < 0 || t >= closest.t)
    {
        return closest;
    }

    closest.t = t;
    closest.x = ray.orig + ray.dir * t;
    closest.n = normal;
    closest.m = getMaterial();

    return closest;
}