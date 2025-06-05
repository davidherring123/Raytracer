
#include "Sphere.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MatrixStack.h"
#include <limits>

using namespace std;
using namespace glm;

Sphere::Sphere() : translation(vec3(0, 0, 0)), rotAngle(0), rotAxis(1, 1, 1), scale(vec3(1, 1, 1))
{
}

Sphere::Sphere(vec3 _translation, float _rotAngle, vec3 _rotAxis, vec3 _scale) : translation(_translation), rotAngle(_rotAngle), rotAxis(_rotAxis), scale(_scale)
{
    auto M = make_shared<MatrixStack>();
    M->translate(translation);

    vec3 axis = normalize(rotAxis);
    M->rotate(rotAngle, axis);

    M->scale(scale);
    E = M->topMatrix();
    invE = inverse(E);
}

Sphere::~Sphere()
{
}

Hit Sphere::intersect(Ray &ray, Hit &closest)
{
    vec3 p = vec3(invE * vec4(ray.orig, 1.0));
    vec3 v = normalize(vec3(invE * vec4(ray.dir, 0.0)));

    float a = dot(v, v);
    float b = 2 * dot(v, p);
    float c = dot(p, p) - 1;
    float d = b * b - 4 * a * c;

    if (d < 0)
    {
        return closest;
    }

    float t0 = (-b - sqrt(d)) / (2 * a);
    float t1 = (-b + sqrt(d)) / (2 * a);

    float t = -1.0f;
    if (t0 > 0)
    {
        t = t0;
    }
    else if (t1 > 0)
    {
        t = t1;
    }

    if (t < 0.0f)
    {
        return closest;
    }

    Hit h;

    h.x = vec3(E * vec4(p + t * v, 1.0));
    h.n = normalize(vec3(transpose(invE) * vec4(p + t * v, 0.0)));
    h.t = length(h.x - ray.orig);

    if (dot(ray.dir, h.x - ray.orig) < 0)
    {
        h.t = -t;
    }

    if (closest.t < h.t)
    {
        return closest;
    }

    closest.t = h.t;
    closest.x = h.x;
    closest.n = h.n;
    closest.m = getMaterial();

    return closest;
}