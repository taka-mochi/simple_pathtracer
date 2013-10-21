#pragma once

#include "SceneObject.h"
#include "polygon.h"

class Quad : public SceneObject {
public:
  Quad(const Vector3 &anticlockwise_v0, const Vector3 &anticlockwise_v1, const Vector3 &anticlockwise_v2, const Vector3 &anticlockwise_v3, const Material &mat, const Vector3 &pos)
    : SceneObject(mat)
  {
    Vector3 normal(Polygon::calculateNormal(anticlockwise_v0, anticlockwise_v1, anticlockwise_v2));
    polygon[0] = new Polygon(anticlockwise_v0, anticlockwise_v1, anticlockwise_v2, normal, mat, pos);
    polygon[1] = new Polygon(anticlockwise_v0, anticlockwise_v2, anticlockwise_v3, normal, mat, pos);
  }
  Quad(const Quad &quad)
    : SceneObject(quad.material)
  {
    polygon[0] = new Polygon(*quad.polygon[0]);
    polygon[1] = new Polygon(*quad.polygon[1]);
  }
  virtual ~Quad() {
    delete polygon[0]; delete polygon[1];
  }

  bool Intersect(const Ray &ray, HitInformation &hit) const {
    return polygon[0]->Intersect(ray, hit) || polygon[1]->Intersect(ray, hit);
  }

  Polygon *polygon[2];
};
