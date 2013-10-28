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

    double min_array[4] = {min_.x, min_.y, min_.z, 1.0};
    double max_array[4] = {max_.x, max_.y, max_.z, 1.0};
    double ray_dir_array[4] = {ray.dir.x, ray.dir.y, ray.dir.z, 1.0};
    double ray_start_array[4] = {ray.begin.x, ray.begin.y, ray.begin.z, 1.0};

    for (int i=0; i<3; i++) {
      double t_min = INF, t_max = -INF;
      if (fabs(ray_dir_array[i]) >= EPS) {
        double t1 = (min_array[i] - ray_start_array[i])/ray_dir_array[i];
        double t2 = (max_array[i] - ray_start_array[i])/ray_dir_array[i];
        t_min = std::min(t1, t2);
        t_max = std::max(t1, t2);
      } else {
        if (ray_start_array[i] > min_array[i] && ray_start_array[i] < max_array[i]) {
          t_min = EPS; t_max = EPS*2;
        } else {
          return false;
        }
      }
      fastest_out_t = std::min(fastest_out_t, t_max);
      latest_in_t = std::max(latest_in_t, t_min);
      if (latest_in_t > fastest_out_t) return false;
    }

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
