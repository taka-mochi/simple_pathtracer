#pragma once

#include "vector.h"
#include "ray.h"
#include "constant.h"

class BoundingBox {
public:
  BoundingBox(const Vector3 &minPos = Vector3(0,0,0), const Vector3 &maxPos = Vector3(0,0,0))
    : min_(minPos)
    , max_(maxPos)
    , centerPos_((minPos+maxPos)/2.0)
  {
  }

  double CalcVolume() const {
    return (max_.x-min_.x)*(max_.y-min_.y)*(max_.z-min_.z);
  }

  void SetBox(const Vector3 &min_, const Vector3 &max_)
  {
    this->min_ = min_; this->max_ = max_;
    centerPos_ = (min_+max_)/2.0;
  }

  bool Intersect(const Ray &ray, double &distance) const {
    Vector3 t_min(INF, INF, INF), t_max(-INF, -INF, -INF);
    double fastest_out_t = INF;
    double latest_in_t = -INF;

    if (fabs(ray.dir.x) >= EPS) {
      double t1 = (min_.x - ray.begin.x)/ray.dir.x;
      double t2 = (max_.x - ray.begin.x)/ray.dir.x;
      if (t1 < 0 && t2 < 0) {
        return false;
      }
      t_min.x = std::min(t1, t2);
      t_max.x = std::max(t1, t2);
    } else {
      if (ray.begin.x > min_.x && ray.begin.x < max_.x) {
        t_min.x = EPS; t_max.x = EPS*2;
      } else {
        return false;
      }
    }
    fastest_out_t = std::min(fastest_out_t, t_max.x);
    latest_in_t = std::max(latest_in_t, t_min.x);
    if (latest_in_t > fastest_out_t) return false;

    if (fabs(ray.dir.y) >= EPS) {
      double t1 = (min_.y - ray.begin.y)/ray.dir.y;
      double t2 = (max_.y - ray.begin.y)/ray.dir.y;
      if (t1 < 0 && t2 < 0) {
        return false;
      }
      t_min.y = std::min(t1, t2);
      t_max.y = std::max(t1, t2);
    } else {
      if (ray.begin.y > min_.y && ray.begin.y < max_.y) {
        t_min.y = EPS; t_max.y = EPS*2;
      } else {
        return false;
      }
    }
    fastest_out_t = std::min(fastest_out_t, t_max.y);
    latest_in_t = std::max(latest_in_t, t_min.y);
    if (latest_in_t > fastest_out_t) return false;

    if (fabs(ray.dir.z) >= EPS) {
      double t1 = (min_.z - ray.begin.z)/ray.dir.z;
      double t2 = (max_.z - ray.begin.z)/ray.dir.z;
      if (t1 < 0 && t2 < 0) {
        return false;
      }
      t_min.z = std::min(t1, t2);
      t_max.z = std::max(t1, t2);
    } else {
      if (ray.begin.z > min_.z && ray.begin.z < max_.z) {
        t_min.z = EPS; t_max.z = EPS*2;
      } else {
        return false;
      }
    }
    fastest_out_t = std::min(fastest_out_t, t_max.z);
    latest_in_t = std::max(latest_in_t, t_min.z);
    if (latest_in_t > fastest_out_t) return false;

    if (latest_in_t > 0)
      distance = latest_in_t;// = ray.begin + ray.dir * latest_in_t;
    else
      distance = 0;

    return true;
  }

  static BoundingBox CompoundBoxes(const BoundingBox &b1, const BoundingBox &b2) {
    return BoundingBox(Vector3(std::min(b1.min().x, b2.min().x), std::min(b1.min().y, b2.min().y), std::min(b1.min().z, b2.min().z)), 
        Vector3(std::max(b1.max().x, b2.max().x), std::max(b1.max().y, b2.max().y), std::max(b1.max().z, b2.max().z)));
  }

  const Vector3 &min() const {return min_;}
  const Vector3 &max() const {return max_;}
  const Vector3 &position() const {return centerPos_;}

private:
  Vector3 min_;
  Vector3 max_;
  Vector3 centerPos_;
};
