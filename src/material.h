#pragma once

#include <functional>
#include "color.h"

namespace SimpleRenderer {

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

struct MaterialHash {
  std::hash<std::string> sh;
  MaterialHash() : sh() {}

  size_t operator()(const Material &mat) const {
    size_t type = static_cast<size_t>(mat.reflection_type);
    std::stringstream ss;
    ss << type << "_" << mat.color.toString() + "_" + mat.emission.toString() + "_" << mat.refraction_rate;
    return sh(ss.str());
  }
};

struct MaterialEq {
  bool operator()(const Material &mat1, const Material &mat2) const {
    return mat1.reflection_type == mat2.reflection_type && mat1.color == mat2.color && mat1.emission == mat2.emission && mat1.refraction_rate == mat2.refraction_rate;
  }
};

}
