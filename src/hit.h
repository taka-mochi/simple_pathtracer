#pragma once

#include "vector.h"

class HitInformation {
public:
  HitInformation()
    : distance(0)
    , position()
    , normal()
  {
  }

  double distance;
  Vector3 position;
  Vector3 normal;
};
