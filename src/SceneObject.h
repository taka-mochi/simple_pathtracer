#pragma once

#include "color.h"
#include "material.h"

class Ray;
class HitInformation;

class SceneObject {
public:
  SceneObject(const Material &material_)
    : material(material_)
    , position(0,0,0)
  {
  }
  virtual ~SceneObject() {}

  virtual bool Intersect(const Ray &ray, HitInformation &hit) const = 0;

  Material material;
  Vector3 position;
};
