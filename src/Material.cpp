#include "Material.h"
#include <GL/glew.h>

using namespace glm;

Material::Material(vec3 _ambient, vec3 _diffuse, vec3 _specular, float _shininess)
{
    ambient = _ambient;
    diffuse = _diffuse;
    specular = _specular;
    shininess = _shininess;
    reflective = false;
}

Material::Material()
{
    reflective = true;
}