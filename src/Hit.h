#pragma once
#ifndef HIT_H
#define HIT_H

#define GLM_FORCE_RADIANS
#include <limits>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Material.h"

struct Ray
{
    glm::vec3 orig;
    glm::vec3 dir;
};

class Hit
{
public:
    Hit() : x(0), n(0), t(std::numeric_limits<float>::infinity()) {}
    Hit(const glm::vec3 &x, const glm::vec3 &n, float t)
    {
        this->x = x;
        this->n = n;
        this->t = t;
    }

    bool didHit() { return t < std::numeric_limits<float>::infinity(); }

    Material m;
    glm::vec3 x;
    glm::vec3 n;
    float t;
};

#endif
