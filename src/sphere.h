#pragma once

#include "SceneObject.h"
#include "color.h"
#include "ray.h"
#include "hit.h"

class Sphere : public SceneObject {
public:
  Sphere(double r, const Vector3 &pos, const Color &emission_, const Color &color_)
    : SceneObject(color_, emission_)
    , m_position(pos)
    , m_radius(r)
  {
  }

  bool Intersect(const Ray &ray, HitInformation &hit) const
  {
    // x: the origin of the ray
    // v: normalized direction of the ray
    // c: the center of this sphere
    const Vector3 &x = ray.begin;
    const Vector3 &v = ray.dir;
    const Vector3 &c = m_position;

    // ”»•ÊŽ®
    Vector3 c_minus_x = c-x;
    double v_dot_c_minus_x = v.dot(c_minus_x);
    double D1 = v_dot_c_minus_x * v_dot_c_minus_x;
    double D2 = v.lengthSq() * (c_minus_x.lengthSq() - m_radius*m_radius);
    double D = D1 - D2;
    if (D < 0) return false;

    double sqrtD = sqrt(D);
    double t1 = sqrtD + v_dot_c_minus_x;
    double t2 = -sqrtD + v_dot_c_minus_x;

    if (t1 <= EPS && t2 <= EPS) return false;
    
	if (t2 > EPS) hit.distance = t2;
	else hit.distance = t1;

    hit.position = x + v * hit.distance;
    hit.normal = hit.position - m_position; hit.normal.normalize();

    return true;
  }

private:
  double m_radius;
  Vector3 m_position;
};
