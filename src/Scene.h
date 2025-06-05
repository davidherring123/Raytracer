#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hit.h"
#include "Shape.h"
#include "Camera.h"
#include "Image.h"

struct Light
{
    Light(glm::vec3 _position, float _intensity) : position(_position), intensity(_intensity) {}
    glm::vec3 position;
    float intensity;
};

class Scene
{
public:
    Scene();
    ~Scene();
    void addObject(std::shared_ptr<Shape> o) { objects.push_back(o); };
    void addLight(Light l) { lights.push_back(l); };
    void setCamera(std::shared_ptr<Camera> c) { cam = c; }
    void drawImage(std::shared_ptr<Image> img);

private:
    glm::vec3 getRayColor(Ray &r, int depth);

    std::vector<std::shared_ptr<Shape>> objects;
    std::vector<Light> lights;
    std::shared_ptr<Camera> cam;
};

#endif
