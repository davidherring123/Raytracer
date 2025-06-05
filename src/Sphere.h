#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include <string>
#include <vector>
#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hit.h"
#include "Shape.h"

class Sphere : public Shape
{
public:
    Sphere();
    Sphere(glm::vec3 _translation, float _rotAngle, glm::vec3 _rotAxis, glm::vec3 _scale);
    virtual ~Sphere();
    virtual Hit intersect(Ray &ray, Hit &closest);

private:
    glm::vec3 translation;
    float rotAngle;
    glm::vec3 rotAxis;
    glm::vec3 scale;
    glm::mat4 E;
    glm::mat4 invE;
};

#endif
