#pragma once
#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <vector>
#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hit.h"
#include "Shape.h"

class Plane : public Shape
{
public:
    Plane();
    Plane(glm::vec3 _translation, glm::vec3 normal);
    virtual ~Plane();
    virtual Hit intersect(Ray &ray, Hit &closest);

private:
    glm::vec3 translation;
    glm::vec3 normal;
};

#endif
