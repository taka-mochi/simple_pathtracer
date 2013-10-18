#pragma once

#include "color.h"

class Ray;
class HitInformation;

class SceneObject {
public:
  enum REFLECTION_TYPE {
    REFLECTION_TYPE_LAMBERT,	// Š®‘SŠgŽU–Ê
	REFLECTION_TYPE_SPECULAR,	// ‹¾–Ê
	REFLECTION_TYPE_REFRACTION,	// ƒKƒ‰ƒX
  };
public:
  SceneObject(const Color &color_, const Color &emission_, const REFLECTION_TYPE reflection_type_)
    : color(color_)
    , emission(emission_)
	, reflection_type(reflection_type_)
  {
  }
  virtual ~SceneObject() {}

  virtual bool Intersect(const Ray &ray, HitInformation &hit) const = 0;

  Color color;
  Color emission;
  REFLECTION_TYPE reflection_type;
};
