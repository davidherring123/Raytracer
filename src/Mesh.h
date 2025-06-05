#pragma once
#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hit.h"
#include "Shape.h"

class Mesh : public Shape
{
public:
    Mesh();
    Mesh(std::vector<float> &posBuf, std::vector<float> &norBuf, glm::mat4 _E);
    virtual ~Mesh();
    virtual Hit intersect(Ray &ray, Hit &closest);

private:
    glm::mat4 E;
    glm::mat4 invE;
    std::vector<std::vector<glm::vec3>> triangles;
    float radius;
    glm::vec3 center;
};

#endif
