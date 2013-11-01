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
  double CalcSurfaceArea() const {
    double diff_x = max_.x-min_.x, diff_y = max_.y-min_.y, diff_z = max_.z-min_.z;
    return diff_x*diff_y + diff_x*diff_z + diff_y*diff_z;
  }

  template <typename FLOATING> static FLOATING CalcSurfaceArea(FLOATING min[3], FLOATING max[3]) {
    FLOATING diff[3] = {max[0]-min[0], max[1]-min[1], max[2]-min[2]};
    return diff[0]*diff[1] + diff[1]*diff[2] + diff[0]*diff[2];
  }

  void SetBox(const Vector3 &min_, const Vector3 &max_)
  {
    this->min_ = min_; this->max_ = max_;
    centerPos_ = (min_+max_)/2.0;
  }

  inline bool Intersect(const Ray &ray, double &distance) const {
    Vector3 t_min(INF, INF, INF), t_max(-INF, -INF, -INF);
    double fastest_out_t = INF;
    double latest_in_t = -INF;

    double min_array[3] = {min_.x, min_.y, min_.z};
    double max_array[3] = {max_.x, max_.y, max_.z};
    double ray_dir_array[3] = {ray.dir.x, ray.dir.y, ray.dir.z};
    double ray_start_array[3] = {ray.begin.x, ray.begin.y, ray.begin.z};

    return CheckIntersection(ray_dir_array, ray_start_array, min_array, max_array, distance);
  }

  template <typename FLOATING> static bool CheckIntersection(FLOATING rayDir[3], FLOATING rayOrig[3], FLOATING minbox[3], FLOATING maxbox[3], FLOATING &distance) {
    FLOATING fastest_out_t = INF;
    FLOATING latest_in_t = -INF;

    for (int i=0; i<3; i++) {
      FLOATING t_min = INF, t_max = -INF;
      FLOATING inv = rayDir[i] != 0 ? 1.0/rayDir[i] : INF;
      FLOATING t1 = (minbox[i] - rayOrig[i])*inv;
      FLOATING t2 = (maxbox[i] - rayOrig[i])*inv;
      t_min = std::min(t1, t2);
      t_max = std::max(t1, t2);
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

  void MergeAnotherBox(const BoundingBox &b2) {
    min_.x = std::min(min_.x, b2.min_.x);
    min_.y = std::min(min_.y, b2.min_.y);
    min_.z = std::min(min_.z, b2.min_.z);
    max_.x = std::max(max_.x, b2.max_.x);
    max_.y = std::max(max_.y, b2.max_.y);
    max_.z = std::max(max_.z, b2.max_.z);
    centerPos_ = (min_+max_)/2;
  }

  const Vector3 &min() const {return min_;}
  const Vector3 &max() const {return max_;}
  const Vector3 &position() const {return centerPos_;}

private:
  Vector3 min_;
  Vector3 max_;
  Vector3 centerPos_;
};
