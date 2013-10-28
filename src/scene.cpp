#include "scene.h"
#include "bvh.h"

Scene::~Scene() {
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
  if (m_bvh) delete m_bvh;
}

void Scene::ConstructBVH()
{
  if (m_bvh) delete m_bvh;

  m_bvh = new BVH();
  m_bvh->Construct(BVH::CONSTRUCTION_OBJECT_SAH, m_inBVHObjects);
  //m_bvh->Construct(BVH::CONSTRUCTION_OBJECT_MEDIAN, m_inBVHObjects);
}

bool Scene::CheckIntersection(const Ray &ray, IntersectionInformation &info) const {
  info.hit.distance = INF;
  info.object = NULL;

  if (m_bvh) {
    m_bvh->CheckIntersection(ray, info);
  }

  std::vector<SceneObject *>::const_iterator it,end = m_notInBVHObjects.end();
  for (it = m_notInBVHObjects.begin(); it!=end; it++) {
    SceneObject *obj = *it;
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
