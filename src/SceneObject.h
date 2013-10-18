#pragma once

class Ray;
class HitInformation;

class SceneObject {
public:
  virtual ~SceneObject() {}

  virtual bool Intersect(const Ray &ray, HitInformation &hit) = 0;
};
