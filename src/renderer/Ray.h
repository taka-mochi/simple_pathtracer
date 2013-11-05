#pragma once

#include "tools/Vector.h"

namespace SimpleRenderer {

class Ray {
public:
  Ray(const Vector3 &begin_, const Vector3 &dir_)
    : orig(begin_)
    , dir(dir_)
  {
  }

  Vector3 orig;
  Vector3 dir;
};

}
