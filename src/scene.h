#pragma once
#include <vector>
#include "ray.h"
#include "hit.h"
#include "SceneObject.h"
#include "constant.h"

class SceneObject;

class Scene {
public:
  struct IntersectionInformation {
    HitInformation hit;
    SceneObject *object;
  };

public:
  virtual ~Scene() {}

  bool CheckIntersection(const Ray &ray, IntersectionInformation &info) const {
    info.hit.distance = INF;
    info.object = NULL;

    for (size_t i=0; i<m_objects.size(); i++) {
      SceneObject *obj = m_objects[i];
      HitInformation hit;
      if (obj->Intersect(ray, hit)) {
        if (info.hit.distance > hit.distance) {
          info.hit = hit;
          info.object = obj;
        }
      }
    }
    return info.hit.distance != INF;
  }

protected:
  Scene() : m_objects() {}

  void addObject(SceneObject *obj) {
    m_objects.push_back(obj);
  }

  std::vector<SceneObject *> m_objects;

private:
  Scene(const Scene &s) {}
  Scene &operator =(const Scene &s) {return *this;}
};
