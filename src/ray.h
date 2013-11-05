#pragma once

#include "vector.h"

namespace SimpleRenderer {

class Ray {
public:
  Ray(const Vector3 &begin_, const Vector3 &dir_)
    : begin(begin_)
    , dir(dir_)
  {
  }

  Vector3 begin;
  Vector3 dir;
};

}
