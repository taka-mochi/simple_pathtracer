#pragma once

#include <algorithm>
#include "tools/Vector.h"
#include "Ray.h"
#include "tools/Constant.h"

namespace SimpleRenderer {

class BoundingBox {
public:
  BoundingBox(const Vector3 &minPos = Vector3(0,0,0), const Vector3 &maxPos = Vector3(0,0,0))
    : m_min(minPos)
    , m_max(maxPos)
    , m_centerPos((minPos+maxPos)/2.0)
  {
  }

  double CalcVolume() const {
    return (m_max.x-m_min.x)*(m_max.y-m_min.y)*(m_max.z-m_min.z);
  }
  double CalcSurfaceArea() const {
    double diff_x = m_max.x-m_min.x, diff_y = m_max.y-m_min.y, diff_z = m_max.z-m_min.z;
    return diff_x*diff_y + diff_x*diff_z + diff_y*diff_z;
  }

  template <typename FLOATING> static FLOATING CalcSurfaceArea(FLOATING min[3], FLOATING max[3]) {
    FLOATING diff[3] = {max[0]-min[0], max[1]-min[1], max[2]-min[2]};
    return diff[0]*diff[1] + diff[1]*diff[2] + diff[0]*diff[2];
  }

  void SetBox(const Vector3 &min_, const Vector3 &max_)
  {
    this->m_min = min_; this->m_max = max_;
    m_centerPos = (m_min+m_max)/2.0;
  }

  inline bool CheckIntersection(const Ray &ray, double &distance) const {
    Vector3 t_min(INF, INF, INF), t_max(-INF, -INF, -INF);
    double fastest_out_t = INF;
    double latest_in_t = -INF;

    double min_array[3] = {m_min.x, m_min.y, m_min.z};
    double max_array[3] = {m_max.x, m_max.y, m_max.z};
    double ray_dir_array[3] = {ray.dir.x, ray.dir.y, ray.dir.z};
    double ray_start_array[3] = {ray.orig.x, ray.orig.y, ray.orig.z};

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

  // Reference: http://d.hatena.ne.jp/ototoi/20090925/p1
  inline static bool CheckIntersection4floatAABB(
	  const __m128 bboxes[2][3], // 4boxes: min-max[2] * xyz[3] * boxes[4](__m128)
	  const __m128 rayOrig[3],   // ray origin
	  const __m128 rayInverseDir[3], // ray inversed dir
	  const int raySign[3],         // ray xyz direction => +:0, -:1
	  __m128 tmin, __m128 tmax,     // ray range tmin-tmax
	  bool results[4]               // intersection results
	  ) {

    // tmin = max(tmin, (box[min or max]-rayOrig)/rayDir)
    // tmax = min(tmax, (box[max or min]-rayOrig)/rayDir)
    // if tmax > tmin, intersects. Otherwise, no intersections

    // x
    tmin = _mm_max_ps(
      tmin, _mm_mul_ps(_mm_sub_ps(bboxes[raySign[0]][0], rayOrig[0]), rayInverseDir[0])
    );
    tmax = _mm_min_ps(
      tmax, _mm_mul_ps(_mm_sub_ps(bboxes[1 - raySign[0]][0], rayOrig[0]), rayInverseDir[0])
    );

    // y
    tmin = _mm_max_ps(
      tmin, _mm_mul_ps(_mm_sub_ps(bboxes[raySign[1]][1], rayOrig[1]), rayInverseDir[1])
      );
    tmax = _mm_min_ps(
      tmax, _mm_mul_ps(_mm_sub_ps(bboxes[1 - raySign[1]][1], rayOrig[1]), rayInverseDir[1])
      );

    // z
    tmin = _mm_max_ps(
      tmin, _mm_mul_ps(_mm_sub_ps(bboxes[raySign[2]][2], rayOrig[2]), rayInverseDir[2])
      );
    tmax = _mm_min_ps(
      tmax, _mm_mul_ps(_mm_sub_ps(bboxes[1 - raySign[2]][2], rayOrig[2]), rayInverseDir[2])
      );

    int ret = _mm_movemask_ps(_mm_cmpge_ps(tmax, tmin));
    if (ret == 0) return false;
    for (int i = 0; i < 4; i++) results[i] = ((ret >> i) & 0x1) != 0;
    return true;
  }

  inline static bool CheckIntersection2doubleAABB(
    const __m128d bboxes[2][3], // 4boxes: min-max[2] * xyz[3] * boxes[2](__m128)
    const __m128d rayOrig[3],   // ray origin
    const __m128d rayInverseDir[3], // ray inversed dir
    const int raySign[3],         // ray xyz direction => +:0, -:1
    __m128d tmin, __m128d tmax,     // ray range tmin-tmax
    bool results[2]               // intersection results
    ) {

    // tmin = max(tmin, (box[min or max]-rayOrig)/rayDir)
    // tmax = min(tmax, (box[max or min]-rayOrig)/rayDir)
    // if tmax > tmin, intersects. Otherwise, no intersections

    // x
    tmin = _mm_max_pd(
      tmin, _mm_mul_pd(_mm_sub_pd(bboxes[raySign[0]][0], rayOrig[0]), rayInverseDir[0])
      );
    tmax = _mm_min_pd(
      tmax, _mm_mul_pd(_mm_sub_pd(bboxes[1 - raySign[0]][0], rayOrig[0]), rayInverseDir[0])
      );

    // y
    tmin = _mm_max_pd(
      tmin, _mm_mul_pd(_mm_sub_pd(bboxes[raySign[1]][1], rayOrig[1]), rayInverseDir[1])
      );
    tmax = _mm_min_pd(
      tmax, _mm_mul_pd(_mm_sub_pd(bboxes[1 - raySign[1]][1], rayOrig[1]), rayInverseDir[1])
      );

    // z
    tmin = _mm_max_pd(
      tmin, _mm_mul_pd(_mm_sub_pd(bboxes[raySign[2]][2], rayOrig[2]), rayInverseDir[2])
      );
    tmax = _mm_min_pd(
      tmax, _mm_mul_pd(_mm_sub_pd(bboxes[1 - raySign[2]][2], rayOrig[2]), rayInverseDir[2])
      );

    int ret = _mm_movemask_pd(_mm_cmpge_pd(tmax, tmin));
    if (ret == 0) return false;
    for (int i = 0; i < 2; i++) results[i] = ((ret >> i) & 0x1) != 0;
    return true;
  }

  static BoundingBox CompoundBoxes(const BoundingBox &b1, const BoundingBox &b2) {
    return BoundingBox(Vector3(std::min(b1.min().x, b2.min().x), std::min(b1.min().y, b2.min().y), std::min(b1.min().z, b2.min().z)), 
        Vector3(std::max(b1.max().x, b2.max().x), std::max(b1.max().y, b2.max().y), std::max(b1.max().z, b2.max().z)));
  }

  void MergeAnotherBox(const BoundingBox &b2) {
    m_min.x = std::min(m_min.x, b2.m_min.x);
    m_min.y = std::min(m_min.y, b2.m_min.y);
    m_min.z = std::min(m_min.z, b2.m_min.z);
    m_max.x = std::max(m_max.x, b2.m_max.x);
    m_max.y = std::max(m_max.y, b2.m_max.y);
    m_max.z = std::max(m_max.z, b2.m_max.z);
    m_centerPos = (m_min+m_max)/2;
  }

  const Vector3 &min() const {return m_min;}
  const Vector3 &max() const {return m_max;}
  const Vector3 &position() const {return m_centerPos;}

private:
  Vector3 m_min;
  Vector3 m_max;
  Vector3 m_centerPos;
};

}
