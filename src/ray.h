#pragma once

#include "vector.h"

class Ray {
public:
  Ray(const Vector3 &begin, const Vector3 &dir)
    : m_begin(begin)
    , m_dir(dir)
  {
  }

private:
  Vector3 m_begin;
  Vector3 m_dir;
};
