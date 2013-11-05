#pragma once

#include "Color.h"
#include "Material.h"
#include "BoundingBox.h"

namespace SimpleRenderer {

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
  BoundingBox boundingBox;
};

}
