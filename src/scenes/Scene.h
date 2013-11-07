#pragma once
#include <vector>
#include "renderer/Ray.h"
#include "renderer/HitInformation.h"
#include "renderer/SceneObject.h"
#include "renderer/Model.h"
#include "tools/Constant.h"

namespace SimpleRenderer {

class SceneObject;
class BVH;
class QBVH;

class Scene {
public:
  struct IntersectionInformation {
    HitInformation hit;
    SceneObject *object;
  };

public:
  virtual ~Scene();

  void ConstructBVH();
  void ConstructQBVH();

  bool CheckIntersection(const Ray &ray, IntersectionInformation &info) const;

protected:
  Scene() : m_objects(), m_models(), m_inBVHObjects(), m_notInBVHObjects(), m_bvh(NULL), m_qbvh(NULL) {}

  void AddObject(SceneObject *obj, bool doDelete = true, bool containedInBVH = true) {
    m_objects.push_back(SceneObjectInfo(obj, doDelete, containedInBVH));
    if (containedInBVH) {
      m_inBVHObjects.push_back(obj);
    } else {
      m_notInBVHObjects.push_back(obj);
    }
  }

  void AddModel(Model *obj, bool doDelete = true, bool containedInBVH = true) {
    m_models.push_back(ModelObjectInfo(obj, doDelete));

    for (size_t i=0; i<obj->GetMaterialCount(); i++) {
      const Material &mat = obj->GetMaterial(i);
      const Model::PolygonList &pl = obj->GetPolygonList(mat);
      for (size_t j=0; j<pl.size(); j++) {
        AddObject(pl[j], false, containedInBVH);
      }
    }
  }

  struct SceneObjectInfo {
    SceneObjectInfo(SceneObject *obj, bool doDelete_, bool inBVH_)
      : object(obj)
      , doDelete(doDelete_)
      , inBVH(inBVH_)
    {
    }
    SceneObject *object;
    bool doDelete;
    bool inBVH;
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

  std::vector<SceneObject *> m_inBVHObjects;
  std::vector<SceneObject *> m_notInBVHObjects;
  std::vector<SceneObjectInfo> m_objects;
  std::vector<ModelObjectInfo> m_models;

  BVH *m_bvh;
  QBVH *m_qbvh;

private:
  Scene(const Scene &s) {}
  Scene &operator =(const Scene &s) {return *this;}

};

}
