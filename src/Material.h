#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Material
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    bool reflective;
    float shininess;

    Material();
    Material(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _shininess);
};

#endif
