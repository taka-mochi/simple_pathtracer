#pragma once

#include "SceneObject.h"
#include "ray.h"
#include "constant.h"
#include "hit.h"
#include "matrix.h"

class Polygon : public SceneObject {
public:
  Polygon(const Vector3 &pos1, const Vector3 &pos2, const Vector3 &pos3, const Vector3 &normal, const Material &mat, const Vector3 &pos)
    : SceneObject(mat)
    , m_normal(normal)
  {
    m_pos[0] = m_rotatedPos[0] = pos1;
    m_pos[1] = m_rotatedPos[1] = pos2;
    m_pos[2] = m_rotatedPos[2] = pos3;
    position = pos;
  }
  Polygon(const Polygon &polygon)
    : SceneObject(polygon.material)
  {
    for (int i=0; i<3; i++) {
      m_pos[i] = polygon.m_pos[i];
      m_rotatedPos[i] = polygon.m_rotatedPos[i];
    }
    m_normal = polygon.m_normal;
  }
  virtual ~Polygon() {}

  static Vector3 calculateNormal(const Vector3 &anticlockwise_v0, const Vector3 &anticlockwise_v1, const Vector3 &anticlockwise_v2) {
    Vector3 v((anticlockwise_v1-anticlockwise_v0).cross(anticlockwise_v2-anticlockwise_v0));
    v.normalize();
    return v;
  }

  void setRotation(const Matrix &matrix) {
    for (int i=0; i<3; i++) {
      m_rotatedPos[i] = matrix.apply(m_pos[i]);
    }
  }

  bool Intersect(const Ray &ray, HitInformation &hit) const {
    // ˜A—§•û’öŽ®‚ð‰ð‚­
    // ŽQl: http://shikousakugo.wordpress.com/2012/07/01/ray-intersection-3/
    Vector3 edge1(m_rotatedPos[1] - m_rotatedPos[0]);
    Vector3 edge2(m_rotatedPos[2] - m_rotatedPos[0]);

    Vector3 P(ray.dir.cross(edge2));
    double det = P.dot(edge1);

    if (det > EPS) {
      // solve u
      Vector3 T(ray.begin - (m_rotatedPos[0]+position));
      double u = P.dot(T);

      if (u>=0 && u<= det) {
        // solve v
        Vector3 Q(T.cross(edge1));
        double v = Q.dot(ray.dir);

        if (v>=0 && u+v<=det) {
          double t = Q.dot(edge2) / det;

          if (t>=EPS) {
            hit.distance = t;
            hit.position = ray.begin + ray.dir*t;
            hit.normal = m_normal;

            return true;
          }
        }
      }

    }

    return false;
  }

  Vector3 m_pos[3];
  Vector3 m_normal;

private:
  Vector3 m_rotatedPos[3];
};
