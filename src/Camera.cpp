#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "MatrixStack.h"
#include "Hit.h"

using namespace std;
using namespace glm;

Camera::Camera() : fovy((float)(45.0 * M_PI / 180.0)),
                   forward(1, 0, 0)
{
}

Camera::~Camera()
{
}

vector<Ray> Camera::generateCameraRays(int width, int height)
{
    vec3 forward = normalize(position - target);
    vec3 right = normalize(cross(forward, vec3(0, 1, 0)));
    vec3 up = cross(right, forward);

    float planeDist = 1.0f;
    float halfHeight = tan(fovy * 0.5f) * planeDist;
    float halfWidth = halfHeight * (float(width) / float(height));

    vector<Ray> rays;

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            float u = (i + 0.5f) / width, v = (j + 0.5f) / height;
            float xCam = (2.0f * u - 1.0f) * halfWidth;
            float yCam = (1.0f - 2.0f * v) * halfHeight;
            vec3 pCam(xCam, yCam, -planeDist);
            vec3 dir = normalize(pCam.x * right + pCam.y * up + pCam.z * forward);
            rays.push_back({position, dir});
        }
    }
    return rays;
}