#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Hit.h"

class Camera
{
public:
    Camera();
    virtual ~Camera();
    std::vector<Ray> generateCameraRays(int width, int height);
    void setPosition(glm::vec3 _position) { position = _position; };
    glm::vec3 getPosition() { return position; };
    void setTarget(glm::vec3 _target) { target = _target; };
    glm::vec3 getTarget() { return target; };
    void setFOVY(float _degree) { fovy = glm::radians(_degree); };
    float getFOVY() { return fovy; };

private:
    float fovy;
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 forward;
};

#endif
