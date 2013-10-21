#pragma once

#include "SceneObject.h"
#include "ray.h"
#include "quad.h"

class Box : public SceneObject {
public:
  Box(const Quad &leftSideQuad, const Quad &frontSideQuad, const Quad &rightSideQuad, const Quad &backSideQuad, const Quad &topSideQuad, const Quad &bottomSideQuad)
    : SceneObject(leftSideQuad.material)
  {
    quads[0] = new Quad(leftSideQuad);
    quads[1] = new Quad(frontSideQuad);
    quads[2] = new Quad(rightSideQuad);
    quads[3] = new Quad(backSideQuad);
    quads[4] = new Quad(topSideQuad);
    quads[5] = new Quad(bottomSideQuad);
  }

  virtual ~Box() {
    for (int i=0; i<6;  i++) {
      delete quads[i];
    }
  }

  bool Intersect(const Ray &ray, HitInformation &hit) const {
    for (int i=0; i<6; i++) {
      if (quads[i]->Intersect(ray, hit)) {
        return true;
      }
    }
    return false;
  }

  Quad *quads[6];
};
