#pragma once

#include <vector>
#include "scenes/Scene.h"

namespace SimpleRenderer {
  class SceneObject;
  class Ray;
  struct Scene::IntersectionInformation;

  class QBVH {
  public:
    explicit QBVH() : m_root(), m_leafObjectArray(NULL) {}
    ~QBVH();

    void Construct(const std::vector<SceneObject *> &targets);
    bool CheckIntersection(const Ray &ray, Scene::IntersectionInformation &info) const;

  private:
    void Construct_internal(const std::vector<SceneObject *> &targets, int index);
    void MakeLeaf_internal(const std::vector<SceneObject *> &targets, int index);

    bool CheckIntersection_Leaf(const Ray &ray, size_t leafStartIndex, Scene::IntersectionInformation &hitResultDetail) const;

  private:
    struct QBVH_structure;
    std::vector<QBVH_structure> m_root;
    std::vector<SceneObject *> m_leafObjectArray;
    
  };
}
