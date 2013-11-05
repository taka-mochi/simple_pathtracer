#pragma once

#include "tools/Vector.h"

namespace SimpleRenderer {

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

}
