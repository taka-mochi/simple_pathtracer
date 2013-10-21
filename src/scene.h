#pragma once
#include <vector>
#include "ray.h"
#include "hit.h"
#include "SceneObject.h"
#include "model.h"
#include "constant.h"

class SceneObject;

class Scene {
public:
  struct IntersectionInformation {
    HitInformation hit;
    SceneObject *object;
  };

public:
  ~Scene() {
    for (size_t i=0; i<m_objects.size(); i++) {
      SceneObjectInfo &info = m_objects.at(i);
      if (info.doDelete) {
        delete info.object;
      }
    }
    for (size_t i=0; i<m_models.size(); i++) {
      ModelObjectInfo &info = m_models.at(i);
      if (info.doDelete) {
        delete info.model;
      }
    }
  }

  bool CheckIntersection(const Ray &ray, IntersectionInformation &info) const {
    info.hit.distance = INF;
    info.object = NULL;

    for (size_t i=0; i<m_objects.size(); i++) {
      SceneObject *obj = m_objects[i].object;
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

  void addObject(SceneObject *obj, bool doDelete = true) {
    m_objects.push_back(SceneObjectInfo(obj, doDelete));
  }

  void addModel(Model *obj, bool doDelete = true) {
    m_models.push_back(ModelObjectInfo(obj, doDelete));

    for (size_t i=0; i<obj->getMaterialCount(); i++) {
      const Material &mat = obj->getMaterial(i);
      const Model::PolygonList &pl = obj->getPolygonList(mat);
      for (size_t j=0; j<pl.size(); j++) {
        addObject(pl[j], false);
      }
    }
  }

  struct SceneObjectInfo {
    SceneObjectInfo(SceneObject *obj, bool doDelete_)
      : object(obj)
      , doDelete(doDelete_)
    {
    }
    SceneObject *object;
    bool doDelete;
  };
  struct ModelObjectInfo {
    ModelObjectInfo(Model *model_, bool doDelete_)
      : model(model_)
      , doDelete(doDelete_)
    {
    }
    Model *model;
    bool doDelete;
  };

  std::vector<SceneObjectInfo> m_objects;
  std::vector<ModelObjectInfo> m_models;

private:
  Scene(const Scene &s) {}
  Scene &operator =(const Scene &s) {return *this;}
};
