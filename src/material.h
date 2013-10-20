#pragma once

#include "color.h"

class Material {
public:
  enum REFLECTION_TYPE {
    REFLECTION_TYPE_LAMBERT,	// Š®‘SŠgŽU–Ê
    REFLECTION_TYPE_SPECULAR,	// ‹¾–Ê
    REFLECTION_TYPE_REFRACTION,	// ƒKƒ‰ƒX
  };

public:
  Material(const REFLECTION_TYPE type = REFLECTION_TYPE_LAMBERT,
      const Vector3 emission_ = Vector3(0,0,0),
      const Vector3 color_ = Vector3(0,0,0),
      const double refraction_rate = 0.0) 
  : reflection_type(type)
  , emission(emission_)
  , color(color_)
  , refraction_rate(refraction_rate)
  {}


  REFLECTION_TYPE reflection_type;
  Color emission;
  Color color;
  double refraction_rate;
};
