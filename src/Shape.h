#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <vector>
#include <memory>

#include "Hit.h"
#include "Material.h"

class Shape
{
public:
    Shape() = default;
    virtual ~Shape() = default;
    virtual Hit intersect(Ray &ray, Hit &closest) = 0;
    void setMaterial(Material m) { this->m = m; };
    Material getMaterial() { return m; };

private:
    Material m;
};

#endif
