#include <vector>
#include <iostream>

#include "Camera.h"
#include "Scene.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

const vec3 BACKGROUND(0.0f);
const int MAX_DEPTH = 20;

Scene::Scene()
{
}

Scene::~Scene()
{
}

vec3 Scene::getRayColor(Ray &r, int depth)
{
    if (depth > MAX_DEPTH)
    {
        return BACKGROUND;
    }

    Hit closest;

    for (shared_ptr<Shape> &o : objects)
    {
        closest = o->intersect(r, closest);
    }

    if (!closest.didHit())
    {
        return BACKGROUND;
    }

    vec3 color = closest.m.ambient;

    vec3 viewDir = normalize(r.orig - closest.x);

    for (auto &L : lights)
    {

        vec3 lightDirection = normalize(L.position - closest.x);

        Ray shadow;
        shadow.dir = lightDirection;
        shadow.orig = closest.x + closest.n * (float)1e-4;
        float lightDist = glm::length(L.position - closest.x);

        Hit sh;

        bool inShadow = false;

        for (shared_ptr<Shape> &o : objects)
        {

            sh = o->intersect(shadow, sh);
            if (sh.didHit() && sh.t < lightDist)
            {
                inShadow = true;
                break;
            }
        }

        if (inShadow)
        {
            continue;
        }

        float diff = std::max(dot(closest.n, lightDirection), 0.0f);
        color += closest.m.diffuse * L.intensity * diff;

        vec3 H = normalize(lightDirection + viewDir);
        float spec = pow(std::max(dot(closest.n, H), 0.0f), closest.m.shininess);
        color += closest.m.specular * L.intensity * spec;
    }

    if (closest.m.reflective)
    {
        Ray reflection;
        reflection.orig = closest.x + closest.n * 1e-4f;
        reflection.dir = reflect(normalize(r.dir), closest.n);

        vec3 reflectionColor = getRayColor(reflection, depth + 1);

        color = reflectionColor;
    }

    return color;
}

void Scene::drawImage(std::shared_ptr<Image> img)
{
    int W = img->getWidth(), H = img->getHeight(), idx = 0;
    auto rays = cam->generateCameraRays(W, H);

    for (auto &r : rays)
    {
        vec3 color = getRayColor(r, 0);

        int x = W - 1 - idx % W;
        int y = H - 1 - idx / W;

        img->setPixel(x, y,
                      int(std::clamp(color.r, 0.0f, 1.0f) * 255.0f),
                      int(std::clamp(color.g, 0.0f, 1.0f) * 255.0f),
                      int(std::clamp(color.b, 0.0f, 1.0f) * 255.0f));
        ++idx;
    }
}